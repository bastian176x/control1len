%{
#include "galaxia.tab.h"  // Asegúrate de que el archivo generado por Bison está siendo incluido correctamente.
#include <stdio.h>
#include <stdlib.h>
%}

%option noyywrap

DIGITO   [0-9]
LETRA    [a-zA-Z]
ID       {LETRA}({LETRA}|{DIGITO})*
ESPACIO  [ \t]

%%

"subgalaxia"      return SUBGALAXIA;
"galaxia"         return GALAXIA;
"arista"          return ARISTA;
"nave"            return NAVE;
"combustible"     return COMBUSTIBLE;
"peso"            return PESO;
"reabastecer"     return REABASTECER;
"viajar"          return VIAJAR;
"autonomo"        return AUTONOMO;
"guiado"          return GUIADO;
{ID}              { yylval.str = strdup(yytext); return IDENTIFICADOR; }
{DIGITO}+         { yylval.numero = atoi(yytext); return NUMERO; }
"="               return IGUAL;
";"               return PUNTOYCOMA;
","               return COMA;
{ESPACIO}+        {/* Ignorar espacios en blanco */ }
[\n\r]            {/* Ignorar saltos de línea */}
.                 { printf("Caracter no reconocido: %s\n", yytext); return yytext[0]; }

%%
