#include "minishell.h"

int ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int is_operator(char *str) {
    if (!strncmp(str, "<<", 2) || !strncmp(str, ">>", 2))
        return 2;
    if (strchr("|<>", str[0]))
        return 1;
    return 0;
}

int is_quote(char c) {
    return (c == '"' || c == '\'');
}

t_token_type get_token_type(char *value)
{
    if (!strcmp(value, "|")) return PIPE;
    if (!strcmp(value, "<")) return REDIR_IN;
    if (!strcmp(value, ">")) return REDIR_OUT;
    if (!strcmp(value, ">>")) return APPEND;
    if (!strcmp(value, "<<")) return HEREDOC;
    return WORD;
}

// Tokenize the input string
t_token *tokenize(char *input)
{
    t_token *tokens;
    char *token;
    int operator_len;
    t_token_type type;
    char quote_char;

    tokens = NULL;
    while (*input)
    {
        while (*input && ft_isspace(*input))
            input++;
        if (!*input)
            break;
        token = input;
        operator_len = is_operator(input);
        if (operator_len)
        {
            input += operator_len;
            type = get_token_type(ft_substr(token, 0, operator_len));
        }
        else if (is_quote(*input))
        {
            quote_char = *input++;
            while (*input && *input != quote_char)
                input++;
            if (*input == quote_char)
                input++;
            type = WORD;
        }
        else
        {
            while (*input && !ft_isspace(*input) && !is_operator(input))
                input++;
            type = WORD;
        }
        add_token(&tokens, new_token(ft_substr(token, 0, input - token), type));
    }
    return tokens;
}

void print_tokens(t_token *tokens) {
    t_token *tmp = tokens;
    while (tmp) {
        printf("Token: %-10s Type: %d\n", tmp->value, tmp->type);
        tmp = tmp->next;
    }
}
