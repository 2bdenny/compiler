%{
	/* definitions of manifest constants
	   LT, LE, EQ, NE, GT, GE
	   IF, THEN, ELSE, ID, NUMBER, RELOP
	 */
%}

/* regular definitions */
delim	[ \t\n]
ws	{delim}+
letter	[A-Za-z]
digit8	[0-7]
digit10	[0-9]
digit16	[0-9A-Fa-f]
int8	(-)?0(digit8)+
int10	(-)?(0)|([1-9]digit10*)
int16	(-)?0xdigit16+
id	{letter}({letter}|{digit})*
float	{digit}+(\.{digit}+)?(E[+-]?{digit}+)?

%%
{ws}	{/*no action and no return*/}
if	{return(IF);}
then	{return(THEN);}
else	{return(ELSE);}
{id}	{yylval = (int) installID(); return(ID);}
{number}	{yylval = (int) installNum(); return(NUMBER);}
"<"	{yylval = LT; return(RELOP);}
"<="	{yylval = LE; return(RELOP);}
"="	{yylval = EQ; return(RELOP);}
"<>"	{yylval = NE; return(RELOP);}
">"	{yylval = GT; return(RELOP);}
">="	{yylval = GE; return(RELOP);}

%%

int installID() {
	printf("%s\n", yytext);
	return 0;
}

int installNum() {
	printf("%s\n", yytext);
	return 0;
}

%%
