#ifndef __ENV_H__
#define __ENV_H__

#include "reader.h"
#include "types.h"

struct env_entry{
    char name[32];
    struct Gen_type_t *(*func)(struct Gen_type_t **);
};

struct env_t{
    struct env_entry *symmap[64];
    int alloc_map[64];
    int env_cnt;
};
struct env_t *init_env(void);
void add_env_entry(struct env_t *env, char *name, struct Gen_type_t *(*func)(struct Gen_type_t **));
void remove_env_entry(struct env_t *env, int idx);
struct env_entry *lookup_env(struct env_t *env, char *name);


#endif
