#include "lexer.h"
#include <string.h>

void lexer_init(Lexer *lexer, const char *source)
{
  lexer->source = source;
  lexer->pos = 0;
  lexer->line = 1;
}

Token lexer_next_token(Lexer *lexer)
{
  Token token;
  token.type = TOK_EOF; // placeholder
  token.lexeme[0] = '\0';
  token.line = lexer->line;
  return token;
}
