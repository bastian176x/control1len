%{
#include "galaxia.tab.h"  // Incluir el archivo generado por Bison
#include <stdio.h>
void yyerror(const char *s);
%}

%option noinput nounput

DIGITO     [0-9]
LETRA      [a-zA-Z]
ID         {LETRA}({LETRA}|{DIGITO})*
ESPACIO    [ \t]
SALTOLINEA [\n]

%%

"modificar_arista"   { return MODIFICAR_ARISTA; }
"galaxia"            { return GALAXIA; }
"crear_nave"         { return CREAR_NAVE; }
"arista"             { return ARISTA; }
"combustible"        { return COMBUSTIBLE; }
"peso"               { return PESO; }
"reabastecer"        { return REABASTECER; }
"viajar"             { return VIAJAR; }
"autonomo"           { return AUTONOMO; }
"guiado"             { return GUIADO; }
"salir"              { return SALIR; }

{ID}                 { yylval.str = strdup(yytext); return IDENTIFICADOR; }
{DIGITO}+            { yylval.numero = atoi(yytext); return NUMERO; }
"="                  { return IGUAL; }
";"                  { return PUNTOYCOMA; }
","                  { return COMA; }
{ESPACIO}+           { /* Ignorar espacios en blanco */ }
{SALTOLINEA}+        { /* Ignorar saltos de línea */ }

.    {
        fprintf(stderr, "Error léxico en línea %d: Caracter no reconocido '%s'\n", yylineno, yytext);
        return ERROR;
    }

<<EOF>> {
    return 0;  // Retorna EOF al parser
}

%%

int yywrap() {
    return 1;
}
