#include "env.h"

static int alloc_env_slot(struct env_t *env){
    for(int i=0; i<64; i++){
        if(!env->alloc_map[i]){
            env->alloc_map[i] = 1;
            env->symmap[i] = (struct env_entry*)malloc(sizeof(struct env_ent));
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

    ret_env->map_cnt = 0;
    return ret_env;
}

void add_env_entry(struct env_t *env, char *name, struct AST_Node *(*func)){
    
    int alloc_pt = alloc_env_slot(env);
    strcpy(env->symmap[alloc_pt]->name, name);
    env->symmap[alloc_pt]->func = func;

    return;
}

void remove_env_entry(struct env_t *env, int idx){
    free_env_slot(env, idx);
    return;
}

struct env_ent *lookup_env(struct env_t *env, char *name){
    struct env_t *env_ent;

    for(int i=0; i<64; i++){
        env_ent = env->symmap[i];
        if(strcmp(name, env_ent.name) == 0){
            return env_ent;
        }
    }

    return NULL;
}
