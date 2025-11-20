#include "Subgroddit.h"

void createSubgroddit()
{
    printf("Masukkan nama subgroddit.\n");
    fflush(stdout);

    STARTWORD_INPUT();

    if (currentWord.Length == 0)
    {
        printf("Nama subgroddit tidak boleh kosong!\n");
        return;
    }

    char subgroddit_name[256];
    wordToString(subgroddit_name, currentWord);

    if (currentWord.Length < 2 || subgroddit_name[0] != 'r' || subgroddit_name[1] != '/')
    {
        printf("Nama Subgroddit HARUS UNIK dan diawali dengan \"r/\".\n");
        return;
    }

    if (findSubgrodditIndexByName(subgroddit_name) != -1)
    {
        printf("Nama Subgroddit HARUS UNIK dan diawali dengan \"r/\".\n");
        return;
    }

    SubGroddit newSubgroddit;
    initSubGroddit(&newSubgroddit);

    char id_str[256];
    sprintf(id_str, "S%03d", SUBGRODDIT_COUNT + 1);
    stringToWord(&newSubgroddit.subgroddit_id, id_str);

    stringToWord(&newSubgroddit.name, subgroddit_name);

    ListElement elem;
    elem.type = TYPE_SUBGRODDIT;
    elem.data.subgroddit = newSubgroddit;
    insertLastList(&SUBGRODDITS, elem);

    SUBGRODDIT_COUNT++;

    printf("Subgroddit %s berhasil dibuat!\n", subgroddit_name);
}
