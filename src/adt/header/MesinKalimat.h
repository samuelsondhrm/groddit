#ifndef __MESINKALIMAT_H__
#define __MESINKALIMAT_H__

#include "boolean.h"
#include "MesinKarakter.h"

#define NMax 999
#define ENDLINE '\n'
#define BLANK ' '

/* Struktur untuk menampung satu kalimat */
typedef struct {
    char TabSentence[NMax];  /* container penyimpan kalimat, indeks [0..NMax-1] */
    int Length;              /* panjang kalimat yang dipakai */
} Sentence;

/* State Mesin Kalimat */
extern boolean EndSentence;
extern Sentence currentSentence;

/* Mengabaikan BLANK dan newline */
void IgnoreBlanks();
void IgnoreNewline();

/* Inisialisasi mesin kalimat */
void STARTSENTENCE();
/* I.S. : currentChar sembarang
   F.S. : EndSentence = true jika MARK, 
         atau EndSentence = false dan currentSentence berisi kalimat pertama */

void ADVSENTENCE();
/* I.S. : currentChar adalah karakter pertama kalimat berikutnya
   F.S. : currentSentence berisi kalimat terakhir yang dibaca
         currentChar menunjuk karakter pertama kalimat berikutnya
         EndSentence = true jika MARK */

void CopySentence();
/* Mengakuisisi karakter sampai BLANK, newline atau MARK, simpan ke currentSentence
   Jika panjang > NMax, sisa karakter dipotong */

#endif
