#ifndef __READER_H__ 
#define __READER_H__

enum token_type{
    TOKEN_REGULAR,
    L_PAREN,
    R_PAREN,
    TOKEN_SPECIAL
};

struct Token{
    char tok[128];
    enum token_type type;
};

struct Reader{
    struct Token *token_list;
    int max_token;
    int readpt;
};

struct AST_Node{
    struct Token token;
    struct AST_Node *ops[64];
    int isleaf;
};

void AST_destroy(struct AST_Node *root);
struct AST_Node *line_reader(char *line);
void pr_str(struct AST_Node *root);

#endif
