#include "stdio.h"
#include "stdlib.h"
#include "readline/readline.h"
#include "reader.h"

struct AST_Node *READ(char *prompt){
    char *user_in;
    user_in = readline(prompt);

    if(user_in == NULL){
        return NULL;
    }

    struct AST_Node *AST_root = line_reader(user_in);
    return AST_root;
}

struct AST_Node *EVAL(struct AST_Node *root){
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

    while(1){
        ast = READ(prompt);

        if(ast == NULL){
            break;
        }
        exp = EVAL(ast);
        PRINT(exp);
        AST_destroy(ast);
    }

}
