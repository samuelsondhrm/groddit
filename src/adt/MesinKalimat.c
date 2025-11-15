#include <stdio.h>
#include "header/MesinKalimat.h"

/* State global */
boolean EndSentence;
Sentence currentSentence;

/* Mengabaikan BLANK (spasi) */
void IgnoreBlanks() {
    while (currentChar == BLANK) {
        ADV();
    }
}

/* Mengabaikan newline (\n atau \r) */
void IgnoreNewline() {
    while (currentChar == '\n' || currentChar == '\r') {
        ADV();
    }
}

/* Memulai pembacaan kalimat */
void STARTSENTENCE() {
    START();          // mulai mesin karakter
    IgnoreBlanks();
    IgnoreNewline();
    if (currentChar == MARK) {
        EndSentence = true;
    } else {
        EndSentence = false;
        CopySentence();
    }
}

/* Melanjutkan ke kalimat berikutnya */
void ADVSENTENCE() {
    IgnoreBlanks();
    IgnoreNewline();
    if (currentChar == MARK) {
        EndSentence = true;
    } else {
        EndSentence = false;
        CopySentence();
        IgnoreBlanks();
        IgnoreNewline();
    }
}

/* Mengakuisisi satu kalimat penuh ke currentSentence */
void CopySentence() {
    currentSentence.Length = 0;
    while (currentChar != MARK && currentChar != ENDLINE) {
        if (currentSentence.Length < NMax) {
            currentSentence.TabSentence[currentSentence.Length++] = currentChar;
        }
        ADV();
    }
    // Akhiri kalimat dengan '\0' agar mudah dipakai
    if (currentSentence.Length < NMax)
        currentSentence.TabSentence[currentSentence.Length] = '\0';
    else
        currentSentence.TabSentence[NMax-1] = '\0';
}
