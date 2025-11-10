%option c++ noyywrap yylineno nodefault
%{
#include <iostream>

#define	PLUS 		1
#define MINUS 		2
#define MULT 		3
#define DIV 		4
#define EXP 		5
#define POPEN 		6
#define PCLOSE 		7
#define _FLOAT		8
#define _INT		9	

%}

DIGIT 		[0-9]
FLOAT 		{DIGIT}+\.{DIGIT}+
INT 		{DIGIT}+
WHITESPACE	[ \t\n]

%%

"+"		{ std::cout << "found " << "+" << "\n"; return PLUS; }
"-"		{ std::cout << "found " << "-" << "\n"; return MINUS; }
"*" 		{ std::cout << "found " << "*" << "\n"; return MULT; }
"/" 		{ std::cout << "found " << "/" << "\n"; return DIV; }
"^" 		{ std::cout << "found " << "^" << "\n"; return EXP; }
"(" 		{ std::cout << "found " << "(" << "\n"; return POPEN; }
")" 		{ std::cout << "found " << ")" << "\n"; return PCLOSE; }

{FLOAT} 	{ std::cout << "found FLOAT(" << YYText() << ")\n"; return _FLOAT;}
{INT} 		{ std::cout << "found INT(" << YYText() << ")\n"; return _INT;}
{WHITESPACE}+   {}
.               { std::cerr << "unexpected char: " << YYText() << "\n"; return 0; }
%%

int main() {
  yyFlexLexer scanner;
  while (scanner.yylex() != 0) {}
  return 0;
}

