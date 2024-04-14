#include "stdio.h"
#include "stdlib.h"
#include "types.h"
#include "readline/readline.h"
#include "reader.h"
#include "env.h"
/*
 * TODO:
 *          implement multi-argument for add/sub/mul/div
 */

struct Gen_type_t *mikal_add(struct Gen_type_t **list_add){
    struct Gen_type_t *adder = NULL;
    long sum = 0;
    for(int i=0; list_add[i] != NULL; i++){
        adder = list_add[i];
        if(adder->type != TYPE_INTEGER){
            fprintf(stderr, "Failed to add: number is in incorrect type\n");
            return NULL;
        }
        sum += adder->value.integer;
    }

    struct Gen_type_t *result;
    result = make_integer(sum);

    return result;
}

struct Gen_type_t *mikal_sub(struct Gen_type_t **list_sub){
    struct Gen_type_t *sub1 = list_sub[0];
    struct Gen_type_t *sub2 = list_sub[1];

    if(sub1->type != TYPE_INTEGER){
        fprintf(stderr, "Failed to subtract: number 1 is in incorrect type\n");
        return NULL;
    }
    if(sub2->type != TYPE_INTEGER){
        fprintf(stderr, "Failed to subtract: number 2 is in incorrect type\n");
        return NULL;
    }

    struct Gen_type_t *result;
    result = make_integer(sub1->value.integer - sub2->value.integer);

    return result;
}

struct Gen_type_t *mikal_mul(struct Gen_type_t **list_mul){
    struct Gen_type_t *mul_num = NULL;
    long mul_result = 1;
    for(int i=0; list_mul[i] != NULL; i++){
        mul_num = list_mul[i];
        if(mul_num->type != TYPE_INTEGER){
            fprintf(stderr, "Failed to mul: number is in incorrect type\n");
            return NULL;
        }
        mul_result *= mul_num->value.integer;
    }

    struct Gen_type_t *result;
    result = make_integer(mul_result);

    return result;
}

struct Gen_type_t *mikal_div(struct Gen_type_t **list_div){
    struct Gen_type_t *div1 = list_div[0];
    struct Gen_type_t *div2 = list_div[1];

    if(div1->type != TYPE_INTEGER){
        fprintf(stderr, "Failed to divid: number 1 is in incorrect type\n");
        return NULL;
    }
    if(div2->type != TYPE_INTEGER){
        fprintf(stderr, "Failed to divid: number 2 is in incorrect type\n");
        return NULL;
    }

    if(div2->value.integer == 0){
        fprintf(stderr, "Failed to divid: number 2 is zero\n");
        return NULL;
    }

    struct Gen_type_t *result;
    result = make_integer(div1->value.integer / div2->value.integer);

    return result;
}

struct Gen_type_t *apply(struct Gen_type_t *op, struct Gen_type_t **args, struct env_t *env){
    struct env_entry *ent;
    struct Gen_type_t *ret = NULL;

    switch (op->type){
        case TYPE_OPERATOR:
            ent = lookup_env(env, op->value.op);
            if(ent == NULL){
                fprintf(stderr, "operator %s undefined\n", op->value.op);
            }
            ret = ent->func(args);
            break;

        default:
            fprintf(stderr, "operator type unsuppoerted by apply function\n");
            break;
    }

    return ret; 
}

struct Gen_type_t *eval(struct AST_Node *root, struct env_t *env){
    if(AST_Node_isleaf(root)){
        root->gen_val = token2gen(&(root->token));
        return root->gen_val;
    }else{
        struct Gen_type_t *args[64];
        struct Gen_type_t *eval_result;
        memset(args, 0, sizeof(void*) * 64);
        for(int i=0; i<64; i++){
            if(root->ops[i] != NULL){
               args[i] = eval(root->ops[i], env);
               if(args[i] == NULL)
                   return NULL;
            }
        }
        
        int root_isleaf = 1;

        if(!args[0] || args[0]->type != TYPE_OPERATOR){
            eval_result = make_list(args);
            root_isleaf = 0;
        }else{
            eval_result = apply(args[0], &(args[1]), env);
            if(eval_result == NULL){
                return NULL;
            }
        }
        
        for(int i=0; i<64; i++){
            if(root->ops[i] != NULL)
                root_isleaf &= root->ops[i]->isleaf;
        }

        root->gen_val = eval_result;
        root->isleaf = root_isleaf;
        return eval_result;
    }
    fprintf(stderr, "eval(): should not be there :(\n");
    return NULL;
}

struct AST_Node *READ(char *prompt){
    char *user_in;
    user_in = readline(prompt);

    if(user_in == NULL){
        return NULL;
    }

    struct AST_Node *AST_root = line_reader(user_in);
    return AST_root;
}

struct AST_Node *EVAL(struct AST_Node *root, struct env_t *env){
    struct Gen_type_t *eval_result = eval(root, env);
    if(eval_result == NULL)
        return NULL;
    else
        return root;
}

void PRINT(struct AST_Node *root){
    pr_str(root);
    printf("\n");
    return;
}

int main(void){

    char prompt[128];
    struct AST_Node *ast, *exp;

    snprintf(prompt, sizeof(prompt), "user> ");

    struct env_t *meta_env = init_env();
    add_env_entry(meta_env, "+", &mikal_add);
    add_env_entry(meta_env, "-", &mikal_sub);
    add_env_entry(meta_env, "*", &mikal_mul);
    add_env_entry(meta_env, "/", &mikal_div);

    while(1){
        ast = READ(prompt);

        if(ast == NULL){
            break;
        }
        exp = EVAL(ast, meta_env);
        PRINT(exp);
        AST_destroy(ast);
    }
    destroy_env(meta_env);
}
