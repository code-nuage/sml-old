#ifndef NODE_H
#define NODE_H

#include "../token/token.h"

typedef struct sml_ast_node sml_ast_node;

typedef enum sml_ast_node_type {
    N_BINARY,
    N_UNARY,
    N_NUMBER
} sml_ast_node_type;

typedef struct sml_ast_node {
    sml_ast_node_type type;

    union {
        struct binary { sml_token op; sml_ast_node *left; sml_ast_node *right;} binary;
        struct unary { sml_token op; sml_ast_node *factor; } unary;
        struct number { sml_token number; } number;
    } data;
} sml_ast_node;

char *node_display(sml_ast_node *n, int indent);
void node_print(sml_ast_node *n);

#endif // NODE_H
