#ifndef __ENV_H__
#define __ENV_H__

#include "reader.h"

struct env_entry{
    char name[32];
    struct AST_Node *(*func)(struct AST_Node *x, struct AST_Node *y);
};

struct env_t{
    struct env_t *symmap[64];
    int alloc_map[64];
    int env_cnt;
};

#endif
