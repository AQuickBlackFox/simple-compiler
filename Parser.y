%{
    #include "Node.h"
    NBlock *programBlock;

    extern int yylex();
    void yyerror(const char *s) { printf("ERROR: %sn", s); }
%}

%union {
    Node *node;
    NBlock *block;
    NExpression *expr;
    NStatement *stmt;
    NIdentifier *ident;
    NVariableDeclaration *var_decl;
    std::vector<NVariableDeclaration*> *varvec;
    std::vector<NExpression*> *exprvec;
    std::string *string;
    int token;
}

%token <string> TIDENTIFIER TINTEGER TDOUBLE
%token <token>  TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL
%token <token>  TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TDOT
%token <token>  TPLUS TMINUS TMUL TDIV
%token <token>  TRETURN TEXTERN
%type <ident> ident
%type <expr> numeric expr
%type <block> program stmt
%type <stmt> var_decl
%type <token> comparsion

%left TPLUS TMINUS
%left TMUL TDIV

%start program

%%
program : stmt { programBlock = $1; }
        ;

stmt : var_decl ;

var_decl : ident ident { $$ = new NVariableDeclaration(*$1, *$2); }
        | ident ident TEQUAL expr { $$ = new NVariableDeclaration(*$1, *$2, $4); }
        ;

ident : TIDENTIFIER { $$ = new NIdentifier(*$1); delete $1; }
        ;

numeric : TINTEGER { $$ = new NInteger(atol($1->c_str())); delete $1; }
        | TDOUBLE { $$ = new NDouble(atof($1->c_str())); delete $1; }
        ;

expr : ident TEQUAL expr { $$ = new NAssignment(*$<ident>1, *$3); }
        | ident { $<ident>$ = $1; }
        | numeric
        | expr comparsion expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
        | TLPAREN expr TRPAREN { $$ = $2; }
        ;

comparsion : TCEQ | TCNE | TCLT | TCLE | TCGT | TCGE
        | TPLUS | TMINUS | TMUL | TDIV
        ;
%%