#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "error/error.h"
#include "color/color.h"
#include "eval/eval.h"
#include "token/token.h"
#include "lexer/lexer.h"
#include "node/node.h"
#include "parser/parser.h"
#include "eval/eval.h"

char *read_file(char *path) {
    FILE *file = fopen(path, "r");

    if (file == NULL) sml_error(0, 0, "Unable to open file", ENOENT);

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    char *content = malloc(size + 1);

    if (content == NULL) {
        fclose(file);
        sml_error(0, 0, "Unable to alloc content string", ENOMEM);
    }

    fread(content, 1, size, file);
    content[size] = '\0';

    fclose(file);
    return content;
}

void run(char *content) {
        sml_lexer l = lexer_new();
        lexer_lex(&l, content);

        for (size_t i = 0; i < l.count; i++) {
            token_print(l.tokens[i]);
        }

        sml_ast_parser p = parser_new();
        parser_parse(&p, l.tokens);

        for (size_t i = 0; i < p.count; i++) {
            node_print(p.statements[i]);
        }

        sml_eval e = eval_new();
        eval_evaluate(&e, p.statements, p.count);

        for (size_t i = 0; i < e.count; i++) {
            printf("%f\n", e.results[i]);
        }
};

int main(int argc, char **argv) {
    printf("sml-dev # somemicrolanguage\nMIT License Copyright 2026 code-nuage\n\n");
    if (argc == 1) {
        char prompt[256];

        while (1) {
            printf(" > ");
            fgets(prompt, sizeof(prompt), stdin);
            prompt[strcspn(prompt, "\n")] = '\0';

            run(prompt);
        }

        return 0;
    } else if (argc == 2) {
        char *path = argv[1];
        char *content = read_file(path);

        run(content);

        free(content);
    } else {
        return sml_error(0, 0, FG_RED "<Usage error> " RESET "Usage is 'sml [file]'", 64);
    }

    return 0;
}
