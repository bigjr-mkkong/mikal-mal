#include "env.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//#define ENV_TEST

static int alloc_env_slot(struct env_t *env){
    for(int i=0; i<64; i++){
        if(!env->alloc_map[i]){
            env->alloc_map[i] = 1;
            env->symmap[i] = (struct env_entry*)malloc(sizeof(struct env_entry));
            env->env_cnt++;
            return i;
        }
    }

    fprintf(stderr, "Envioronment is full :(\n");
    return -1;
}

static void free_env_slot(struct env_t *env, int idx){
    if(idx >= 64){
        fprintf(stderr, "Failed to free env slot: idx Out of Bound\n");
        return;
    }

    if(env->alloc_map[idx] == 0){
        fprintf(stderr, "Failed to free env slot: slot not allocated yet\n");
        return ;
    }
    free(env->symmap[idx]);
    env->alloc_map[idx] = 0;
    env->env_cnt--;
    return ;
}

struct env_t *init_env(void){
    struct env_t *ret_env = (struct env_t*)malloc(sizeof(struct env_t));

    ret_env->env_cnt = 0;
    return ret_env;
}

void add_env_entry(struct env_t *env, char *name, struct Gen_type_t *(*func)(struct Gen_type_t **)){
    
    int alloc_pt = alloc_env_slot(env);
    strcpy(env->symmap[alloc_pt]->name, name);
    env->symmap[alloc_pt]->func = func;

    return;
}

void remove_env_entry(struct env_t *env, int idx){
    free_env_slot(env, idx);
    return;
}

struct env_entry *lookup_env(struct env_t *env, char *name){
    struct env_entry *env_ent;

    for(int i=0; i<64; i++){
        env_ent = env->symmap[i];
        if(strcmp(name, env_ent->name) == 0){
            return env_ent;
        }
    }

    return NULL;
}

#ifdef ENV_TEST

int main(void){
    struct env_t *meta_env = init_env();
    add_env_entry(meta_env, "+", &add);
    add_env_entry(meta_env, "-", &sub);

    struct env_entry *target = lookup_env(meta_env, "-");

    struct Gen_type_t *a = make_integer(10);
    struct Gen_type_t *b = make_integer(14);
    struct Gen_type_t *ans = target->func(a, b);

    printf("%d\n", ans->value.integer);
}

#endif
