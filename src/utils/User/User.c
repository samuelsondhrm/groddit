#include "User.h"

int findIdByUsername(const char *username)
{
    for (int i = 0; i < USER_COUNT; i++)
    {
        char currUsername[256];
        wordToString(currUsername, USERS[i].username);
        if (strCmp(currUsername, username) == 0)
            return i;
    }
    return -1;
}

char *findUsernameById(int id)
{
    static char buffer[256];

    if (id < 0 || id >= USER_COUNT)
    {
        buffer[0] = '\0';
        return buffer;
    }

    wordToString(buffer, USERS[id].username);
    return buffer;
}

int findUserIndexById(const char *userId)
{
    if (userId == NULL || userId[0] == '\0')
        return IDX_UNDEF;
    char idStr[16];
    for (int i = 0; i < USER_COUNT; i++)
    {
        wordToString_safe(idStr, sizeof(idStr), USERS[i].user_id);
        if (strCmp(idStr, userId) == 0)
            return i;
    }
    return IDX_UNDEF;
}

void generateUserID(char *id, int num)
{
    id[0] = 'U';
    id[1] = 'S';
    id[2] = 'E';
    id[3] = 'R';
    int hundreds = (num / 100) % 10;
    int tens = (num / 10) % 10;
    int ones = num % 10;
    id[4] = '0' + hundreds;
    id[5] = '0' + tens;
    id[6] = '0' + ones;
    id[7] = '\0';
}

void registerUser()
{
    if (CURRENT_USER_INDEX != -1)
    {
        char *usn;
        usn = findUsernameById(CURRENT_USER_INDEX);
        printf("Anda tidak dapat melakukan registrasi karena telah login sebagai %s\n", usn);
        return;
    }

    char username[256];
    char password[25];
    int usernameValid = 0;
    int passwordValid = 0;

    // Username validation loop
    while (!usernameValid)
    {
        printf("Masukkan username: ");
        fflush(stdout);
        STARTWORD_INPUT();
        wordToString(username, currentWord);
        IgnoreNewline();

        if (findIdByUsername(username) != -1)
        {
            printf("Maaf, username %s sudah terdaftar :(. Harap pilih username yang lain.\n", username);
            continue;
        }

        usernameValid = 1;
    }

    // Password validation loop
    while (!passwordValid)
    {
        printf("Masukkan kata sandi: ");
        fflush(stdout);
        STARTWORD_INPUT();
        wordToString(password, currentWord);

        IgnoreNewline();

        int passLen = 0;
        while (password[passLen] != '\0')
            passLen++;

        if (passLen < 8 || passLen > 20)
        {
            printf("Panjang kata sandi harus 8-20 karakter! Silakan coba lagi.\n");
            continue;
        }

        passwordValid = 1;
    }

    if (!ensureCapacity((void **)&USERS, &USER_CAPACITY, sizeof(User), USER_COUNT + 1))
    {
        printf("Gagal menambah user, kapasitas penuh!\n");
        return;
    }

    User *u = &USERS[USER_COUNT];

    char uid[8];
    generateUserID(uid, USER_COUNT + 1);

    Word uidWord;
    stringToWord(&uidWord, uid);
    copyWord(&u->user_id, uidWord);

    Word usn;
    Word pw;
    if (password_hashing_enabled())
    {
        uint32_t h = hash_password(password);
        uint32ToWord(&pw, h);
    }
    else
    {
        stringToWord(&pw, password);
    }
    stringToWord(&usn, username);
    copyWord(&u->username, usn);
    copyWord(&u->password, pw);
    u->karma = 0;
    u->created_at = time(NULL);

    USER_COUNT++;
    addVertex(&SOCIAL_GRAPH);

    printf("Akun dengan username %s berhasil didaftarkan! Silahkan gunakan perintah LOGIN untuk mengakses fitur-fitur Groddit.\n", username);
}

void loginUser()
{
    if (CURRENT_USER_INDEX != -1)
    {
        char *usn = findUsernameById(CURRENT_USER_INDEX);
        printf("Anda tidak dapat melakukan login karena telah login sebagai %s.\n", usn);
        return;
    }

    char username[256];
    char inputPw[256];

    printf("Masukkan username: ");
    STARTWORD_INPUT();
    wordToString(username, currentWord);
    IgnoreNewline();

    int user_id = findIdByUsername(username);
    if (user_id == -1)
    {
        printf("Maaf, username %s tidak ditemukan :(. Harap register terlebih dahulu.\n", username);
        return;
    }

    printf("Masukkan kata sandi: ");
    STARTWORD_INPUT();
    wordToString(inputPw, currentWord);
    IgnoreNewline();

    int len = 0;
    while (inputPw[len] != '\0')
        len++;

    if (len < 8 || len > 20)
    {
        printf("Panjang kata sandi harus 8-20 karakter!\n");
        return;
    }

    char storedPw[256];
    wordToString(storedPw, USERS[user_id].password);

    int stored_is_digits = 1;
    for (int i = 0; storedPw[i] != '\0'; i++)
    {
        if (storedPw[i] < '0' || storedPw[i] > '9')
        {
            stored_is_digits = 0;
            break;
        }
    }

    if (password_hashing_enabled())
    {
        uint32_t hashedInput = hash_password(inputPw);

        if (stored_is_digits)
        {
            uint32_t storedHash = (uint32_t)wordToInt(USERS[user_id].password);

            if (hashedInput != storedHash)
            {
                printf("Password salah!\n");
                return;
            }

            CURRENT_USER_INDEX = user_id;
            printf("Akun dengan username %s berhasil login! Selamat datang di Groddit!\n", username);
            return;
        }

        if (!stored_is_digits)
        {
            if (strcmp(inputPw, storedPw) != 0)
            {
                printf("Password salah!\n");
                return;
            }

            CURRENT_USER_INDEX = user_id;
            printf("Akun dengan username %s berhasil login! (plaintext legacy)\n", username);
            return;
        }
    }
    else
    {
        if (strcmp(inputPw, storedPw) != 0)
        {
            printf("Password salah!\n");
            return;
        }

        CURRENT_USER_INDEX = user_id;
        printf("Akun dengan username %s berhasil login! Selamat datang di Groddit!\n", username);
        return;
    }
}

void logoutUser()
{
    if (CURRENT_USER_INDEX == -1)
    {
        printf("Anda belum login! Masuk terlebih dahulu untuk dapat mengakses Groddit.\n");
        return;
    }

    CURRENT_USER_INDEX = -1;

    if (CURRENT_USER_INDEX == -1)
    {
        printf("Anda berhasil logout. Sampai jumpa di pertemuan berikutnya!\n");
    }
    else
    {
        printf("Gagal Logout!\n");
    }

    printf("\n");
    return;
}