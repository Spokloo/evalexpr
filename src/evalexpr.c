#include "evalexpr.h"

#include "evaluate.h"
#include "lexer.h"
#include "shunting_yard.h"

static char *get_input(void)
{
    char *input = calloc(1, sizeof(char));
    int c = getchar();
    int i = 0;

    while (c != EOF)
    {
        input[i] = c;
        i++;
        input = realloc(input, i + sizeof(char));
        if (!input)
            return NULL;

        c = getchar();
    }

    if (i == 0)
    {
        input = realloc(input, 1 + sizeof(char));
        if (!input)
            return NULL;

        input[0] = '\n';
        i++;
    }

    if (i > 0 && input[i - 1] != '\n')
    {
        input = realloc(input, i + 1 + sizeof(char));
        if (!input)
            return NULL;

        input[i] = '\n';
        i++;
    }

    input[i] = '\0';
    return input;
}

int main(int argc, char **argv)
{
    if (argc != 1 && argc != 2)
    {
        fprintf(stderr, "Invalid argument\n");
        exit(4);
    }

    char *input = get_input();

    if (argc == 2)
    {
        if (strcmp(argv[1], "-rpn") != 0)
        {
            fprintf(stderr, "Invalid argument\n");
            free(input);
            exit(4);
        }

        struct token **tokens = lexer(input, strlen(input), 1);

        int res = compute_rpn(tokens);
        printf("%d\n", res);

        free_tokens(tokens);
    }
    else
    {
        struct token **tokens = lexer(input, strlen(input), 0);
        struct token **tokens_rpn = shunting_yard(tokens);

        int res = compute_rpn(tokens_rpn);
        printf("%d\n", res);

        free_tokens(tokens);
    }

    free(input);
    exit(0);
}
