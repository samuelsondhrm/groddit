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
#include "../Security/Security.h"

// Membuat folder jika folder belum ada
int createFolderIfNotExists(const char *folder);

// Melakukan save data
void performSave(const char *folder);

// Perintah save yang akan dipanggil di main
void commandSave();

// CSV Builder
char *serialize_users(size_t *out_len);
char *serialize_comments(size_t *out_len);
char *serialize_posts(size_t *out_len);
char *serialize_subgroddits(size_t *out_len);
char *serialize_socials(size_t *out_len);
char *serialize_votings(size_t *out_len);

#endif