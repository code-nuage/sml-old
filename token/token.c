#include "token.h"

#include <stdio.h>
#include <stdlib.h>

#include "../color/color.h"

#include "../error/error.h"

char *token_display(sml_token t) {
    char *buffer = malloc(64);
    char *type;
    switch(t.type) {
        case T_NUMBER: 
            sprintf(buffer, BOLD_BLUE "%d;%d\t" RESET "| " FG_YELLOW "NUMBER" RESET " %f", t.line, t.column, t.number);
            return buffer;

        case T_LPAREN: type = "("; break;
        case T_RPAREN: type = ")"; break;

        case T_PLUS:   type = "+"; break;
        case T_MINUS:  type = "-"; break;
        case T_STAR:   type = "*"; break;
        case T_SLASH:  type = "/"; break;

        case T_SEMICOLON:  type = ";"; break;

        case T_EOF:    type = BOLD_BLACK "EOF"; break;

        case T_UNKNOWN: type = BOLD_RED "Unknown"; break;

        default:
            type = "Unknown token type";
            sml_error(t.line, t.column, "Unknown token type", 1);
    }

    sprintf(buffer, BOLD_BLUE "%d;%d\t" RESET "| " FG_YELLOW "(" RESET "%s" FG_YELLOW ")" RESET, t.line, t.column, type);
    return buffer;
}

void token_print(sml_token t) {
    char *token_string = token_display(t);
    printf("%s\n", token_string);
    free(token_string);
}
