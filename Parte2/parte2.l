%{
#include "parte2.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%%

"galaxia_actual" { return GALAXIA_ACTUAL; }
"mostrar_vecinas"    { return MOSTRAR_VECINAS; }
"viajar_secuencia"   { return VIAJAR_SECUENCIA; }
"help"    { return HELP; }
"viajar"               { return VIAJAR; }
"combustible"          { return COMBUSTIBLE; }
"reabastecer"          { return REABASTECER; }
"ruta_optima"          { return RUTA_OPTIMA; }
"ruta_corta"           { return RUTA_CORTA; }
[0-9]+                 { yylval.intval = atoi(yytext); return NUMERO; }
[a-zA-Z0-9_]+          { yylval.strval = strdup(yytext); return IDENTIFICADOR; }
[ \t]+                 { /* Ignorar espacios */ }
\n                     { return '\n'; }
.                      { /* Ignorar cualquier otro carácter no reconocido */ }


%%

int yywrap() {
    return 1;
}
