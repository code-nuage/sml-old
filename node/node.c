#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../color/color.h"

char *node_display(sml_ast_node *n, int indent) {
    char *buf = NULL;

    switch (n->type) {
        case N_NUMBER: {
            char *tok = token_display(n->data.number.number);
            size_t len = indent + strlen(tok) + 32;
            buf = malloc(len);
            snprintf(buf, len, "%*s" FG_YELLOW "NUMBER " RESET "%s\n",
                     indent, "", tok);
            free(tok);
            break;
        }

        case N_UNARY: {
            char *tok    = token_display(n->data.unary.op);
            char *factor = node_display(n->data.unary.factor, indent + 2);
            size_t len = indent + strlen(tok) + strlen(factor) + 32;
            buf = malloc(len);
            snprintf(buf, len, "%*s" FG_YELLOW "UNARY " RESET "%s\n%s",
                     indent, "", tok, factor);
            free(tok);
            free(factor);
            break;
        }

        case N_BINARY: {
            char *tok   = token_display(n->data.binary.op);
            char *left  = node_display(n->data.binary.left,  indent + 2);
            char *right = node_display(n->data.binary.right, indent + 2);
            size_t len = indent + strlen(tok) + strlen(left) + strlen(right) + 32;
            buf = malloc(len);
            snprintf(buf, len, "%*s" FG_YELLOW "BINARY " RESET "%s\n%s%s",
                     indent, "", tok, left, right);
            free(tok);
            free(left);
            free(right);
            break;
        }
    }

    return buf;
}

void node_print(sml_ast_node *n) {
    char *s = node_display(n, 0);
    printf("%s", s);
    free(s);
}
