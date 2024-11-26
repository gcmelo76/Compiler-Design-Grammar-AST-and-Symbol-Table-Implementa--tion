/*
Guilherme Melo - 2021217138
João Gaspar - 2021221276
*/
%{
  #include "ast.h"
  #include "y.tab.h"
  #include "semantics.h"

  #define PASS_TOKEN() yylval.lexeme = strdup(yytext);
  #define YY_USER_ACTION { syn_line = line; syn_column = column; column += yyleng; yylloc.first_line = syn_line; yylloc.first_column=syn_column;}

  int flag = 0;
  int line = 1, column = 1;
  int syn_line = 1, syn_column = 1;
  int start_line = 1, start_col = 1;
  int errors = 0;

  extern struct node *program;
%}

CHAR          char
INT           int
SHORT         short
DOUBLE        double
IF            if
ELSE          else
WHILE         while
RETURN        return
VOID          void

NOT           "!"
BITWISEAND    "&"
BITWISEOR     "|"
BITWISEXOR    "^"
AND           "&&"
OR            "||"
EQ            "=="
NE            "!="
GE            ">="
LE            "<="
GT            ">"
LT            "<"
LBRACE        "{"
RBRACE        "}"
LPAR          "("
RPAR          ")"
ASSIGN        "="
MUL           "*"
COMMA         ","
DIV           "/"
MINUS         "-"
MOD           "%"
PLUS          "+"
SEMI          ";"

DIGIT         [0-9]
VAR           [a-zA-Z_]
RESERVED      [\[\]]|"++"|"--"|auto|break|case|const|continue|default|do|enum|extern|float|for|goto|inline|long|register|restrict|signed|sizeof|static|struct|switch|typedef|union|unsigned|volatile|_Bool|_Complex|_Imaginary
NEWLINE       \r\n|\n|\r
ESCAPESEQUENCES  \\(n|t|\\|\'|\"|[0-7]{1,3}) 
DECIMAL      ({DIGIT}+"."{DIGIT}*|"."{DIGIT}+|{DIGIT}+)

%x COMMENT_ML
%x COMMENT_SL
%%
"//"                                        { BEGIN(COMMENT_SL); start_col = column; start_line=line;  }
<COMMENT_SL>.                               { ; }
<COMMENT_SL><<EOF>>                         { BEGIN(INITIAL);}
<COMMENT_SL>{NEWLINE}                       { line++; column = 1; BEGIN(INITIAL);}

"/*"                                        { BEGIN(COMMENT_ML); start_col = column-yyleng; start_line=line;  }
<COMMENT_ML>.                               { }
<COMMENT_ML><<EOF>>                         { printf("Line %d, column %d: unterminated comment\n", start_line, start_col); BEGIN(INITIAL);} 
<COMMENT_ML>{NEWLINE}                       { line++; column = 1;} 
<COMMENT_ML>"*/"                            {  BEGIN(INITIAL);}

{NEWLINE}                                   { line++; column = 1; syn_column = column;  syn_line = line; }
" "|"\t"                                    { syn_column = column; }
{CHAR}                                      { if(flag == 1)printf("CHAR\n"); if(flag != 1) return CHAR; }
{INT}                                       { if(flag == 1)printf("INT\n");  if(flag != 1) return INT; }
{SHORT}                                     { if(flag == 1)printf("SHORT\n"); if(flag != 1) return SHORT; }
{DOUBLE}                                    { if(flag == 1)printf("DOUBLE\n"); if(flag != 1) return DOUBLE; }
{IF}                                        { if(flag == 1)printf("IF\n"); if(flag != 1) return IF; }
{ELSE}                                      { if(flag == 1)printf("ELSE\n"); if(flag != 1) return ELSE; }
{WHILE}                                     { if(flag == 1)printf("WHILE\n"); if(flag != 1) return WHILE; }
{RETURN}                                    { if(flag == 1)printf("RETURN\n"); if(flag != 1) return RETURN; }
{VOID}                                      { if(flag == 1)printf("VOID\n"); if(flag != 1) return VOID; }
{NOT}                                       { if(flag == 1)printf("NOT\n"); if(flag != 1) return NOT; }
{BITWISEAND}                                { if(flag == 1)printf("BITWISEAND\n"); if(flag != 1) return BITWISEAND; }
{BITWISEOR}                                 { if(flag == 1)printf("BITWISEOR\n"); if(flag != 1) return BITWISEOR; }
{BITWISEXOR}                                { if(flag == 1)printf("BITWISEXOR\n"); if(flag != 1) return BITWISEXOR; }
{AND}                                       { if(flag == 1)printf("AND\n"); if(flag != 1) return AND; }
{OR}                                        { if(flag == 1)printf("OR\n"); if(flag != 1) return OR; }
{EQ}                                        { if(flag == 1)printf("EQ\n"); if(flag != 1) return EQ; }
{NE}                                        { if(flag == 1)printf("NE\n"); if(flag != 1) return NE; }
{GE}                                        { if(flag == 1)printf("GE\n"); if(flag != 1) return GE; }
{LE}                                        { if(flag == 1)printf("LE\n"); if(flag != 1) return LE; }
{GT}                                        { if(flag == 1)printf("GT\n"); if(flag != 1) return GT; }
{LT}                                        { if(flag == 1)printf("LT\n"); if(flag != 1) return LT; }
{LBRACE}                                    { if(flag == 1)printf("LBRACE\n"); if(flag != 1) return LBRACE; }
{RBRACE}                                    { if(flag == 1)printf("RBRACE\n"); if(flag != 1) return RBRACE; }
{LPAR}                                      { if(flag == 1)printf("LPAR\n"); if(flag != 1) return LPAR; }
{RPAR}                                      { if(flag == 1)printf("RPAR\n"); if(flag != 1) return RPAR; }
{ASSIGN}                                    { if(flag == 1)printf("ASSIGN\n"); if(flag != 1) return ASSIGN; }
{MUL}                                       { if(flag == 1)printf("MUL\n"); if(flag != 1) return MUL; }
{COMMA}                                     { if(flag == 1)printf("COMMA\n"); if(flag != 1) return COMMA; }
{DIV}                                       { if(flag == 1)printf("DIV\n"); if(flag != 1) return DIV; }
{MINUS}                                     { if(flag == 1)printf("MINUS\n"); if(flag != 1) return MINUS; }
{MOD}                                       { if(flag == 1)printf("MOD\n"); if(flag != 1) return MOD; }
{PLUS}                                      { if(flag == 1)printf("PLUS\n"); if(flag != 1) return PLUS; }
{SEMI}                                      { if(flag == 1)printf("SEMI\n"); if(flag != 1) return SEMI; }
                        
{RESERVED}                                  { if(flag == 1)printf("RESERVED(%s)\n", yytext); if(flag != 1) return RESERVED; }
{VAR}+({VAR}|{DIGIT})*                      { if(flag == 1)printf("IDENTIFIER(%s)\n", yytext); PASS_TOKEN(); if(flag != 1) return IDENTIFIER; }
{DIGIT}{DIGIT}*                             { if(flag == 1)printf("NATURAL(%s)\n", yytext); PASS_TOKEN(); if(flag != 1) return NATURAL; }
{DECIMAL}([Ee][+-]?{DIGIT}+)?              { if(flag == 1)printf("DECIMAL(%s)\n", yytext); PASS_TOKEN(); if(flag != 1) return DECIMAL; }

\'([^\'\n\r\\]|{ESCAPESEQUENCES})\'            { if(flag == 1)printf("CHRLIT(%s)\n", yytext); PASS_TOKEN(); if(flag != 1) return CHRLIT; }
\'([^\'\n\r\\]|{ESCAPESEQUENCES})*             { printf("Line %d, column %d: unterminated char constant\n", line, column-yyleng);  }
\'([^\'\n\r\\]|{ESCAPESEQUENCES}|[^\'\n\r]*)   { printf("Line %d, column %d: unterminated char constant\n", line, column-yyleng);  }
\'([^\'\n\r\\]|{ESCAPESEQUENCES}|\\[^\n\r'])*\'  { printf("Line %d, column %d: invalid char constant (%s)\n", line, column-yyleng, yytext);  }

.                                           { printf("Line %d, column %d: unrecognized character (%s)\n", line, column-yyleng, yytext);  }

%%
extern int yylex();



int main(int argc, char **argv) {
  if(argc > 1){
    if(!strcmp(argv[1], "-l")){
      flag = 1;
    }else if(!strcmp(argv[1], "-t")){
      flag = 2;
    }else if(!strcmp(argv[1], "-s")){
      flag = 3;
    }
  }

  if(flag == 1){
    yylex(); 
  }else{
    yyparse();
    if(flag == 2 && errors == 0){
      show(program, 0);
    }
    else if(flag == 3 && errors == 0){
      checkProgram(program);
      showSymbolTable();
      show(program, 0);
    }
  }
  return 0;
}

int yywrap() {
  return 1;
}

void yyerror(char *error) {
    errors++;
    printf("Line %d, column %d: %s: %s\n", syn_line, syn_column, error,  yytext);
}