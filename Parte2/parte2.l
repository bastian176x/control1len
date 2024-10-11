%{
#include "parte2.tab.h"  // El archivo generado por Bison
#include <stdio.h>
%}

%%

"galaxia "                { return GALAXIA; }  // Coincide con 'galaxia ' (el espacio es parte del token)
"arista "                 { return ARISTA; }   // Coincide con 'arista '
"nave "                   { return NAVE; }     // Coincide con 'nave '
"reabastecer"          { return REABASTECER; }


[0-9]+                    { yylval.intval = atoi(yytext); return NUMERO; }  // Coincide con números
[a-zA-Z0-9_]+             { yylval.strval = strdup(yytext); return IDENTIFICADOR; }  // Coincide con identificadores (galaxias, ubicaciones)

[ \t\n]+                  { /* Ignorar espacios, tabulaciones y saltos de línea */ }
.                         { /* Ignorar cualquier otro carácter no reconocido */ }

%%

int yywrap() {
    return 1;
}
