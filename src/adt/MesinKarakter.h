#ifndef __MESIN_KARAKTER_H_
#define __MESIN_KARAKTER_H_

#include "boolean.h"

#define MARK ';'
extern char currentChar;
extern boolean EOP;

void START();
/**
 * Mesin siap dioperasikan. Pita disiapkan untuk dibaca.
 * Karakter pertama yang ada pada pita posisinya adalah pada jendela.
 * I.S. : sembarang
 * F.S  : currentChar adalah karakter pertama pada pita
 *        Jika currentChar != MARK maka EOP akan padam (false)
 *        Jika currentChar = MARK maka EOP akan menyala (true)
 */

void ADV();
/**
 * Pita dimajukan satu karakter.
 * I.S. : Karakter pada jendela = currentChar, currentCHar != MARK
 * F.S. : currentChar adalah karakter berikutnya dari currentChar yang lama,
 *        currentChar mungkin = MARK
 *        Jika currentChar = MARK maka EOP akan menyala (true)
 */

#endif