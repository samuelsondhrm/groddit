#ifndef __MESIN_KATA_H__
#define __MESIN_KATA_H__

#include "boolean.h"
#include "MesinKarakter.h"

#define NMax 50 // nanti bisa atur atur di sini lagi buat max kalimat
#define BLANK ' '

typedef struct 
{
    char TabWord[NMax];
    int Length;
} Word;

extern boolean EndWord;
extern Word currentWord;

void IgnoreBlanks();
/**
 * Mengabaikan satu atau beberapa BLANK
 * I.S. : currentChar sembarang
 * F.S. : currentChar != BLANK atau currentChar = MARK
 */

void STARTWORD();
/**
 * I.S. : currentChar sembarang
 * F.S  : EndWord = true, can currenetChar = MARK;
 *        atau EndWord = false, currentWord adalah kata yang sudah diakuasisi.
 *        currentChar karakter pertama sesudah karakter terakhir kata
 */

void ADVWORD();
/**
 * I.S. : currentChar adalah karakter pertama kata yang akan diakuisisi
 * F.S. : currentWord adalah kata terakhir yang sudah diakuisisi,
 *        currentChar adalah karakter pertama dari kata berikutnya, mungin MARK 
 *        Jika currentChar = MARK, EndWord = true.
 * Proses : Akuisisi kata mennggunakan procedure CopyWord()
*/

void CopyWord();
/**
 * Mengakuisisi kata, menyimpan dalam currentWord
 * I.S. :   currentChar adadah kata terakhir yang sudah diakuisii.
 * F.S. :   currentChar adalah karakter pertama dari kata berikutnya, mungkin MARK
 *          currentChar = BLANK atau currentChar = MARK;
 *          currentChar adalah karakter sesudah karakter terakhir yang diakuisisi.
 *          Jika panjang kata melebihi NMax, maka sisa kata dipotong.
 */

#endif