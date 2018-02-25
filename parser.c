#include "token.h"
#include "parser.h"
#include "lexer.h"
#include "node.h"

static void *factor(Parser *parser);
static void *term(Parser *parser);
static void *expr(Parser *parser);

static void *factor(Parser *parser)
{
    Lexer *lexer = parser->lexer;
    token_t current_token = lexer->current_token;
    NumNode_t *node = NULL;

    if (current_token.type == INTEGER) {
        match(lexer, INTEGER);
        node = (NumNode_t *) malloc(sizeof(NumNode_t));
        node->operand = current_token;
        return node;
    } else if (current_token.type == LPAREN) {
        match(lexer, LPAREN);
        node = expr(parser);
        match(lexer, RPAREN);
        return node;
    }
    
    printf("Unavailable expression!\n");
    exit(0);
}

static void *term(Parser *parser)
{
    void *node = factor(parser);
    Lexer *lexer = parser->lexer;

    while (lexer->current_token.type == MULTIPLY || \
           lexer->current_token.type == DIVIDE) {
        
        token_t current_token = lexer->current_token;
        
        if (current_token.type == MULTIPLY) {
            match(lexer, MULTIPLY);
        }

        if (current_token.type == DIVIDE) {
            match(lexer, DIVIDE);
        }
        
        BinNode_t *space = (BinNode_t *) malloc(sizeof(BinNode_t));
        space->op = current_token;
        space->left = node;
        space->right = factor(parser);

        node = space;
    }
    return node;
}

static void *expr(Parser *parser)
{
    Lexer *lexer = parser->lexer;
    void *node = term(parser);

    while (lexer->current_token.type != NONE && \
           lexer->current_token.type != RPAREN) {
        
        token_t current_token = lexer->current_token;
        
        switch(current_token.type) {
            case PLUS:
                match(lexer, PLUS);
                break;
            case MINUS:
                match(lexer, MINUS);
                break;
            default:
                break;
        }

        BinNode_t *space = (BinNode_t *) malloc(sizeof(BinNode_t));
        space->op = current_token;
        space->left = node;
        space->right = factor(parser);

        node = space;
    }
    return node;
}

Parser *parser_init(const char *text)
{
    Parser *parser = (Parser *) malloc(sizeof(Parser));
    if (!parser) {
        printf("parser.c: Not available space to allocate\n");
        exit(0);
    }

    parser->lexer = lexer_init(text);

    return parser;
}

void *parse(Parser *parser)
{
    return expr(parser);
}