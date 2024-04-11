#ifndef __TYPES_H__
#define __TYPES_H__

#include "reader.h"

enum Gen_types{
    TYPE_INTEGER,
    TYPE_STRING,
    TYPE_SYMBOL,  
    TYPE_OPERATOR
};

struct Gen_type_t{
    union Gen_typ_val{
        unsigned long integer;
        char *str;
        char *symbol;
        char *op;
    } value;
    enum Gen_types type;
};

struct Gen_type_t *make_integer(int x);
struct Gen_type_t *make_string(char *str);
struct Gen_type_t *make_symbol(char *str);
struct Gen_type_t *make_operator(char *x);
void destroy_gentype(struct Gen_type_t *gentype);
struct Gen_type_t *token2gen(struct Token *token);
struct Token *gen2token(struct Gen_type_t *gentype);


#endif
