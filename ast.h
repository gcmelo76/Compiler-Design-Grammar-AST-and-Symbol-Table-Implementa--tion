/*
Guilherme Melo - 2021217138
João Gaspar - 2021221276
*/
#ifndef _AST_H
#define _AST_H

#define categories { "Program", "FuncDeclaration", "FuncDefinition", "ParamList", "FuncBody",\
                 "ParamDeclaration", "Declaration", "StatList", "If", "While", "Return", "Or", \
                 "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Add", "Sub", "Mul", "Div", "Mod", \
                 "Not", "Minus", "Plus", "Store", "Comma", "Call", "BitWiseAnd", "BitWiseOr", \
                 "BitWiseXor", "Char", "ChrLit", "Identifier", "Int", "Short", "Natural", "Double",\
                  "Decimal", "Void", "Null", "Undefined"};

enum category { Program, FunctionDeclaration, FunctionDefinition, ParamList, FuncBody,
                ParamDeclaration, Declaration, StatList, If, While, Return, Or,
                And, Eq, Ne, Lt, Gt, Le, Ge, Add, Sub, Mul, Div, Mod, Not, Minus,
                Plus, Store, Comma, Call, BitWiseAnd, BitWiseOr, BitWiseXor,
                Char, ChrLit, Identifier, Int, Short, Natural, Double, Decimal,
                Void, Null, Undefined
                };

enum type {int_type, char_type, short_type, double_type, void_type, undef_type, no_type, func_type};

struct node {
    enum category category;
    char *token;
    int line;
    int column;    
    enum type type;
    struct node *function;
    struct node_list *children;
};

struct node_list {
    struct node *node;
    struct node_list *next;
};

struct node *newNode(enum category category, char *token);
void addChild(struct node *parent, struct node *child);
void addFirstChild(struct node *parent, struct node *child);
struct node *getChild(struct node *parent, int position);
void show(struct node *n, int level);

char* type_name(enum type type);
enum type category_type(enum category category);

#endif