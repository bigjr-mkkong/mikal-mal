#include "stdio.h"
#include "stdlib.h"
#include "types.h"
#include "readline/readline.h"
#include "reader.h"
#include "env.h"
/*
 * TODO:
 *      Debug memory bugs (if there are any of them)
 *      make sure all errors are been cateched and will not trigger a segfault
 */

struct Gen_type_t *mikal_add(struct Gen_type_t **list_add){
    struct Gen_type_t *adder1 = list_add[0];
    struct Gen_type_t *adder2 = list_add[1];

    if(adder1->type != TYPE_INTEGER){
        fprintf(stderr, "Failed to add: number 1 is in incorrect type\n");
        return NULL;
    }
    if(adder2->type != TYPE_INTEGER){
        fprintf(stderr, "Failed to add: number 2 is in incorrect type\n");
        return NULL;
    }

    struct Gen_type_t *result;
    result = make_integer(adder1->value.integer + adder2->value.integer);

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
    struct Gen_type_t *mul1 = list_mul[0];
    struct Gen_type_t *mul2 = list_mul[1];

    if(mul1->type != TYPE_INTEGER){
        fprintf(stderr, "Failed to multiply: number 1 is in incorrect type\n");
        return NULL;
    }
    if(mul2->type != TYPE_INTEGER){
        fprintf(stderr, "Failed to multiply: number 2 is in incorrect type\n");
        return NULL;
    }

    struct Gen_type_t *result;
    result = make_integer(mul1->value.integer * mul2->value.integer);

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
    /*
     * there is a gen_val field inside struct AST_Node
     * we can put translated/evaluated gen type data there
     * so memory management is easier
     */
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
            }
        }

        int root_isleaf = 1;
        for(int i=0; i<64; i++){
            if(root->ops[i] != NULL)
                root_isleaf &= root->ops[i]->isleaf;
        }

        eval_result = apply(args[0], &(args[1]), env);
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

}
