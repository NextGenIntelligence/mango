#include "lexer.h"

Buffer *newBuffer(size_t size)
{
	Buffer *buf = malloc(sizeof(*buf));
	buf->size = size;
	buf->inp = malloc(size);

	return buf;
}

void freeBuffer(Buffer *buf)
{
	free(buf->inp);
	free(buf);
}

Token* newToken(TokenType type, int start, int end)
{

	Token *t = malloc(sizeof(*t));
	t->type = type;
	t->start = start;
	t->end = end;
	t->previous = NULL;
	t->next = NULL;

	return t;
}

void printToken(Buffer *buf, Token *tok)
{
	for (int i = tok->start; i < tok->end; i++)
	{
		putchar(buf->inp[i]);
	}
}

Token* unlinkToken(Token *tok)
{
	if (tok->prev) tok->next->previous = tok->prev;
	if (tok->next) tok->previous->next = tok->next;

	tok->next = NULL;
	tok->previous = NULL;

	return tok;
}


Token *tokenize(Buffer *input)
{
	Lexer *l;
	l->source = input;
	l->start = 0;
	l->pos = 0;
	l->head = NULL;
	l->tail = NULL;

	while (peek(l) != '\0')
	{
		l->start = l->pos;

		char c = next(l);
		switch(c)
		{
			case '(': addToken(l, TOKEN_LEFT_PAREN); break;
			case ')': addToken(l, TOKEN_RIGHT_PAREN); break;
			case '%': addToken(l, TOKEN_MOD); break;
			case '.': addToken(l, TOKEN_DOT); break;
			case '/': addToken(l, TOKEN_SLASH); break;
			case '*': addToken(l, TOKEN_STAR); break;
			case '=': addToken(l, TOKEN_EQ); break;
			case '-': addToken(l, TOKEN_MINUS); break;
			case '+': addToken(l, TOKEN_PLUS); break;
			case ' ': readWhitespace(l); break;
			case '"': readString(l); break;
			case '\n': addToken(l, TOKEN_LINE); break;

			default:
				if (isName(c)) 
				{
					readName(l);
				}
				else if (isDigit(c))
				{
					readNumber(c);
				}
				else 
				{
					emitToken(l, TOKEN_ERROR);
				}
				break;
		}
	}

	lexer.start = lexer.pos;
	emitToken(l, TOKEN_EOF);

	return l->head;
}

static void readName(Lexer *l)
{
	while (isName(l) || isDigit(peek(l))) next(l);

	TokenType type = TOKEN_NAME;

	if(isKeyword(l, "let")) type = TOKEN_LET;

	addToken(l, type);
}

static void readNum(Lexer *l)
{
	while (isDigit(peek(l))) next(l);

	addToken(l, TOKEN_NUMBER);
}

static void readString(Lexer *l)
{
	while (next(l) != '"');

	addToken(l, TOKEN_STRING);
}

static void readWhitespace(Lexer *l)
{
	while (peek(l) == ' ') next(l);

	addToken(l, TOKEN_WHITESPACE);
}

static bool isName(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isDigit(char c)
{
	return (c >= '0' && c <= '9');
}

static bool isKeyword(Lexer *l, char *keyword)
{
	size_t length = l->pos - l->start;
	size_t keywordLength = strlen(keyword);
	return length == keywordLength && strncmp(l->source->inp + l->start, keyword, length) == 0;
}

static char next(Lexer *l)
{
	char c = peek(l);
	l->pos++;
	return c;
}

static char peek(Lexer *l)
{
	return l->source->inp[l->pos];
}

static char addToken(Lexer *l, TokenType type)
{
	Token *tok = newToken(type, l->start, l->pos);

	tok->prev = l->tail;
	tok->next = NULL;

	if (l->tail == NULL)
	{
		l->head = tok;
	}
	else
	{
		l->tail->next = tok;
	}

	l->tail = tok;
}
