#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include "../../adt/MesinKata/MesinKata.h"
#include "../GlobalData/GlobalData.h"
#include "../Helper/Helper.h"
#include "../Security/Security.h"
#include "../Kreativitas/Kreativitas.h"

// Mencari id berdasarkan username, return idx, -1 jika tidak ditemukan
int findIdByUsername(const char *username);

// Menghasilkan username dari USERS berdasarkan masukan index
char *findUsernameById(int id);

//  Mencari index user di USERS berdasarkan user_id
int findUserIndexById(const char *userId);

// Men-generate USERID yang baru
void generateUserID(char *id, int num);

// Register user baru
void registerUser();

// User login
void loginUser();

// User logout
void logoutUser();

#endif