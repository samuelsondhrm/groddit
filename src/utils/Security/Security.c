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
        return true;

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
        *out_len = fsize;
        if (was_encrypted)
            *was_encrypted = false;
        return disk;
    }

    // Check magic
    if (!(disk[0] == 'G' && disk[1] == 'R' && disk[2] == 'E' && disk[3] == 'N'))
    {
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
        return NULL;
    }

    uint8_t version = disk[4];
    if (version != 1)
    {
        free(disk);
        return NULL;
    }

    size_t payload_len = fsize - 5;
    uint8_t *cipher = disk + 5;

    uint8_t *plain = malloc(payload_len);
    memcpy(plain, cipher, payload_len);

    crypt_buffer(plain, payload_len);

    *out_len = payload_len;
    return plain;
}

void migrate_passwords_in_memory(void)
{
    extern User *USERS;
    extern int USER_COUNT;

    for (int i = 0; i < USER_COUNT; ++i)
    {
        char pwbuf[512];
        wordToString_safe(pwbuf, sizeof(pwbuf), USERS[i].password);

        uint32_t h = hash_password(pwbuf);

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
    clearScreen();
    printBreadcrumb("Home > Security Settings");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                        %sSECURITY SETTINGS%s                         %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printError("Invalid command format");
        printf("Security option is required.\n\n");
        printf("%sUsage:%s\n", BOLD_WHITE, RESET);
        printf("  %sSET_SECURITY PASSWORD;%s          - Enable password hashing\n", BOLD_CYAN, RESET);
        printf("  %sSET_SECURITY FILE <ON|OFF>;%s     - Toggle file encryption\n", BOLD_CYAN, RESET);
        printf("  %sSET_SECURITY ENC_SEED <number>;%s - Set encryption seed\n", BOLD_CYAN, RESET);
        return;
    }

    char key[64];
    wordToString(key, currentWord);

    if (strcmp(key, "PASSWORD") == 0)
    {
        ADVWORD_INPUT();
        if (currentWord.Length != 0)
        {
            while (currentWord.Length != 0)
            {
                ADVWORD_INPUT();
            }
            printError("Invalid command format");
            printf("Too many arguments provided.\n\n");
            printf("%sUsage:%s %sSET_SECURITY PASSWORD;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
            return;
        }

        printf("\n");
        printSectionHeader("", "PASSWORD HASHING");
        printf("\n");

        if (g_pwd_hash)
        {
            printf("%s %s⚠ Password hashing is already enabled%s\n", BOX_V, BOLD_YELLOW, RESET);
            printSectionDivider();
            printf("\n");
            printInfo("Current status: ");
            printf("%sENABLED%s - All passwords are hashed using FNV-1a 32-bit\n", BOLD_GREEN, RESET);
        }
        else
        {
            spinnerAnimation("Enabling password hashing", 8);
            printf("\n\n");
            enable_password_hashing();
            printf("%s %s[SUCCESS] Password hashing enabled%s\n", BOX_V, GREEN, RESET);
            printf("%s %sMigrating existing passwords...%s", BOX_V, DIM, RESET);
            fflush(stdout);
            printf(" %sDone%s\n", BOLD_GREEN, RESET);
            printSectionDivider();
            printf("\n");
            printSuccess("Security enhanced!");
            printf("All passwords are now hashed using %sFNV-1a 32-bit%s algorithm.\n\n", BOLD_CYAN, RESET);
            printf("%sNote:%s Remember to %sSAVE;%s to persist this change.\n", BOLD_WHITE, RESET, BOLD_YELLOW, RESET);
        }
        return;
    }

    if (strcmp(key, "FILE") == 0)
    {
        ADVWORD_INPUT();
        if (currentWord.Length == 0)
        {
            printError("Invalid command format");
            printf("Value is required: %sON%s or %sOFF%s\n\n", BOLD_GREEN, RESET, BOLD_RED, RESET);
            printf("%sUsage:%s %sSET_SECURITY FILE <ON|OFF>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
            printf("%sExample:%s %sSET_SECURITY FILE ON;%s\n", DIM, RESET, BOLD_WHITE, RESET);
            return;
        }

        char val[64];
        wordToString(val, currentWord);

        ADVWORD_INPUT();
        if (currentWord.Length != 0)
        {
            while (currentWord.Length != 0)
            {
                ADVWORD_INPUT();
            }
            printError("Invalid command format");
            printf("Too many arguments provided.\n\n");
            printf("%sUsage:%s %sSET_SECURITY FILE <ON|OFF>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
            return;
        }

        printf("\n");
        printSectionHeader("", "FILE ENCRYPTION");
        printf("\n");

        if (strcmp(val, "ON") == 0)
        {
            if (g_file_enc)
            {
                printf("%s %s⚠ File encryption is already enabled%s\n", BOX_V, BOLD_YELLOW, RESET);
                printSectionDivider();
                printf("\n");
                printInfo("Current status: ");
                printf("%sENABLED%s - All files are encrypted with LCG keystream\n", BOLD_GREEN, RESET);
            }
            else
            {
                spinnerAnimation("Enabling file encryption", 8);
                printf("\n\n");
                g_file_enc = true;
                printf("%s %s[SUCCESS] File encryption enabled%s\n", BOX_V, GREEN, RESET);
                printf("%s %sUsing LCG keystream cipher%s\n", BOX_V, DIM, RESET);
                printf("%s %sCurrent seed: %s%u%s\n", BOX_V, DIM, BOLD_WHITE, g_seed, RESET);
                printSectionDivider();
                printf("\n");
                printSuccess("File encryption activated!");
                printf("All future SAVE operations will encrypt CSV files.\n\n");
                printf("%sNote:%s Remember to %sSAVE;%s to persist this change.\n", BOLD_WHITE, RESET, BOLD_YELLOW, RESET);
            }
        }
        else if (strcmp(val, "OFF") == 0)
        {
            if (!g_file_enc)
            {
                printf("%s %s⚠ File encryption is already disabled%s\n", BOX_V, BOLD_YELLOW, RESET);
                printSectionDivider();
                printf("\n");
                printInfo("Current status: ");
                printf("%sDISABLED%s - Files are saved in plain text\n", BOLD_RED, RESET);
            }
            else
            {
                spinnerAnimation("Disabling file encryption", 8);
                printf("\n\n");
                g_file_enc = false;
                printf("%s %s[SUCCESS] File encryption disabled%s\n", BOX_V, GREEN, RESET);
                printSectionDivider();
                printf("\n");
                printWarning("Security reduced!");
                printf("Future SAVE operations will store files in %splain text%s.\n\n", BOLD_YELLOW, RESET);
                printf("%sNote:%s Remember to %sSAVE;%s to persist this change.\n", BOLD_WHITE, RESET, BOLD_YELLOW, RESET);
            }
        }
        else
        {
            printError("Invalid value");
            printf("Unknown value: %s%s%s\n\n", BOLD_RED, val, RESET);
            printf("%sValid options:%s %sON%s or %sOFF%s\n", BOLD_WHITE, RESET, BOLD_GREEN, RESET, BOLD_RED, RESET);
        }

        return;
    }

    if (strcmp(key, "ENC_SEED") == 0)
    {
        ADVWORD_INPUT();
        if (currentWord.Length == 0)
        {
            printError("Invalid command format");
            printf("Seed value is required.\n\n");
            printf("%sUsage:%s %sSET_SECURITY ENC_SEED <number>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
            printf("%sExample:%s %sSET_SECURITY ENC_SEED 987654321;%s\n", DIM, RESET, BOLD_WHITE, RESET);
            return;
        }

        char val[64];
        wordToString(val, currentWord);

        ADVWORD_INPUT();
        if (currentWord.Length != 0)
        {
            while (currentWord.Length != 0)
            {
                ADVWORD_INPUT();
            }
            printError("Invalid command format");
            printf("Too many arguments provided.\n\n");
            printf("%sUsage:%s %sSET_SECURITY ENC_SEED <number>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
            return;
        }

        // Validate numeric input
        boolean valid = true;
        for (int i = 0; val[i] != '\0'; i++)
        {
            if (val[i] < '0' || val[i] > '9')
            {
                valid = false;
                break;
            }
        }

        if (!valid)
        {
            printError("Invalid seed value");
            printf("Seed must be a positive number.\n\n");
            printf("%sExample:%s %sSET_SECURITY ENC_SEED 987654321;%s\n", DIM, RESET, BOLD_WHITE, RESET);
            return;
        }

        uint32_t oldSeed = g_seed;
        int newSeed = atoi(val);
        g_seed = (uint32_t)newSeed;

        printf("\n");
        printSectionHeader("", "ENCRYPTION SEED");
        printf("\n");
        spinnerAnimation("Updating encryption seed", 8);
        printf("\n\n");
        printf("%s %sPrevious seed:%s %s%u%s\n", BOX_V, DIM, RESET, BOLD_RED, oldSeed, RESET);
        printf("%s %sNew seed:%s      %s%u%s\n", BOX_V, DIM, RESET, BOLD_GREEN, g_seed, RESET);
        printSectionDivider();
        printf("\n");
        printSuccess("Encryption seed updated!");
        printf("LCG keystream will use the new seed: %s%u%s\n\n", BOLD_CYAN, g_seed, RESET);
        printf("%sWarning:%s Files encrypted with different seeds %scannot be decrypted%s!\n", 
               BOLD_YELLOW, RESET, BOLD_RED, RESET);
        printf("%sNote:%s Remember to %sSAVE;%s to persist this change.\n", BOLD_WHITE, RESET, BOLD_YELLOW, RESET);

        return;
    }

    printError("Unknown security option");
    printf("Unrecognized key: %s%s%s\n\n", BOLD_RED, key, RESET);
    printf("%sAvailable options:%s\n", BOLD_WHITE, RESET);
    printf("  • %sPASSWORD%s   - Enable password hashing\n", BOLD_CYAN, RESET);
    printf("  • %sFILE%s       - Toggle file encryption (ON/OFF)\n", BOLD_CYAN, RESET);
    printf("  • %sENC_SEED%s   - Set encryption seed (number)\n", BOLD_CYAN, RESET);
}