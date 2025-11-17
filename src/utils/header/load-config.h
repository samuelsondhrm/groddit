/* adt/load-config.c */
#include <stdio.h>
#include <stdlib.h>
#include "../../adt/header/adt-sederhana.h"
#include "../../utils/header/helper.h"

// Load
int loadComments(const char *filepath, Comment **commentPtr, int *capacityPtr);