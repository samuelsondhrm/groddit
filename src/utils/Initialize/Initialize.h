#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <stdio.h>
#include "../../adt/MesinKata/MesinKata.h"
#include "../../adt/ADTSederhana/ADTSederhana.h"
#include "../Load/Load.h"
#include "../Helper/Helper.h"
#include "../GlobalData/GlobalData.h"
#include "../User/User.h"

// Membentuk path
void buildPath(char *out, const char *folder, const char *file);

// Validasi folder konfigurasi
int isFolderValid(const char *folder);

// Inisialisasi
void initialize();

#endif