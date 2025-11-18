#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <stdio.h>
#include "../../adt/header/MesinKata.h"
#include "../../adt/header/ADTSederhana.h"
#include "load-config.h"
#include "helper.h"
#include "global-data.h"

// Membentuk path
void buildPath(char *out, const char *folder, const char *file);

// Inisialisasi
void initialize();

#endif