#include "lexer.h"
#include "minishell.h"

void print_tokens(t_token *tokens)
{
	while (tokens != NULL)
	{
		switch (tokens->type)
		{
			case TOKEN_WORD: printf("WORD: '%s'\n", tokens->value); break;
			case TOKEN_PIPE: printf("PIPE: '%s'\n", tokens->value); break;
			case TOKEN_REDIRECT_IN: printf("REDIRECT_IN: '%s'\n", tokens->value); break;
			case TOKEN_REDIRECT_OUT: printf("REDIRECT_OUT: '%s'\n", tokens->value); break;
			case TOKEN_APPEND: printf("APPEND: '%s'\n", tokens->value); break;
			case TOKEN_HEREDOC: printf("HEREDOC: '%s'\n", tokens->value); break;
			case TOKEN_AND: printf("AND: '%s'\n", tokens->value); break;
			case TOKEN_OR: printf("OR: '%s'\n", tokens->value); break;
			case TOKEN_LEFT_PAREN: printf("LEFT_PAREN: '%s'\n", tokens->value); break;
			case TOKEN_RIGHT_PAREN: printf("RIGHT_PAREN: '%s'\n", tokens->value); break;
			case TOKEN_EOF: printf("EOF\n"); break;
			case TOKEN_SEMICOLON: printf("SEMICOLON: '%s'\n", tokens->value); break;
			case TOKEN_WILDCARD: printf("WILDCARD: '%s'\n", tokens->value); break;
			case TOKEN_ERROR: printf("ERROR: '%s'\n", tokens->value); break;
			default: printf("UNKNOWN TOKEN: '%s'\n", tokens->value);
		}
		tokens = tokens->next;
	}
}