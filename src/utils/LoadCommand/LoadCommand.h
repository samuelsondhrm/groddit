#ifndef LOADCOMMAND_H
#define LOADCOMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../adt/MesinKata/MesinKata.h"
#include "../../adt/ADTSederhana/ADTSederhana.h"
#include "../Load/Load.h"
#include "../Helper/Helper.h"
#include "../GlobalData/GlobalData.h"
#include "../User/User.h"
#include "../Initialize/Initialize.h"

// Melakukan load data
void performLoad(const char *folder);

// Perintah load yang dipanggil di main
void commandLoad();

#endif
