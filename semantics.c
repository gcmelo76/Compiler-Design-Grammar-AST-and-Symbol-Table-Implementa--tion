/*
Guilherme Melo - 2021217138
João Gaspar - 2021221276
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantics.h"

struct scoped_table_list *local_table_list;
struct symbol_list *global_symbol_table;


void operatorError(struct node *node1, struct node* node2, char* op, int line, int col){
    printf("Line %d, column %d: Operator %s cannot be applied to ", line, col, op);
    if(node2 == NULL){
        printf("type ");
        if(node1->function == NULL){
            printf("%s", type_name(node1->type));
        }else{
            printf("%s", type_name(category_type(getChild(node1->function, 0)->category)));
            struct node_list *param_list = getChild(node1->function, 2)->children->next;
            printf("(");
            while(param_list != NULL){
                printf("%s", type_name(category_type(getChild(param_list->node, 0)->category)));
                param_list = param_list->next;
                if(param_list != NULL) printf(",");
            }
            printf(")");
        }
    }else{
        printf("types ");
        if (node1->function == NULL){
            printf("%s, ", type_name(node1->type));
        }else {
            printf("%s", type_name(category_type(getChild(node1->function, 0)->category)));
            struct node_list *param_list = getChild(node1->function, 2)->children->next;
            printf("(");
            while(param_list != NULL){
                printf("%s", type_name(category_type(getChild(param_list->node, 0)->category)));
                param_list = param_list->next;
                if(param_list != NULL) printf(",");
            }
            printf("), ");
        }

        if (node2->function == NULL){
            printf("%s", type_name(node2->type));
        }else {
            printf("%s", type_name(category_type(getChild(node2->function, 0)->category)));
            struct node_list *param_list = getChild(node2->function, 2)->children->next;
            printf("(");
            while(param_list != NULL){
                printf("%s", type_name(category_type(getChild(param_list->node, 0)->category)));
                param_list = param_list->next;
                if(param_list != NULL) printf(",");
            }
            printf(")");
        }
    }
    printf("\n");
}

int countParams(struct node *node1, struct node* node2){
    struct node_list *aux1 = getChild(node1, 2)->children->next;
    struct node_list *aux2 = getChild(node2, 2)->children->next;
    int n1 = 0, n2 = 0;
    while (aux1 != NULL){
        aux1 = aux1->next;
        n1++;
    }
    while (aux2 != NULL){
        aux2 = aux2->next;
        n2++;
    }
    if (n1 != n2) {
        printf("Line %d, column %d: Wrong number of arguments to function %s (got %d, expected %d)\n", getChild(node1, 1)->line, getChild(node1, 1)->column, getChild(node1, 1)->token, n1, n2);
    }
    return n1 == n2;
}

void conflictingTypes(struct node * node1, struct node* node2, enum type type, int line, int col){
    printf("Line %d, column %d: Conflicting types (got ", line, col);
    if(node1->type == func_type){
        printf("%s", type_name(category_type(getChild(node1->function, 0)->category)));
        struct node_list *param_list = getChild(node1->function, 2)->children->next;
        printf("(");
        while(param_list != NULL){
            printf("%s", type_name(category_type(getChild(param_list->node, 0)->category)));
            param_list = param_list->next;
            if(param_list != NULL) printf(",");
        }
        printf(")");
    }else{
        printf("%s", type_name(node1->type));
    }
    printf(", expected ");
    if(node2 == NULL){
        printf("%s)", type_name(type));
    }else{
        printf("not done yet");
    }
    printf("\n");
    // %s, expected %s)\n", line, col, "ola", "ola");
}

int compareParams(struct node_list *param1, struct node_list *param2){
    while (param1 != NULL){
        if(comparator(category_type(getChild(param2->node,0)->category), param1->node->type, 5) == undef_type){
            if(param1->node->category == Call){
                conflictingTypes(param1->node, NULL, category_type(getChild(param2->node,0)->category), getChild(param1->node, 0)->line, getChild(param1->node, 0)->column);
            }else{
                conflictingTypes(param1->node, NULL, category_type(getChild(param2->node,0)->category), param1->node->line, param1->node->column);
            }
            return 0;
        }
        param1 = param1->next;
        param2 = param2->next;
    }
    return 1;
}

enum type comparator(enum type type1, enum type type2, int mode){
    if (mode == 0){
        if (type1 == void_type || type2 == void_type)
            return undef_type;
        else if (type1 == undef_type || type2 == undef_type)
            return undef_type;
        else if (type1 == no_type || type2 == no_type)
            return undef_type;
        else if (type1 == func_type || type2 == func_type)
            return undef_type;
        else if (type1 == type2)
            return type1;
        else if (type1 == double_type || type2 == double_type)
            return double_type;
        else if (type1 == int_type || type2 == int_type)
            return int_type;
        else if (type1 == short_type || type2 == short_type)
            return short_type;
        else if (type1 == char_type || type2 == char_type)
            return char_type;
    }
    else if (mode == 1){
        if (type1 == double_type || type2 == double_type)
            return undef_type;
        else if (type1 == void_type || type2 == void_type)
            return undef_type;
        else if (type1 == no_type || type2 == no_type)
            return undef_type;
        else if (type1 == undef_type || type2 == undef_type)
            return undef_type;
        else if ( type1 == func_type || type2 == func_type)
            return undef_type;
        else
            return int_type;
    }
    else if (mode == 2) {
        if (type1 == void_type || type2 == void_type)
            return undef_type;
        else if (type1 == no_type || type2 == no_type)
            return undef_type;
        else if (type1 == undef_type || type2 == undef_type)
            return undef_type;
        else if (type1 == func_type || type2 == func_type)
            return undef_type;
        else
            return int_type;
    }
    else if (mode == 3) {
        if (type1 == type2)
            return type1;
        else if (type1 == double_type && type2 != void_type && type2 != no_type && type2 != func_type)
            return double_type;
        else if (type1 == int_type && type2 != void_type && type2 != no_type && type2 != undef_type && type2 != func_type)
            return int_type;
        else if (type1 == char_type && type2 != void_type && type2 != no_type && type2 != undef_type && type2 != func_type)
            return char_type;
        else if (type1 == short_type && type2 != void_type && type2 != no_type && type2 != undef_type && type2 != func_type)
            return short_type;
        else
            return undef_type;
    }else if (mode == 4) {
        if ( type1 == func_type )
            return undef_type;
        if ( type1 == no_type )
            return undef_type;
        return type1;
    }else if(mode == 5){
        if ( type1 == type2 )
            return type1;
        if ( type1 != double_type && type2 == double_type)
            return undef_type;
        if ( type1 == func_type || type2 == func_type)
            return undef_type;
        if ( type1 == no_type || type2 == no_type)
            return undef_type;
        if ( type1 == undef_type || type2 == undef_type)
            return undef_type;
        if ( type1 == void_type || type2 == void_type)
            return undef_type;
        else return type1;
    }
    else return type1;
}

void addPutcharGetchar(struct symbol_list *symbol_table){
    struct node *putchar = newNode(FunctionDefinition, "putchar");
    addChild(putchar, newNode(Int, NULL));
    addChild(putchar, newNode(Identifier, "putchar"));
    addChild(putchar, newNode(ParamList, NULL));
    addChild(getChild(putchar, 2), newNode(ParamDeclaration, NULL));
    addChild(getChild(getChild(putchar, 2), 0), newNode(Int, NULL));
    insert_symbol(symbol_table, "putchar", int_type, putchar);
    struct node *getchar = newNode(FunctionDefinition, "getchar");
    addChild(getchar, newNode(Int, NULL));
    addChild(getchar, newNode(Identifier, "getchar"));
    addChild(getchar, newNode(ParamList, NULL));
    addChild(getChild(getchar, 2), newNode(ParamDeclaration, NULL));
    addChild(getChild(getChild(getchar, 2), 0), newNode(Void, NULL));
    insert_symbol(symbol_table, "getchar", int_type, getchar);
}

struct symbol_list *insert_symbol(struct symbol_list *symbol_table, char *identifier, enum type type, struct node *node){
    struct symbol_list *new_symbol = malloc(sizeof(struct symbol_list));
    new_symbol->identifier = malloc(sizeof(char) * (strlen(identifier) + 1));
    strcpy(new_symbol->identifier, identifier);
    new_symbol->type = type;
    new_symbol->node = node;
    new_symbol->next = NULL;
    struct symbol_list *symbol = symbol_table;
    while(symbol != NULL) {
        if(symbol->next == NULL) {
            symbol->next = new_symbol;    /* insert new symbol at the tail of the list */
            break;
        } else if(strcmp(symbol->next->identifier, identifier) == 0) {
            free(new_symbol);
            return NULL;           /* return NULL if symbol is already inserted */
        }
        symbol = symbol->next;
    }
    return new_symbol;
}

struct symbol_list *search_symbol(struct symbol_list *symbol_table, char *identifier){
    if (symbol_table == NULL){
        return NULL;
    }
    struct symbol_list *current = symbol_table->next;
    while(current != NULL){
        if(strcmp(current->identifier, identifier) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

struct scoped_table_list *insert_symbol_list(struct node *function, struct symbol_list *symbol_table){
    struct scoped_table_list *new_list = malloc(sizeof(struct scoped_table_list));
    new_list->function = function;
    new_list->symbol_table = symbol_table;
    struct scoped_table_list *current = local_table_list;
    while(current != NULL) {
        if(current->next == NULL) {
            current->next = new_list;    /* insert new symbol at the tail of the list */
            break;
        }
        current = current->next;
    }
    return new_list;
}

struct scoped_table_list *search_symbol_list(char *function){
    struct scoped_table_list *current = local_table_list->next;
    while(current != NULL){
        if(!strcmp(getChild(current->function, 1)->token, function)){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void checkExpression(struct node *expression, struct symbol_list *symbol_table){
    struct symbol_list *symbol = NULL;
    struct node_list *current = expression->children->next;
    int n1, n2;
    switch (expression->category)
    {
    case Natural:
        expression->type = int_type;
        break;
    case ChrLit:
        expression->type = int_type;
        break;
    case Decimal:
        expression->type = double_type;
        break;
    case Null:
        // expression->type = void_type;
        break;
    case Add:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        expression->type = comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 0);
        if(expression->type == undef_type)
            operatorError(getChild(expression, 0), getChild(expression, 1), "+", expression->line, expression->column);
        break;
    case Sub:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        expression->type = comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 0);
        if(expression->type == undef_type)
            operatorError(getChild(expression, 0), getChild(expression, 1), "-", expression->line, expression->column);
        break;
    case Mul:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        expression->type = comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 0);
        if(expression->type == undef_type)
            operatorError(getChild(expression, 0), getChild(expression, 1), "*", expression->line, expression->column);
        break;
    case Div:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        expression->type = comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 0);
        if(expression->type == undef_type)
            operatorError(getChild(expression, 0), getChild(expression, 1), "/", expression->line, expression->column);
        break;
    case Mod:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        if(comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 1) == undef_type){
            operatorError(getChild(expression, 0), getChild(expression, 1), "%", expression->line, expression->column);
        }
        expression->type = int_type;
        break;
    case And:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        if(comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 1) == undef_type){
            operatorError(getChild(expression, 0), getChild(expression, 1), "&&", expression->line, expression->column);
        }   
        expression->type = int_type;
        break;
    case Or:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        if(comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 1) == undef_type){
            operatorError(getChild(expression, 0), getChild(expression, 1), "||", expression->line, expression->column);
        }   
        expression->type = int_type;
        break;
    case BitWiseAnd:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        if(comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 1) == undef_type){
            operatorError(getChild(expression, 0), getChild(expression, 1), "&", expression->line, expression->column);
        }   
        expression->type = int_type;
        break;
    case BitWiseOr:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        if(comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 1) == undef_type){
            operatorError(getChild(expression, 0), getChild(expression, 1), "|", expression->line, expression->column);
        }
        expression->type = int_type;
        break;
    case BitWiseXor:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        if(comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 1) == undef_type){
            operatorError(getChild(expression, 0), getChild(expression, 1), "^", expression->line, expression->column);
        }   
        expression->type = int_type;
        break;
    case Eq:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        if (comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 2) == undef_type){
            operatorError(getChild(expression, 0), getChild(expression, 1), "==", expression->line, expression->column);
        }
        expression->type = int_type;
        break;
    case Ne:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        if (comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 2) == undef_type){
            operatorError(getChild(expression, 0), getChild(expression, 1), "!=", expression->line, expression->column);
        }
        expression->type = int_type;
        break;
    case Lt:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        if (comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 2) == undef_type){
            operatorError(getChild(expression, 0), getChild(expression, 1), "<", expression->line, expression->column);
        }
        expression->type = int_type;
        break;
    case Gt:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        if (comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 2) == undef_type){
            operatorError(getChild(expression, 0), getChild(expression, 1), ">", expression->line, expression->column);
        }
        expression->type = int_type;
        break;
    case Le:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        if (comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 2) == undef_type){
            operatorError(getChild(expression, 0), getChild(expression, 1), "<=", expression->line, expression->column);

        }
        expression->type = int_type;
        break;
    case Ge:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        if (comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 2) == undef_type){
            operatorError(getChild(expression, 0), getChild(expression, 1), ">=", expression->line, expression->column);
        }
        expression->type = int_type;
        break;
    case Not:
        checkExpression(getChild(expression, 0), symbol_table);
        if (comparator(getChild(expression, 0)->type, int_type, 1) == undef_type){
            operatorError(getChild(expression, 0), NULL, "!", expression->line, expression->column);
        }
        expression->type = int_type;
        break; 
    case Plus:
        checkExpression(getChild(expression, 0), symbol_table);
        expression->type = comparator(getChild(expression, 0)->type, -1, 4);
        if(comparator(getChild(expression, 0)->type, -1, 2) == undef_type){
            operatorError(getChild(expression, 0), NULL, "+", expression->line, expression->column);
        }
        break;
    case Minus:
        checkExpression(getChild(expression, 0), symbol_table);
        expression->type = comparator(getChild(expression, 0)->type, -1, 4);
        if(comparator(getChild(expression, 0)->type, -1, 2) == undef_type){
            operatorError(getChild(expression, 0), NULL, "-", expression->line, expression->column);
        }
        break;
    case If:
        checkExpression(getChild(expression, 0), symbol_table);
        if (comparator(getChild(expression, 0)->type, int_type, 1) == undef_type){
            conflictingTypes(getChild(expression, 0), NULL, int_type, getChild(expression, 0)->line, getChild(expression, 0)->column);
        }
        checkExpression(getChild(expression, 1), symbol_table);
        checkExpression(getChild(expression, 2), symbol_table);
        break;
    case While:
        checkExpression(getChild(expression, 0), symbol_table);
        if (comparator(getChild(expression, 0)->type, int_type, 1) == undef_type){
            conflictingTypes(getChild(expression, 0), NULL, int_type, getChild(expression, 0)->line, getChild(expression, 0)->column);
        }
        checkExpression(getChild(expression, 1), symbol_table);
        break;
    case Comma:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        if (comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 2) == undef_type){
            operatorError(getChild(expression, 0), getChild(expression, 1), ",", expression->line, expression->column);
        }
        expression->type = getChild(expression, 1)->type;
        if(expression->type == func_type)
            expression->type = undef_type;
        break;
    case Store:
        checkExpression(getChild(expression, 0), symbol_table);
        checkExpression(getChild(expression, 1), symbol_table);
        expression->type = comparator(getChild(expression, 0)->type, getChild(expression, 1)->type, 3);
        if (expression->type == undef_type)
            operatorError(getChild(expression, 0), getChild(expression, 1), "=", expression->line, expression->column);
        else if (getChild(expression, 0)->type != double_type && getChild(expression, 1)->type == double_type)
            operatorError(getChild(expression, 0), getChild(expression, 1), "=", expression->line, expression->column);
        if (getChild(expression, 0)->category != Identifier)
            printf("Line %d, column %d: Lvalue required\n", getChild(expression, 0)->line, getChild(expression, 0)->column);
        break;
    case Identifier:
        symbol = search_symbol(symbol_table, expression->token);
        if (symbol){
            expression->type = symbol->type;
            return;
        }
        symbol = search_symbol(global_symbol_table, expression->token);
        if (symbol){
            if(symbol->node->category == FunctionDefinition || symbol->node->category == FunctionDeclaration){
                expression->type = func_type;
                expression->function = symbol->node;
                return;
            }
            expression->type = symbol->type;
            return;
        }
        printf("Line %d, column %d: Unknown symbol %s\n", expression->line, expression->column, expression->token);
        expression->type = undef_type;
        break;
    case StatList:
        while (current != NULL){
            checkExpression(current->node, symbol_table);
            current = current->next;
        }
        break;
    case Return:
        symbol = search_symbol(symbol_table, "return");
        checkExpression(getChild(expression, 0), symbol_table);
        if (comparator(symbol->type, getChild(expression, 0)->type, 3) == undef_type){
            conflictingTypes(getChild(expression, 0), NULL, symbol->type, getChild(expression, 0)->line, getChild(expression,0)->column);
        }
        break;
    case Call:
        symbol = search_symbol(global_symbol_table, getChild(expression, 0)->token);
        if (symbol){
            n1 = -1;
            n2 = 0;
            expression->type = symbol->type;
            while (current != NULL){
                checkExpression(current->node, symbol_table);
                current = current->next;
                n1 ++;
            }
            current = getChild(symbol->node, 2)->children->next;
            while (current != NULL){
                if(getChild(current->node, 0)->category != Void)
                    n2 ++;
                current = current->next;
            }
            if(n1 != n2){
                printf("Line %d, column %d: Wrong number of arguments to function %s (got %d, required %d)\n", getChild(expression, 0)->line, getChild(expression,0)->column, getChild(expression, 0)->token, n1, n2);
                return;
            }
            compareParams(expression->children->next->next, getChild(symbol->node, 2)->children->next);
            return;
        }
        printf("Line %d, column %d: Unknown symbol %s\n", getChild(expression, 0)->line, getChild(expression, 0)->column, getChild(expression, 0)->token);
        expression->type = undef_type;
        break;
    default:
        printf("Checking expression\n");
        break;
    }
}

int checkVoidParams(struct node *params){
    int void_col, void_line, first_void = 1;
    struct node_list *current = params->children->next;
    struct node *type = getChild(current->node, 0);
    struct node *identifier = getChild(current->node, 1);
    if(type->category == Void && identifier != NULL){
        printf("Line %d, column %d: Invalid use of void type in declaration\n", type->line, type->column);
        return 0;
    }
    if(type->category == Void){
        void_col = type->column;
        void_line = type->line;
        first_void = 0;
    }
    current = current->next;
    while (current != NULL){
        type = getChild(current->node, 0);
        identifier = getChild(current->node, 1);
        if (type->category == Void){
            if(first_void){
                void_col = type->column;
                void_line = type->line;
                first_void = 0;}
            printf("Line %d, column %d: Invalid use of void type in declaration\n", void_line, void_col);
            return 0;
        }
        current = current->next;
    }
    return 1;
}

void checkParams(struct node *params, struct symbol_list *symbol_table){
    struct node_list *current = params->children->next;
    while (current != NULL){
        struct node *type = getChild(current->node, 0);
        struct node *identifier = getChild(current->node, 1);
        if(identifier){
            if(search_symbol(symbol_table, identifier->token)){
                printf("Line %d, column %d: Symbol %s already defined\n", identifier->line, identifier->column, identifier->token);
            }else{
                insert_symbol(symbol_table, identifier->token, category_type(type->category), current->node);
            }
        }
        current = current->next;
    }
}

void checkLocalDeclaration(struct node *declaration, struct symbol_list *symbol_table){
    struct node *type = getChild(declaration, 0);
    struct node *identifier = getChild(declaration, 1);
    struct node *expression = getChild(declaration, 2);
    if (type->category == Void){
        printf("Line %d, column %d: Invalid use of void type in declaration\n", identifier->line, identifier->column);
        return;
    }
    struct symbol_list *symbol = search_symbol(symbol_table, identifier->token);
    if (symbol != NULL && symbol->node->category == Declaration && symbol->type != category_type(type->category)){
        printf("Line %d, column %d: Symbol %s is already defined\n", identifier->line, identifier->column, identifier->token);
        return;
    }
    insert_symbol(symbol_table, identifier->token, category_type(type->category), declaration);
    if(expression)
        checkExpression(expression, global_symbol_table);
}

void checkGlobalDeclaration(struct node *declaration){
    //! change this
    struct node *type = getChild(declaration, 0);
    struct node *identifier = getChild(declaration, 1);
    struct node *expression = getChild(declaration, 2);
    if(expression)
        checkExpression(expression, global_symbol_table);
    if (type->category == Void){
        printf("Line %d, column %d: Invalid use of void type in declaration\n", identifier->line, identifier->column);
        return;
    }
    struct symbol_list *symbol = search_symbol(global_symbol_table, identifier->token);
    if (symbol != NULL && symbol->node->category == Declaration && symbol->type != category_type(type->category)){
        printf("Line %d, column %d: Symbol %s is already defined\n", identifier->line, identifier->column, identifier->token);
        return;
    }
    insert_symbol(global_symbol_table, identifier->token, category_type(type->category), declaration);
}

int checkProgram(struct node *program){
    global_symbol_table = malloc(sizeof(struct symbol_list));
    global_symbol_table->identifier = NULL;

    local_table_list = malloc(sizeof(struct scoped_table_list));
    local_table_list->function = NULL;
    local_table_list->symbol_table = NULL;
    local_table_list->next = NULL;

    struct node_list *current = program->children->next;
    addPutcharGetchar(global_symbol_table);
    while (current != NULL){
        if (current->node->category == Declaration){
            checkGlobalDeclaration(current->node);
        }else if (current->node->category == FunctionDeclaration){
            //! MISSING REDECLARATION, VOID CHECK AND PARAM CHECK
            struct node *type = getChild(current->node, 0);
            struct node *identifier = getChild(current->node, 1);
            struct node *param_list = getChild(current->node, 2);
            struct symbol_list *symbol = search_symbol(global_symbol_table, identifier->token);
            struct symbol_list *temp_table = malloc(sizeof(struct symbol_list));
            temp_table->next = NULL;
            struct symbol_list *local_table = malloc(sizeof(struct symbol_list));
            local_table->identifier = NULL;
            local_table->next = NULL;
            if(!checkVoidParams(param_list)){
                current = current->next;
                continue;
            }
            if (symbol != NULL && (symbol->node->category == FunctionDeclaration || symbol->node->category == FunctionDefinition)){
                if(!countParams(symbol->node, current->node)){
                    ;
                }
                // checkEqualParams(symbol->node, current->node);
                current = current->next;
                continue;
            }
            checkParams(param_list, temp_table);
            insert_symbol_list(current->node, local_table);
            insert_symbol(global_symbol_table, identifier->token, category_type(type->category), current->node);
        }else if (current->node->category == FunctionDefinition){
            //! MISSING REDECLARATION and PARAM CHECK
            struct node *type = getChild(current->node, 0);
            struct node *identifier = getChild(current->node, 1);
            struct node *param_list = getChild(current->node, 2);
            struct node *body = getChild(current->node, 3);
            struct scoped_table_list *symbol_list = search_symbol_list(identifier->token);
            struct symbol_list *local_table = malloc(sizeof(struct symbol_list));
            local_table->identifier = NULL;
            local_table->next = NULL;
            if(!checkVoidParams(param_list)){
                current = current->next;
                continue;
            }
            if (symbol_list != NULL){
                if(symbol_list->function->category == FunctionDefinition){
                    printf("Line %d, column %d: Symbol %s already defined\n", identifier->line, identifier->column, identifier->token);
                    current = current->next;
                    continue;
                }else if(symbol_list->function->category == FunctionDeclaration){
                    symbol_list->function = current->node;
                    symbol_list->symbol_table = local_table;
                }
            }else{
                insert_symbol_list(current->node, local_table);
            }
            insert_symbol(local_table, "return", category_type(type->category), current->node);
            checkParams(param_list, local_table);
            insert_symbol(global_symbol_table, identifier->token, category_type(type->category), current->node);
            struct node_list *current_body = body->children->next;
            while(current_body != NULL){
                if (current_body->node->category == Declaration)
                    checkLocalDeclaration(current_body->node, local_table);
                else 
                    checkExpression(current_body->node, local_table);
                current_body = current_body->next;
            }
        }
        current = current->next;
    }
    return 1;
}

void showSymbolTable(){
    // Global symbol table
    printf("===== Global Symbol Table =====\n");
    struct symbol_list *current = global_symbol_table->next;
    while(current != NULL){
        printf("%s\t%s", current->identifier, type_name(current->type));
        if (current->node->category == ParamDeclaration)
            printf("\tparam");
        else if (current->node->category == FunctionDefinition || current->node->category == FunctionDeclaration){
            struct node_list *param_list = getChild(current->node, 2)->children->next;
            printf("(");
            while (param_list != NULL){
                printf("%s", type_name(category_type(getChild(param_list->node, 0)->category)));
                param_list = param_list->next;
                if(param_list != NULL)
                    printf(",");
            }
            printf(")");
        }

        printf("\n");
        current = current->next;
    }
    printf("\n");

    // Local symbol tables
    struct scoped_table_list *current_local = local_table_list->next;
    while (current_local != NULL){
        if(current_local->function->category == FunctionDefinition){
            printf("===== Function %s Symbol Table =====\n", getChild(current_local->function, 1)->token);
            current = current_local->symbol_table->next;
            while(current != NULL){
                printf("%s\t%s", current->identifier, type_name(current->type));
                if (current->node->category == ParamDeclaration)
                    printf("\tparam");
                printf("\n");
                current = current->next;
            }
            printf("\n");
        }
        current_local = current_local->next;
    } 

}