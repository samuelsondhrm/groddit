// #include "header/user.h"

// int findUsername(const char *username)
// {
//     for (int i = 0; i < USER_COUNT; i++)
//     {
//         if (strCmp(USERS[i].username, username) == 0)
//             return i;
//     }
//     return -1;
// }

// void generateUserID(char *id, int num)
// {
//     id[0] = 'U';
//     id[1] = 'S';
//     id[2] = 'E';
//     id[3] = 'R';
//     int hundreds = (num / 100) % 10;
//     int tens = (num / 10) % 10;
//     int ones = num % 10;
//     id[4] = '0' + hundreds;
//     id[5] = '0' + tens;
//     id[6] = '0' + ones;
//     id[7] = '\0';
// }

// void registerUser()
// {
//     if (CURRENT_USER != NULL)
//     {
//         printf("Anda tidak dapat melakukan registrasi karena telah login sebagai %s\n", CURRENT_USER->username);
//         return;
//     }

//     char username[256];
//     char password[25];

//     printf("Masukkan username: ");
//     STARTWORD();
//     wordToString(username, currentWord);
//     IgnoreNewline();

//     if (findUsername(username) != -1)
//     {
//         printf("Maaf, username %s sudah terdaftar :(. Harap pilih username yang lain.\n", username);
//         return;
//     }

//     printf("Masukkan kata sandi: ");
//     STARTWORD();
//     wordToString(password, currentWord);
//     IgnoreNewline();

//     int passLen = 0;
//     while (password[passLen] != '\0')
//         passLen++;

//     if (passLen < 8 || passLen > 20)
//     {
//         printf("Panjang kata sandi harus 8-20 karakter!\n");
//         return;
//     }

//     if (!ensureCapacity((void **)&USERS, &USER_CAPACITY, sizeof(User), USER_COUNT + 1))
//     {
//         printf("Gagal menambah user, kapasitas penuh!\n");
//         return;
//     }

//     User *u = &USERS[USER_COUNT];
//     generateUserID(u->user_id, USER_COUNT + 1);
//     copyString(u->username, username, 256);
//     copyString(u->password, password, 25);
//     u->karma = 0;
//     u->created_at = time(NULL);

//     USER_COUNT++;

//     printf("Akun dengan username %s berhasil didaftarkan! Silahkan gunakan perintah LOGIN untuk mengakses fitur-fitur Groddit.\n", username);
// }

// void loginUser()
// {
//     if (CURRENT_USER != NULL)
//     {
//         printf("Anda tidak dapat melakukan login karena telah login sebagai %s.\n", CURRENT_USER->username);
//         return;
//     }

//     char username[256];
//     char password[25];

//     printf("Masukkan username: ");
//     STARTWORD();
//     wordToString(username, currentWord);
//     IgnoreNewline();

//     int user_id = findUsername(username);
//     if (user_id == -1)
//     {
//         printf("Maaf, username %s tidak ditemukan :(. Harap register terlebih dahulu.\n", username);
//         return;
//     }

//     printf("Masukkan kata sandi: ");
//     STARTWORD();
//     wordToString(password, currentWord);
//     IgnoreNewline();

//     int passLen = 0;
//     while (password[passLen] != '\0')
//         passLen++;

//     if (passLen < 8 || passLen > 20)
//     {
//         printf("Panjang kata sandi harus 8-20 karakter!\n");
//         return;
//     }

//     if (strCmp(password, USERS[user_id].password) != 0)
//     {
//         printf("Password salah!\n");
//         return;
//     }

//     CURRENT_USER = &USERS[user_id];

//     printf("Akun dengan username %s berhasil login! Selamat datang di Groddit!\n", username);
// }

// void logoutUser()
// {
//     if (CURRENT_USER == NULL)
//     {
//         printf("Anda belum login! Masuk terlebih dahulu untuk dapat mengakses Groddit.\n");
//         return;
//     }

//     CURRENT_USER = NULL;

//     if (CURRENT_USER == NULL)
//     {
//         printf("Anda berhasil logout. Sampai jumpa di pertemuan berikutnya!\n");
//     }
//     else
//     {
//         printf("Gagal Logout!\n");
//     }

//     return;
// }

// int findUserIndexByUsername(const char *username){
//     //TODO Implementasi fungsinya boy
// }
// const char *getUsernameByIndex(int idx){
//     //TODO Implementasi fungsinya boy
// }