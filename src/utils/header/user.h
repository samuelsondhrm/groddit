#include <stdio.h>
#include <stdlib.h>
#include "global-data.h"
#include "../../adt/header/MesinKata.h"
#include "helper.h"

// Mengecek apakah username sebelumnya telah ada atau belum
int findUsername(const char *username);

// Men-generate USERID yang baru
void generateUserID(char *id, int num);

// Register user baru
void registerUser();

// User login
void loginUser();

// User logout
void logoutUser();
// Search index user dari USERS berdasarkan masukan username
int findUserIndexByUsername(const char *username);

// Menghasilkan username dari USERS berdasarkan masukan index
const char *getUsernameByIndex(int idx);
