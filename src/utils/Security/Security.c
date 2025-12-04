#include "Security.h"

static boolean g_pwd_hash = false;
static boolean g_file_enc = false;
static uint32_t g_seed = 123456789u; // default seed
static uint32_t g_keystream_state = 0;

char global_security_conf_path[256];

#define CONFIG_LINE_LEN 256

static boolean load_security_conf(const char *path);

boolean save_security_conf(const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f)
        return false;
    fprintf(f, "PASSWORD_HASHING=%d\n", g_pwd_hash ? 1 : 0);
    fprintf(f, "FILE_ENCRYPTION=%d\n", g_file_enc ? 1 : 0);
    fprintf(f, "ENC_SEED=%u\n", g_seed);
    fclose(f);
    return true;
}

boolean security_init(const char *pathConf)
{
    struct stat st;
    boolean exists = (stat(pathConf, &st) == 0);

    if (!exists)
    {
        g_pwd_hash = false;
        g_file_enc = false;
        g_seed = 123456789;

        return false;
    }

    if (!load_security_conf(pathConf))
    {
        printf("[Warning] security.conf ada tapi gagal dibaca!\n");
        return false;
    }

    return true;
}

boolean password_hashing_enabled(void) { return g_pwd_hash; }
boolean file_encryption_enabled(void) { return g_file_enc; }

boolean enable_password_hashing(void)
{
    if (g_pwd_hash)
        return true; /* already enabled */

    g_pwd_hash = true;

    migrate_passwords_in_memory();

    return true;
}

boolean set_file_encryption(boolean on)
{
    g_file_enc = on;
    return true;
}

void security_set_seed(uint32_t seed)
{
    g_seed = seed;
}

uint32_t security_get_seed(void) { return g_seed; }

/* FNV-1a 32-bit */
uint32_t hash_password(const char *pw)
{
    uint32_t hash = 2166136261u;
    for (size_t i = 0; pw[i] != '\0'; i++)
    {
        hash ^= (uint8_t)pw[i];
        hash *= 16777619u;
    }
    return hash;
}

/* LCG keystream (state uses g_keystream_state) */
void reset_keystream(void)
{
    g_keystream_state = g_seed;
}
static uint8_t next_keystream_byte(void)
{
    // LCG params (glibc-like)
    g_keystream_state = (uint32_t)(g_keystream_state * 1664525u + 1013904223u);
    // take high byte for more entropy
    return (uint8_t)(g_keystream_state >> 24);
}

void crypt_buffer(uint8_t *buf, size_t n)
{
    reset_keystream();
    for (size_t i = 0; i < n; i++)
    {
        buf[i] ^= next_keystream_byte();
    }
}

boolean write_encrypted_file(const char *path, const uint8_t *buf, size_t len)
{
    FILE *f = fopen(path, "wb");
    if (!f)
        return false;

    if (!g_file_enc)
    {
        fwrite(buf, 1, len, f);
        fclose(f);
        return true;
    }

    // Write header: G R E N + version(1)
    fwrite("GREN", 1, 4, f);
    uint8_t version = 1;
    fwrite(&version, 1, 1, f);

    // Encrypt
    uint8_t *cpy = malloc(len);
    memcpy(cpy, buf, len);
    crypt_buffer(cpy, len);

    fwrite(cpy, 1, len, f);
    free(cpy);
    fclose(f);
    return true;
}

// returns malloc'd buffer (must free). out_len set. was_encrypted set accordingly.
uint8_t *read_encrypted_file(const char *path, size_t *out_len, boolean *was_encrypted)
{
    FILE *f = fopen(path, "rb");
    if (!f)
        return NULL;

    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    uint8_t *disk = malloc(fsize);
    fread(disk, 1, fsize, f);
    fclose(f);

    if (fsize < 4)
    {
        // too small → treat as plaintext
        *out_len = fsize;
        if (was_encrypted)
            *was_encrypted = false;
        return disk;
    }

    // Check magic
    if (!(disk[0] == 'G' && disk[1] == 'R' && disk[2] == 'E' && disk[3] == 'N'))
    {
        // plaintext
        *out_len = fsize;
        if (was_encrypted)
            *was_encrypted = false;
        return disk;
    }

    // encrypted
    if (was_encrypted)
        *was_encrypted = true;

    if (fsize < 5)
    {
        free(disk);
        return NULL; // malformed
    }

    uint8_t version = disk[4];
    if (version != 1)
    {
        printf("[READ-DBG] Unsupported encryption version %u\n", version);
        free(disk);
        return NULL;
    }

    // ciphertext begins AFTER version byte
    size_t payload_len = fsize - 5;
    uint8_t *cipher = disk + 5;

    // Debug
    printf("[READ-DBG] %s: fsize=%zu g_seed=%u\n", path, fsize, g_seed);
    printf("[READ-DBG] raw first 8 bytes: %02X %02X %02X %02X %02X %02X %02X %02X\n",
           disk[0], disk[1], disk[2], disk[3], disk[4], disk[5], disk[6], disk[7]);

    printf("[READ-DBG] GREN detected. cipher first 24 bytes: ");
    for (size_t i = 0; i < 24 && i < payload_len; i++)
        printf("%02X ", cipher[i]);
    printf("\n");

    // decrypt into new buffer
    uint8_t *plain = malloc(payload_len);
    memcpy(plain, cipher, payload_len);

    crypt_buffer(plain, payload_len);

    *out_len = payload_len;
    return plain;
}

void migrate_passwords_in_memory(void)
{
    extern User *USERS;    // asumsi nama global di project
    extern int USER_COUNT; // jumlah user saat ini

    for (int i = 0; i < USER_COUNT; ++i)
    {
        char pwbuf[512];
        /* ambil password yang sekarang (bisa plain atau already hashed) */
        wordToString_safe(pwbuf, sizeof(pwbuf), USERS[i].password);

        /* compute hash on current string (this is irreversible) */
        uint32_t h = hash_password(pwbuf);

        /* replace user.password word with decimal string of hash */
        uint32ToWord(&USERS[i].password, h);
    }
}

boolean load_security_conf(const char *path)
{
    g_pwd_hash = false;
    g_file_enc = false;
    g_seed = 123456789u;

    boolean seen_pwd = false;
    boolean seen_file = false;
    boolean seen_seed = false;

    STARTCONF(path);

    if (EOP)
    {
        printf("[Warning] security.conf kosong. Menggunakan default.\n");
        return false;
    }

    while (!EOP)
    {
        while (!EOP && (currentChar == '\n' || currentChar == '\r'))
            ADVCONF();
        if (EOP)
            break;

        Word key;
        key.Length = 0;

        while (!EOP &&
               currentChar != '=' &&
               currentChar != '\n' &&
               currentChar != '\r')
        {
            if (currentChar != ' ' && currentChar != '\t')
            {
                if (key.Length < NMax)
                    key.TabWord[key.Length++] = currentChar;
            }
            ADVCONF();
        }
        key.TabWord[key.Length] = '\0';

        if (EOP || currentChar != '=')
        {
            printf("[Warning] Format security.conf salah (key tanpa '=' ). Abaikan baris.\n");
            continue;
        }

        ADVCONF();

        Word val;
        val.Length = 0;

        while (!EOP && (currentChar == ' ' || currentChar == '\t'))
            ADVCONF();

        while (!EOP &&
               currentChar != '\n' &&
               currentChar != '\r')
        {
            if (currentChar != ' ' && currentChar != '\t')
            {
                if (val.Length < NMax)
                    val.TabWord[val.Length++] = currentChar;
            }
            ADVCONF();
        }
        val.TabWord[val.Length] = '\0';

        if (strcmp(key.TabWord, "PASSWORD_HASHING") == 0)
        {
            seen_pwd = true;

            if (val.Length == 1 && (val.TabWord[0] == '0' || val.TabWord[0] == '1'))
            {
                g_pwd_hash = (val.TabWord[0] == '1');
            }
            else
            {
                printf("[Warning] Nilai PASSWORD_HASHING invalid. Gunakan default (0)\n");
                g_pwd_hash = false;
            }
        }

        else if (strcmp(key.TabWord, "FILE_ENCRYPTION") == 0)
        {
            seen_file = true;

            if (val.Length == 1 && (val.TabWord[0] == '0' || val.TabWord[0] == '1'))
            {
                g_file_enc = (val.TabWord[0] == '1');
            }
            else
            {
                printf("[Warning] Nilai FILE_ENCRYPTION invalid. Gunakan default (0)\n");
                g_file_enc = false;
            }
        }

        else if (strcmp(key.TabWord, "ENC_SEED") == 0)
        {
            seen_seed = true;

            if (val.Length == 0)
            {
                printf("[Warning] ENC_SEED kosong. Gunakan default (123456789)\n");
                g_seed = 123456789u;
            }
            else
            {
                boolean valid = true;
                for (int i = 0; i < val.Length; i++)
                    if (val.TabWord[i] < '0' || val.TabWord[i] > '9')
                        valid = false;

                if (!valid)
                {
                    printf("[Warning] ENC_SEED bukan angka valid. Gunakan default (123456789)\n");
                    g_seed = 123456789u;
                }
                else
                {
                    g_seed = (uint32_t)atoi(val.TabWord);
                }
            }
        }

        else
        {
            printf("[Warning] Key '%s' tidak dikenal. Diabaikan.\n", key.TabWord);
        }

        if (!EOP)
            ADVCONF();
    }

    if (!seen_pwd)
    {
        printf("[Warning] PASSWORD_HASHING tidak ditemukan. Menggunakan default (0)\n");
        g_pwd_hash = false;
    }
    if (!seen_file)
    {
        printf("[Warning] FILE_ENCRYPTION tidak ditemukan. Menggunakan default (0)\n");
        g_file_enc = false;
    }
    if (!seen_seed)
    {
        printf("[Warning] ENC_SEED tidak ditemukan. Menggunakan default (123456789)\n");
        g_seed = 123456789u;
    }

    return true;
}

void handleSecurityCommand()
{
    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printf("Format salah. Gunakan: SET_SECURITY <KEY>;\n");
        return;
    }

    char key[64];
    wordToString(key, currentWord);

    if (strcmp(key, "PASSWORD") == 0)
    {
        ADVWORD_INPUT();
        if (currentWord.Length != 0)
        {
            printf("Format salah. Gunakan: SET_SECURITY PASSWORD;\n");
            return;
        }

        if (g_pwd_hash)
        {
            printf("PASSWORD_HASHING sudah aktif sebelumnya.\n");
        }
        else
        {
            enable_password_hashing();
            printf("PASSWORD_HASHING diaktifkan.\n");
        }
        return;
    }

    if (strcmp(key, "FILE") == 0)
    {
        ADVWORD_INPUT();
        if (currentWord.Length == 0)
        {
            printf("Format salah. Gunakan: SET_SECURITY FILE <ON|OFF>;\n");
            return;
        }

        char val[64];
        wordToString(val, currentWord);

        if (strcmp(val, "ON") == 0)
        {
            g_file_enc = true;
            printf("FILE_ENCRYPTION diaktifkan.\n");
        }
        else if (strcmp(val, "OFF") == 0)
        {
            g_file_enc = false;
            printf("FILE_ENCRYPTION dimatikan.\n");
        }
        else
        {
            printf("Nilai tidak dikenal. Gunakan 'ON' atau 'OFF'.\n");
        }

        return;
    }

    if (strcmp(key, "ENC_SEED") == 0)
    {
        ADVWORD_INPUT();
        if (currentWord.Length == 0)
        {
            printf("Format: SET_SECURITY ENC_SEED <number>;\n");
            return;
        }

        char val[64];
        wordToString(val, currentWord);

        int newSeed = atoi(val);
        g_seed = (uint32_t)newSeed;

        printf("ENC_SEED diubah menjadi %u.\n", g_seed);
        return;
    }

    printf("KEY '%s' tidak dikenal di SET_SECURITY.\n", key);
}