#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "types.h"
#include "reader.h"

struct Gen_type_t *make_integer(int x){
    struct Gen_type_t *ret = (struct Gen_type_t*)malloc(sizeof(struct Gen_type_t));
    ret->value.integer = x;
    ret->type = TYPE_INTEGER;
    return ret;
}

struct Gen_type_t *make_string(char *str){
    int str_len = strlen(str);
    struct Gen_type_t *ret = (struct Gen_type_t*)malloc(sizeof(struct Gen_type_t));
    ret->value.str = (char*)malloc(str_len);
    strcpy(ret->value.str, str);
    ret->type = TYPE_STRING;
    return ret;
}

struct Gen_type_t *make_symbol(char *str){
    int str_len = strlen(str);
    struct Gen_type_t *ret = (struct Gen_type_t*)malloc(sizeof(struct Gen_type_t));
    ret->value.symbol = (char*)malloc(str_len);
    strcpy(ret->value.str, str);
    ret->type = TYPE_SYMBOL;
    return ret;
}

struct Gen_type_t *make_operator(char *x){
    int str_len = strlen(x);
    struct Gen_type_t *ret = (struct Gen_type_t*)malloc(sizeof(struct Gen_type_t));
    ret->value.op = (char*)malloc(str_len);
    strcpy(ret->value.op, x);
    ret->type = TYPE_OPERATOR;
    return ret;
}

void destroy_gentype(struct Gen_type_t *gentype){

    switch(gentype->type){
        case TYPE_SYMBOL:   
            free(gentype->value.symbol);
            free(gentype);
            break;
        case TYPE_STRING:
            free(gentype->value.str);
            free(gentype);
            break;
        default:
            free(gentype);
            break;
    }

    return;
}

struct Gen_type_t *token2gen(struct Token *token){
    struct Gen_type_t *ret = NULL;

    switch (token->type){
        case TOKEN_NUMBER:
            ret = make_integer(atoi(token->tok));
            ret->type = TYPE_INTEGER;
            break;

        case TOKEN_STRING:
            ret = make_string(token->tok);
            ret->type = TYPE_STRING;
            break;

        case TOKEN_REGULAR:
            ret = make_symbol(token->tok);
            ret->type = TYPE_SYMBOL;
            break;

        case TOKEN_SPECIAL:
            ret = make_symbol(token->tok);
            ret->type = TYPE_SYMBOL;
            break;

        case TOKEN_OPERATOR:
            ret = make_operator(token->tok);
            ret->type = TYPE_OPERATOR;
            break;

        default:
            fprintf(stderr, "Failed to convert token to Gen type: Unsupported type of token\n");
            break;
    }

    return ret;
}

struct Token *gen2token(struct Gen_type_t *gentype){
    struct Token *tok = NULL;

    switch (gentype->type){
        case TYPE_INTEGER:
            tok = (struct Token *)malloc(sizeof(struct Token));
            snprintf(tok->tok, 128, "%d", gentype->value.integer);
            tok->type = TOKEN_NUMBER;
            break;

        case TYPE_STRING:
            tok = (struct Token *)malloc(sizeof(struct Token));
            strcpy(tok->tok, gentype->value.str);
            tok->type = TOKEN_STRING;
            break;

        case TYPE_SYMBOL:
            tok = (struct Token *)malloc(sizeof(struct Token));
            strcpy(tok->tok, gentype->value.symbol);
            tok->type = TOKEN_REGULAR;
            break;

        default:
            fprintf(stderr, "This type is not ready to be converted into a token\n");
            break;
    }

    return tok;
}

/*
int main(void){
    struct Gen_type_t *var = make_integer(4);

    printf("%d", var->value.integer);
    destroy_gentype(var);
}
*/
