# Compiler Project: Grammar Rewriting, AST Construction, and Symbol Table

This project implements a compiler module focusing on grammar rewriting, Abstract Syntax Tree (AST) construction, and symbol table management. Designed for handling syntactic structures, resolving ambiguities, and efficiently managing symbols, this project provides a foundation for building robust compilers.

---

## Features

### 1. Grammar Rewriting
- Eliminated ambiguities in constructs like `if-else` using `%nonassoc`, `%left`, and `%right`.
- Defined operator precedence and associativity for correct evaluation:
  - **Example**: `%right NOT` ensures proper handling of unary operators like `+` and `-`.
- Introduced specific nodes for better AST representation:
  - `ExprList` and `ExprCall` distinguish between expressions involving commas.

### 2. Abstract Syntax Tree (AST)
- Implemented global variables to store type declarations.
- Developed functions for hierarchical AST construction:
  - `AddFirstChild` ensures proper insertion of child nodes.
  - Dynamic child inheritance allows flexible AST modification.

### 3. Symbol Table
- Designed a global symbol table to store and retrieve symbols efficiently.
- Utilized professor-provided data structures as a base for implementation.

---

## Technologies Used

- **Programming Language**: C
- **Tools**: Bison (Parser Generator), Lex (Lexer Generator)
- **Key Concepts**: Abstract Syntax Tree (AST), Symbol Table, Grammar Rewriting

---

## Grammar Rules

The grammar is defined in Backus-Naur Form (BNF) for readability and clarity. Below are key components:

### Function Declarations
```bnf
FuncDefinition ::= Type FuncDeclarator FuncBody
FuncDeclarator ::= IDENTIFIER LPAR ParamList RPAR
ParamList ::= ParamDeclaration | ParamList COMMA ParamDeclaration
```

---

# Installation and Setup

Follow these steps to install and set up the Compiler Project:

---

## Prerequisites

Ensure you have the following installed on your system:
- **Compiler**: GCC (or any C compiler supporting POSIX).
- **Tools**: Bison (Parser Generator), Flex (Lexer Generator).

---

## Steps

### 1. Clone the Repository
Download the project files by cloning the GitHub repository:
```bash
git clone https://github.com/your-repository/compiler-project.git
cd compiler-project
```
### 2. Generate the Lexer and Parser
Use Bison and Flex to generate the required files for parsing and lexing:
```bash
bison -d grammar.y
flex lexer.l
gcc -o compiler grammar.tab.c lex.yy.c -lfl
```
### 3. Run the Compiler
Execute the compiled program with your input file:
```bash
./compiler input_file.c
```



