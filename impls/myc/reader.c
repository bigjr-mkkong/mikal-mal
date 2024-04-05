#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "reader.h"

#define MAX_SYM     512

char single_symtab[] = {'[', ']', '{', '}', '(', ')', '\'', '`', '~', '^', '@'};

char special_char[] = {'[', ']', '{', '}', '(', ')', '\'', ' ', '\"', '`', ';'};

int is_special_char(int ch){
    for(int i=0; i<sizeof(special_char); i++){
        if(ch == special_char[i]){
            return 1;
        }
    }
    return 0;
}

static int get_token_len(char *st){
    for(int i=0; i<sizeof(single_symtab); i++){
        if(*st == single_symtab[i]){
            return 1;
        }
    }
    int endpt = 1;
    if(*st == '\"'){
        while((st[endpt] != '\"') || ((st[endpt] == '\"') && (st[endpt-1] == '\\'))){
            endpt++;
            if(st[endpt] == 0){
                fprintf(stderr, "String not close\n");
                return -1;
            }
        }
        return endpt + 1;
    }else{
        for(; *(st+endpt); endpt++){
            if(is_special_char(st[endpt])){
                break;
            }

            if(st[endpt] == ','){
                break;
            }
        }
    }

    return endpt;
}


struct Reader *tokenize(char *line){
    int line_len = strlen(line);
    
    struct Reader *token_reader = malloc(sizeof(struct Reader)); 
    token_reader->token_list = (struct Token*)malloc(sizeof(struct Token) * 512);
    struct Token *token_list = token_reader->token_list;
    int wrtpt = 0;
    
    int next = 0;
    int token_len = 0;
    for(; line[next] != '\0';){
        if(line[next] == ' ' || line[next] == ','){
            next += 1;
            continue;
        }

        if(next < (line_len-1) && (line[next] == '~' && line[next+1] == '@')){
            memcpy((token_list[wrtpt].tok), &(line[next]), 2);
            token_list[wrtpt].type = TOKEN_SPECIAL;
            
            token_len = 2;
            next += token_len;
            wrtpt += 1;
        }else{
            token_len = get_token_len(&(line[next]));
            memcpy((token_list[wrtpt].tok), &(line[next]), token_len);
            if(memcmp((token_list[wrtpt].tok), ")", token_len) == 0){
                token_list[wrtpt].type = R_PAREN;
            }else if(memcmp((token_list[wrtpt].tok), "(", token_len) == 0){
                token_list[wrtpt].type = L_PAREN;
            }else{
                token_list[wrtpt].type = TOKEN_REGULAR;
            }
            next += token_len;
            wrtpt += 1;
        }
    }
    token_reader->token_list = token_list;
    token_reader->max_token = wrtpt;
    token_reader->readpt = 0;

    return token_reader;
}

static int AST_Node_isleaf(struct AST_Node *ptr){
    return ptr->isleaf;
}

struct AST_Node *AST_Node_create(struct Token *tok, struct AST_Node **ops, int isleaf){
    struct AST_Node *new_node = (struct AST_Node*)malloc(sizeof(struct AST_Node));
    memset(new_node, 0, sizeof(struct AST_Node));
    if(tok != NULL)
        memcpy(&(new_node->token), tok, sizeof(struct AST_Node));
    
    memcpy(new_node->ops, ops, sizeof(void*) * 64);
    new_node->isleaf = isleaf;
    return new_node;
}

void AST_destroy(struct AST_Node *root){
    if(root == NULL){
        return;
    }
    for(int i=0; i<64; i++){
        if(root->ops[i] != NULL){
            AST_destroy(root->ops[i]);
        }
    }
    free(root);
    return;
}

struct AST_Node *AST_create(struct Reader *tk_reader, int begin, int end){
    struct Token *tk_list = tk_reader->token_list;
    
    struct AST_Node *ops[64];
    memset(ops, 0, sizeof(void*) * 64);
    
    if(begin == end){
        return AST_Node_create(&(tk_list[begin]), ops, 1);
    }

    int idx = begin;

    int subexp_start = 0;
    int subexp_paren_cnt = 0;

    int ops_pt = 0;  

    for(;idx < end; ops_pt++){
        idx++;
        struct Token *tok = &(tk_list[idx]);     

        if(tok->type != L_PAREN && tok->type !=R_PAREN){
            subexp_paren_cnt = 0;
            ops[ops_pt] = AST_create(tk_reader, idx, idx);
        }else if(tok->type == L_PAREN){
            subexp_start = idx;
            for(; idx<=end; idx++){
                tok = &(tk_list[idx]);

                if(tok->type == L_PAREN){
                    subexp_paren_cnt++;
                }else if(tok->type == R_PAREN){
                    subexp_paren_cnt--;
                }

                if(subexp_paren_cnt == 0){
                    ops[ops_pt] = AST_create(tk_reader, subexp_start, idx);
                    break;
                }
            }
        }else{
            continue;
        }
        
        subexp_paren_cnt = 0;
    }
     
   return AST_Node_create(NULL, ops, 0);
}

void pr_str(struct AST_Node *root){
    if(root == NULL) return;

    if(AST_Node_isleaf(root)){
        printf("%s", root->token.tok);
        return;
    }

    printf("(");
    for(int i=0; i<63; i++){
        if(root->ops[i] != NULL){
            pr_str(root->ops[i]);
        }

        if(root->ops[i+1] != NULL){
            printf(" ");
        }
    }
    printf(")");
    return;
}

struct AST_Node *line_reader(char *line){
    struct Reader *tk_reader = tokenize(line);
    struct AST_Node *AST_root = AST_create(tk_reader, 0, tk_reader->max_token-1);
    
    //free tk_reader
    return AST_root;
}
/*
int main(void){
    char str[] = "(nil)";
    struct Reader *tk_reader = tokenize(str);

    struct AST_Node *AST_root = AST_create(tk_reader, 0, tk_reader->max_token - 1);

    pr_str(AST_root);
}
*/
