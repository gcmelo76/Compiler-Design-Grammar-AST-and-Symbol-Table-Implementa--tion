/*
Guilherme Melo - 2021217138
João Gaspar - 2021221276
*/
#ifndef _SEMANTICS_H
#define _SEMANTICS_H

#include "ast.h"

struct scoped_table_list {
	struct node *function;
	struct symbol_list *symbol_table;
	struct scoped_table_list *next;
};

struct symbol_list {
	char *identifier;
	enum type type;
	struct node *node;
	struct symbol_list *next;
};

struct symbol_list *insert_symbol(struct symbol_list *symbol_table, char *identifier, enum type type, struct node *node);
struct symbol_list *search_symbol(struct symbol_list *symbol_table, char *identifier);
enum type comparator(enum type type1, enum type type2, int mode);
int checkProgram(struct node *program);
void showSymbolTable();
void addPutcharGetchar(struct symbol_list *symbol_table);

#endif
