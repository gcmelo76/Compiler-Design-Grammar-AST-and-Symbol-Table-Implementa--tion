/*
Guilherme Melo - 2021217138
João Gaspar - 2021221276
*/
#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

char* category_names[] = categories;

   struct node *newNode(enum category category, char *token) {
    struct node *new = malloc(sizeof(struct node));
    new->category = category;
    new->token = token;
    new->children = malloc(sizeof(struct node_list));
    new->line = 0;
    new->column = 0;
    new->type = no_type;
    new->children->node = NULL;
    new->children->next = NULL;
    return new;
}

void addChild(struct node *parent, struct node *child) {
    struct node_list *new = malloc(sizeof(struct node_list));
    new->node = child;
    new->next = NULL;
    struct node_list *children = parent->children;
    while(children->next != NULL){
        children = children->next;
    }
    children->next = new;
}

void addFirstChild(struct node *parent, struct node *child) {
    struct node_list *new = malloc(sizeof(struct node_list));
    new->node = child;
    new->next = parent->children->next;
    parent->children->next = new;
}

struct node *getChild(struct node *parent, int position){
    struct node_list *children = parent->children;
    while((children = children->next) != NULL)
        if(position-- == 0)
            return children->node;
    return NULL;
}

void show(struct node *n, int level){
    for(int i = 0; i < level; i++) printf("..");
    if(n -> token)
	    printf("%s(%s)", category_names[n->category], n->token);
    else
	    printf("%s", category_names[n->category]);
    if (n->type == func_type){
        printf(" - %s", type_name(category_type(getChild(n->function, 0)->category)));
        struct node_list *current = getChild(n->function, 2)->children->next;
        printf("(");
        while(current != NULL){
            printf("%s", type_name(category_type(getChild(current->node, 0)->category)));
            current = current->next;
            if(current)
                printf(",");
        }
        printf(")");
    }
    else if(n->type != no_type)
        printf(" - %s", type_name(n->type));
    printf("\n");
    struct node_list *c = n->children;
    while((c = c->next)){
        show(c->node, level + 1);
    }
}

enum type category_type(enum category category){
    switch (category)
    {
    case Int:
        return int_type;
        break;
    case Double:
        return double_type;
        break;
    case Char:
        return char_type;
        break;
    case Void:
        return void_type;
        break;
    case Short:
        return short_type;
        break;
    default:
        return undef_type;
        break;
    }
}

char* type_name(enum type type){
    switch (type)
    {
    case int_type:
        return "int";
        break;
    case double_type:
        return "double";
        break;
    case char_type:
        return "char";
        break;
    case void_type:
        return "void";
        break;
    case short_type:
        return "short";
        break;
    case undef_type:
        return "undef";
        break;
    case func_type:
        return "THIS SHOULD NEVER BE PRINTED";
        break;
    default:
        return "no_type";
        break;
    }

}