#ifndef mango_lexer_h
#define mango_lexer_h


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct 
{
	char *inp;
	size_t size;
} Buffer;

typedef enum
{
	TOKEN_LET,    // let  
	TOKEN_WHITESPACE, // ' '
	TOKEN_ERROR,  
	TOKEN_EOF,    // EOF
	TOKEN_PLUS,   // +
	TOKEN_MINUS,  // -
	TOKEN_EQ,    // =
	TOKEN_STAR,  // *
	TOKEN_SLASH, // /
	TOKEN_DOT, 	 // .
	TOKEN_MOD,	 // %
	TOKEN_NAME,	 
	TOKEN_NUMBER, // 0-9
	TOKEN_LINE,  // \n
	TOKEN_STRING, // "[a-z || A-Z]"
	TOKEN_LEFT_PAREN, // (
	TOKEN_RIGHT_PAREN, // )
	TOKEN_SEMICOLON		// ;
} TokenType;

typedef struct Token_t
{
	TokenType type;
	int start;
	int end;
	int lineNum;
	
	struct Token_t *previous;
	struct Token_t *next;
} Token;

typedef struct 
{
	Buffer *source;
	int start;
	int pos;
	
	Token *head;
	Token *tail;
} Lexer;

Buffer* newBuffer(size_t size);
void freeBuffer(Buffer* buf);

Token* newToken(TokenType type, int start, int end);
void printToken(Buffer *buf, Token *tok);
Token* unlinkToken(Token *tok);

Token* tokenize(Buffer *src);

static void readName(Lexer *lexer);
static void readNum(Lexer *lexer);
static void readString(Lexer *lexer);
static void readWhitespace(Lexer *lexer);
static bool isKeyword(Lexer *lexer, char *keyword);
static bool isName(char c);
static bool isDigit(char c);
static char next(Lexer *lexer);
static char peek(Lexer *lexer);
static void addToken(Lexer *lexer, TokenType type);


#endif