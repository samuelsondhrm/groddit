#ifndef SECURITY_H
#define SECURITY_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include "../../adt/MesinKata/MesinKata.h"
#include "../Helper/Helper.h"
#include "../GlobalData/GlobalData.h"

extern char global_security_conf_path[256];

#define ENC_MAGIC "GREN" // 4 bytes magic header
#define ENC_VERSION 1

// Initialize module (loads security.conf). Call once at program startup.
boolean security_init(const char *config_path);

// Flags accessors
boolean password_hashing_enabled(void);
boolean file_encryption_enabled(void);

// Toggle functions (return true if success)
boolean enable_password_hashing(void); // performs migration; irreversible
boolean set_file_encryption(boolean on);

// Seed control
void security_set_seed(uint32_t seed);
uint32_t security_get_seed(void);

// Hash function for passwords (FNV-1a 32)
uint32_t hash_password(const char *pw);

// Encryption / Decryption primitives
// crypt_buffer applies XOR keystream in-place; call reset_keystream() before each file.
void reset_keystream(void);
void crypt_buffer(uint8_t *buf, size_t n);

// Helpers for file IO with magic header
// write_encrypted_file writes magic+version then encrypted payload
// read_encrypted_file reads file, returns dynamically allocated buffer and sets out_n.
// If file not encrypted, returns raw content and sets *is_encrypted=false
uint8_t *read_encrypted_file(const char *path, size_t *out_len, boolean *was_encrypted);

boolean write_encrypted_file(const char *path, const uint8_t *buf, size_t len);

boolean save_security_conf(const char *path);

void handleSecurityCommand();

void migrate_passwords_in_memory(void);

#endif // SECURITY_H
