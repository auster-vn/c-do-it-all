#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hex_to_text.h"

void hex_to_text(const char *hex, char *output) {
    int len = strlen(hex);
    if (len % 2 != 0) {
        strcpy(output, "ERROR");
        return;
    }

    for (int i = 0; i < len; i += 2) {
        sscanf(hex + i, "%2hhx", &output[i / 2]);
    }
    output[len / 2] = '\0';
}

