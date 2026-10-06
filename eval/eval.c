#include "eval.h"

#include <stdlib.h>
#include <errno.h>

#include "../color/color.h"
#include "../error/error.h"

sml_eval eval_new() {
    sml_eval e = {};
    return e;
}

float eval_evaluate_statement(sml_ast_node *n) {
    switch(n->type) {
        case N_NUMBER: return n->data.number.number.number;                                         // sruct node -> union data -> struct number -> token number -> float number (sorry)
        case N_UNARY: {
            sml_token_type op_type = n->data.unary.op.type;
            float factor = eval_evaluate_statement(n->data.unary.factor);
            switch(op_type) {
                case T_PLUS:  return +factor; break;
                case T_MINUS: return -factor; break;
                default: sml_error(0, 0, "<Runtime error>", 2); break;
            }
            break;
        }
        case N_BINARY: {
            sml_token_type op_type = n->data.binary.op.type;
            float left = eval_evaluate_statement(n->data.binary.left);
            float right = eval_evaluate_statement(n->data.binary.right);
            switch(op_type) {
                case T_PLUS:  return left + right; break;
                case T_MINUS: return left - right; break;
                case T_STAR:  return left * right; break;
                case T_SLASH: return left / right; break;
                default: sml_error(0, 0, "<Runtime error>", 2); break;
            }

            break;
        }
    }

    sml_error(0, 0, "<Runtime error>", 2);
    return 0;                                                                                       // Not needed since we panic with sml_error but futureproof (kinda)
}

void eval_evaluate(sml_eval *e, sml_ast_node **statements, size_t count) {
    e->capacity = 16;
    e->results = malloc(sizeof(float) * 16);
    if (!e->results) { sml_error(0, 0, FG_RED "<Fatal error> Memory allocation failed", ENOMEM); };

    for (size_t i = 0; i < count; i++) {
        if (i > e->count) {
            e->capacity *= 2;
            e->results = realloc(e->results, sizeof(float) * e->capacity);
            if (!e->results) { sml_error(0, 0, FG_RED "<Fatal error> Memory allocation failed", ENOMEM); };
        }

        e->results[i] = eval_evaluate_statement(statements[i]);

        e->count++;
    }
}
