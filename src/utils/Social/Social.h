#ifndef SOCIAL_H
#define SOCIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../adt/Boolean/Boolean.h"
#include "../../adt/Graph/Graph.h"
#include "../GlobalData/GlobalData.h"
#include "../User/User.h"

/* ============================================================================
 *                          Fitur FOLLOW / UNFOLLOW
 * ============================================================================
 */

/**
 * Melakukan FOLLOW dari current user ke user dengan username target.
 *
 * Validasi:
 *  - Jika belum login → tampilkan pesan error.
 *  - Jika user target tidak ditemukan → pesan error.
 *  - Jika target == current user → pesan error.
 *  - Jika sudah follow → pesan info.
 *
 * Jika berhasil, menambahkan edge (currentUserIdx -> targetIdx)
 * ke SOCIAL_GRAPH dan menampilkan pesan sukses.
 */
void socialFollowUser(const char *targetUsername);

/**
 * Melakukan UNFOLLOW dari current user ke user dengan username target.
 *
 * Validasi:
 *  - Jika belum login → pesan error.
 *  - Jika user target tidak ditemukan → pesan error.
 *  - Jika belum follow → pesan info (tidak melakukan apa-apa).
 *
 * Jika berhasil, menghapus edge (currentUserIdx -> targetIdx)
 * dari SOCIAL_GRAPH dan menampilkan pesan sukses.
 */
void socialUnfollowUser(const char *targetUsername);

/* ============================================================================
 *                          Fitur FOLLOWING / FOLLOWERS
 * ============================================================================
 */

/**
 * Menampilkan daftar user yang di-follow oleh user dengan username yang
 * diberikan.
 *
 * Jika username NULL atau string kosong, kamu bisa memilih (di command
 * handler) untuk memanggil ini dengan username current user.
 *
 * I.S.  username valid atau menunjuk ke user yang ada
 * F.S.  Menampilkan list "following" di stdout.
 */
void socialShowFollowing(const char *username);

/**
 * Menampilkan daftar user yang menjadi follower user dengan username yang
 * diberikan.
 *
 * Implementasi:
 *  - Cari index target
 *  - Scan seluruh vertex u, cek apakah ada edge (u -> target)
 *  - Print username dari setiap u yang menjadi follower.
 */
void socialShowFollowers(const char *username);

#endif /* SOCIAL_H */
