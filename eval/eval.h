#ifndef EVAL_H
#define EVAL_H

#include <stdlib.h>

#include "../node/node.h"

typedef struct sml_eval {
    sml_ast_node **statements;

    float *results;
    size_t capacity;
    size_t count;
} sml_eval;

sml_eval eval_new();

float eval_evaluate_statement(sml_ast_node *n);

void eval_evaluate(sml_eval *e, sml_ast_node **statements, size_t count);

#endif // EVAL_H
