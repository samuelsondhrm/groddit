#include <stdio.h>
#include <stdlib.h>
#include "../../adt/header/adt-sederhana.h"
#include "header/load-config.h"
#include "header/helper.h"

int loadComments(const char *filepath, Comment **commentPtr, int *capacityPtr)
{
    FILE *f = fopen(filepath, "r");
    if (!f) {
        printf("[DEBUG] fopen failed: %s\n", filepath);
        return -1;
    }

    char buffer[512];
    int count = 0;

    int capacity = *capacityPtr;
    Comment *comment = *commentPtr;

    if (!fgets(buffer, sizeof(buffer), f)) {
        fclose(f);
        return 0;
    }

    while (1)
    {
        if (!ensureCapacity((void**)&comment, &capacity, sizeof(Comment), count + 1)) {
            fclose(f);
            return -1;
        }
        *commentPtr = comment;
        *capacityPtr = capacity;

        initComment(&comment[count]);

        char f1[32], f2[32], f3[64], f4[32], f5[512], f6[32], f7[32];

        int end = readField(f1, sizeof(f1), f);
        if (end && f1[0] == '\0') {
            break;
        }
        readField(f2, sizeof(f2), f);
        readField(f3, sizeof(f3), f);
        readField(f4, sizeof(f4), f);
        readField(f5, sizeof(f5), f);
        readField(f6, sizeof(f6), f);
        readField(f7, sizeof(f7), f);

        comment[count].comment_id = toInt(f1);
        copyString(comment[count].post_id, f2, 100);
        copyString(comment[count].author_id, f3, 20);
        comment[count].parent_comment_id = toInt(f4);
        copyString(comment[count].content, f5, 200);
        comment[count].upvotes = toInt(f6);
        comment[count].downvotes = toInt(f7);

        count++;
    }

    fclose(f);

    return count;
}
