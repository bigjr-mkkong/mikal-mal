#include "stdio.h"
#include "stdlib.h"
#include "readline/readline.h"

char *READ(char *prompt){
    char *user_in;
    user_in = readline(prompt);

    if(user_in == NULL){
        return NULL;
    }

    return user_in;
}

char *EVAL(char *str){
    return str;
}

char *PRINT(char *lineout){
    return lineout;
}

int main(void){

    char prompt[128];
    char *ast, *exp;

    snprintf(prompt, sizeof(prompt), "user> ");

    while(1){
        ast = READ(prompt);

        if(ast == NULL){
            break;
        }
        exp = EVAL(ast);
        puts(PRINT(exp));
        free(ast);
    }

}
