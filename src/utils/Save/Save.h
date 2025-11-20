#ifndef SAVECOMMAND_H
#define SAVECOMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../../adt/MesinKata/MesinKata.h"
#include "../../adt/ADTSederhana/ADTSederhana.h"
#include "../Helper/Helper.h"
#include "../GlobalData/GlobalData.h"
#include "../User/User.h"
#include "../Initialize/Initialize.h"

// Membuat folder jika folder belum ada
int createFolderIfNotExists(const char *folder);

// Melakukan save data
void performSave(const char *folder);

// Perintah save yang akan dipanggil di main 
void commandSave();



#endif
