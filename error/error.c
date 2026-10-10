#include "error.h"

#include "../color/color.h"

#include <stdio.h>
#include <stdlib.h>

int sml_error(int line, int col, char *text, int code) {
    if (line > 0) {
        printf(BOLD_RED "%d;%d\t" RESET "| %s.\n", line, col, text);
        exit(code);
        return code;
    }

    printf(BOLD_RED "Error\t" RESET "| %s.\n", text);
    exit(code);
    return code;
}
