%option yylineno
%{
#include "syntax.tab.h"
%}
/* regex */
delim	[ \t\n]
ws	{delim}+
letter	[A-Za-z]
digit8	[0-7]
digit10	[0-9]
digit16	[0-9A-Fa-f]
int8	-?0([1-7]{digit8}*)?
int10	-?(0|[1-9]{digit10}*)
int16	-?0x(0|[1-9A-Fa-f]{digit16}*)
id	{letter}({letter}|{digit10})*
float	{int10}(\.{digit10}+)?([Ee]{int10})?

%%
{ws}	{/*no action and no return*/}
if	{printf("IF\n"); return IF;}
then	{printf("THEN\n"); return THEN;}
else	{printf("ELSE\n"); return ELSE;}
int	{printf("TYPE\n"); return TYPE;}
float	{printf("TYPE\n"); return TYPE;}
struct 	{printf("STRUCT\n"); return STRUCT;}
return	{printf("RETURN\n"); return RETURN;}
while	{printf("WHILE\n"); return WHILE;}
{id}	{printf("ID\n"); return ID;}
{int8}	{printf("INT\n"); return INT;}
{int10}	{printf("INT\n"); yylval = atoi(yytext); return INT;}
{int16}	{printf("INT\n"); return INT;}
{float}	{printf("FLOAT\n"); return FLOAT;}
";"	{printf("SEMI\n"); return SEMI;}
","	{printf("COMMA\n"); return COMMA;}
"="	{printf("ASSIGNOP\n"); return ASSIGNOP;}
"+"	{printf("PLUS\n"); return PLUS;}
"-"	{printf("MINUS\n"); return MINUS;}
"*"	{printf("STAR\n"); return STAR;}
"/"	{printf("DIV\n"); return DIV;}
"&&"	{printf("AND\n"); return AND;}
"||"	{printf("OR\n"); return OR;}
"!"	{printf("NOT\n"); return NOT;}
"."	{printf("DOT\n"); return DOT;}
"("	{printf("LP\n"); return LP;}
")"	{printf("RP\n"); return RP;}
"["	{printf("LB\n"); return LB;}
"]"	{printf("RB\n"); return RB;}
"{"	{printf("LC\n"); return LC;}
"}"	{printf("RC\n"); return RC;}
"<"	{printf("RELOP\n"); return RELOP;}
"<="	{printf("RELOP\n"); return RELOP;}
"=="	{printf("RELOP\n"); return RELOP;}
"<>"	{printf("RELOP\n"); return RELOP;}
">"	{printf("RELOP\n"); return RELOP;}
">="	{printf("RELOP\n"); return RELOP;}
.	{printf("Error type A at Line %d: Mysterious Character \"%s\"\n", yylineno, yytext);}

%%
int yywrap(){
	return 1;
}
