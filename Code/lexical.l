%{
	char IF[] = "IF";
	char THEN[] = "THEN";
	char ELSE[] = "ELSE";
	char STRUCT[] = "STRUCT";
	char RETURN[] = "RETURN";
	char WHILE[] = "WHILE";
	char ID[] = "ID";
	char INT[] = "INT";
	char FLOAT[] = "FLOAT";
	char SEMI[] = "SEMI";
	char COMMA[] = "COMMA";
	char ASSIGNOP[] = "ASSIGNOP";
	char PLUS[] = "PLUS";
	char MINUS[] = "MINUS";
	char STAR[] = "STAR";
	char DIV[] = "DIV";
	char AND[] = "AND";
	char OR[] = "OR";
	char DOT[] = "DOT";
	char NOT[] = "NOT";
	char TYPE[] = "TYPE";
	char RELOP[] = "RELOP";
	char LP[] = "LP";
	char RP[] = "RP";
	char LB[] = "LB";
	char RB[] = "RB";
	char LC[] = "LC";
	char RC[] = "RC";
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
if	{printf("%s\n", IF);}
then	{printf("%s\n", THEN);}
else	{printf("%s\n", ELSE);}
struct 	{printf("%s\n", STRUCT);}
return	{printf("%s\n", RETURN);}
while	{printf("%s\n", WHILE);}
{id}	{printf("%s\n", ID);}
{int8}	{printf("%s\n", INT);}
{int10}	{printf("%s\n", INT);}
{int16}	{printf("%s\n", INT);}
{float}	{printf("%s\n", FLOAT);}
";"	{printf("%s\n", SEMI);}
","	{printf("%s\n", COMMA);}
"="	{printf("%s\n", ASSIGNOP);}
"+"	{printf("%s\n", PLUS);}
"-"	{printf("%s\n", MINUS);}
"*"	{printf("%s\n", STAR);}
"/"	{printf("%s\n", DIV);}
"&&"	{printf("%s\n", AND);}
"||"	{printf("%s\n", OR);}
"!"	{printf("%s\n", NOT);}
"."	{printf("%s\n", DOT);}
"int"	{printf("%s\n", TYPE);}
"float"	{printf("%s\n", TYPE);}
"("	{printf("%s\n", LP);}
")"	{printf("%s\n", RP);}
"["	{printf("%s\n", LB);}
"]"	{printf("%s\n", RB);}
"{"	{printf("%s\n", LC);}
"}"	{printf("%s\n", RC);}
"<"	{printf("%s\n", RELOP);}
"<="	{printf("%s\n", RELOP);}
"=="	{printf("%s\n", RELOP);}
"<>"	{printf("%s\n", RELOP);}
">"	{printf("%s\n", RELOP);}
">="	{printf("%s\n", RELOP);}

%%
int main(int argc, char *argv[]){
	yyin = fopen(argv[1], "r");
	yylex();
//	printf("I'm main\n");
	fclose(yyin);
	return 0;
}
int yywrap(){
	return 1;
}

