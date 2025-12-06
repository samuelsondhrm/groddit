#ifndef ADVANCED_SEARCH_H
#define ADVANCED_SEARCH_H

/* ============================================================================
 *                              ADVANCED SEARCH
 * ============================================================================
 * Fitur pencarian berbasis Trie (prefix tree) untuk:
 *  - username User
 *  - judul Post
 *  - nama SubGroddit
 *
 * Semua pencarian prefix bersifat case-insensitive dan menggunakan ADT Trie
 * dari src/adt/Trie.
 * ==========================================================================*/

/**
 * @brief Menangani perintah SEARCH_USER <prefix>;
 *
 * Membangun Trie dari seluruh username di USERS, lalu menampilkan semua user
 * yang memiliki prefix username sesuai argumen.
 */
void commandSearchUser();

/**
 * @brief Menangani perintah SEARCH_POST <prefix>;
 *
 * Membangun Trie dari judul setiap Post, lalu menampilkan daftar post yang
 * judulnya diawali prefix tersebut dalam format:
 *   <no>. [<nama_subgroddit>] <judul_post>
 */
void commandSearchPost();

/**
 * @brief Menangani perintah SEARCH_SUBGRODDIT <prefix>;
 *
 * Membangun Trie dari nama SubGroddit, lalu menampilkan seluruh SubGroddit
 * yang namanya diawali prefix yang diberikan.
 */
void commandSearchSubgroddit();

#endif
