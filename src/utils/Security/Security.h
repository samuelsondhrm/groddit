#ifndef SECURITY_H
#define SECURITY_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include "../../adt/MesinKata/MesinKata.h"
#include "../Helper/Helper.h"
#include "../GlobalData/GlobalData.h"

extern char global_security_conf_path[256];

#define ENC_MAGIC "GREN" // header magic 4 byte untuk file terenkripsi
#define ENC_VERSION 1    // versi format enkripsi

// Inisialisasi modul dan memuat security.conf
boolean security_init(const char *config_path);

// Akses status fitur
boolean password_hashing_enabled(void); // cek apakah hashing password aktif
boolean file_encryption_enabled(void);  // cek apakah enkripsi file aktif

// Mengaktifkan fitur (true jika berhasil)
boolean enable_password_hashing(void);   // aktifkan hashing password dan migrasi data
boolean set_file_encryption(boolean on); // aktif/nonaktif enkripsi file

// Kontrol seed RNG sederhana
void security_set_seed(uint32_t seed); // set nilai seed
uint32_t security_get_seed(void);      // ambil nilai seed

// Hash password FNV-1a 32-bit
uint32_t hash_password(const char *pw); // menghasilkan hash dari password

// Enkripsi XOR stream
void reset_keystream(void);                // reset state keystream sebelum enkripsi file
void crypt_buffer(uint8_t *buf, size_t n); // enkripsi/dekripsi buffer in-place

// IO file terenkripsi
uint8_t *read_encrypted_file(const char *path, size_t *out_len, boolean *was_encrypted);
// baca file, deteksi enkripsi, dan kembalikan buffer

boolean write_encrypted_file(const char *path, const uint8_t *buf, size_t len);
// tulis payload terenkripsi dengan header

boolean save_security_conf(const char *path);
// simpan konfigurasi security.conf

void handleSecurityCommand();
// handler perintah security dari CLI

void migrate_passwords_in_memory(void);
// migrasi password plaintext ke hash di memori

#endif