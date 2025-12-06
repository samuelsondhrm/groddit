#ifndef PROFIL_H
#define PROFIL_H

#include "../../adt/ADTSederhana/ADTSederhana.h"
#include "../../adt/Graph/Graph.h"
#include "../../adt/Boolean/Boolean.h"
#include "../GlobalData/GlobalData.h"
#include "../Helper/Helper.h"
#include "../User/User.h"
#include "../Kreativitas/Kreativitas.h"

// Menghitung karma user berdasarkan upvotes/downvotes posts dan comments
int computeUserKarma(int userIndex);

// Menampilkan profil lengkap untuk username tertentu.
void showUserProfile(const char *username);

// Helper untuk mendapatkan nama subgroddit berdasarkan ID
void getSubgrodditName(Word subId, char *out);

#endif
