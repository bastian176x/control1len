%{
#include "parte2.tab.h"  // El archivo generado por Bison
#include <stdio.h>
%}

%%

"galaxia"                { return GALAXIA; }  
"arista"                 { return ARISTA; }   
"nave"                   { return NAVE; }     
"reabastecer"          { return REABASTECER; }
"autonomo"             { return AUTONOMO; }
"guiado"               { return GUIADO; }

[0-9]+                    { yylval.intval = atoi(yytext); return NUMERO; }  // Coincide con números
[a-zA-Z0-9_]+             { yylval.strval = strdup(yytext); return IDENTIFICADOR; }  // Coincide con identificadores (galaxias, ubicaciones)

[ \t\n]+                  { /* Ignorar espacios, tabulaciones y saltos de línea */ }
.                         { /* Ignorar cualquier otro carácter no reconocido */ }

%%

int yywrap() {
    return 1;
}
