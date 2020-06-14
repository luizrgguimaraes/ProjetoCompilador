/*
The Cradle - O Ber�o

O c�digo abaixo foi escrito por Felipo Soranz e � uma adapta��o
do c�digo original em Pascal escrito por Jack W. Crenshaw em sua
s�rie "Let's Build a Compiler".

Este c�digo � de livre distribui��o e uso.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

char look; /* O caracter lido "antecipadamente" (lookahead) */

/* prot�tipos */
void init();
void nextChar();
void error(char *fmt, ...);
void fatal(char *fmt, ...);
void expected(char *fmt, ...);
void match(char c);
char getName();
char getNum();
void emit(char *fmt, ...);

void sinal();
void ident();
void factor();
void term();
void expression();
void assignment();

void multiply();
void divide();
void add();
void subtract();
int isAddOp(char c);

/* PROGRAMA PRINCIPAL */
int main()
{
    	init();
		assignment();
	if(look != '\n')
		expected("NewLine");
    	return 0;
}

/* inicializa��o do compilador */
void init()
{
    nextChar();
}


void sinal(){
	if(look == '-'){
		match('-');
		emit("XOR AX, AX");
	}


}

void ident(){
	char name;
	name = getName();
	if(look == '('){
		match('(');
		match(')');
		emit("CALL %c",name);
	}else{
		emit("MOV AX,[%c]",getName());
	}
}

void factor(){
	sinal();
	if(look == '('){
		match('(');
		expression();
		match(')');
	}else if(look >= 'a' && look <='z'){
		ident();
	}else{
		//sinal();
		emit("MOV AX, %c", getNum());

	}
}

void term(){
	factor();
	while(look == '*' || look == '/'){
		emit("PUSH AX");
		switch(look){
			case '*':
				multiply();
				break;
			case '-':
				divide();
				break;
			default:
				expected("MulOp");
				break;

		}
	}
}

void expression(){
	//if(isAddOp(look))
	//	emit("XOR AX, AX");
	//else	
	term();
	while(look == '+' || look == '-'){
		emit("PUSH AX");
		switch(look){
			case '+':
				add();
				break;
			case '-':
				subtract();
				break;
			default:
				expected("AddOp");
				break;

		}
	}
}
void assignment(){
	char name;
	name = getName();
	match('=');
	expression();
	emit("MOV [%c], AX",name);

}

void multiply(){
	match('*');
	factor();
	emit("POP BX");
	emit("IMUL BX");
}

void divide(){
	match('/');
	factor();
	emit("POP BX");
	emit("XCHG AX, BX");
	emit("CWD");
	emit("IDIV BX");
}
void add(){
	match('+');
	term();
	emit("POP BX");
	emit("ADD AX, BX");
}
void subtract(){
	match('-');
	term();
	emit("POP BX");
	emit("SUB AX, BX");
	emit("NEG AX");
}

int isAddOp(char c){
	return (c == '+' || c == '-');
}

/* l� pr�ximo caracter da entrada */
void nextChar()
{
    look = getchar();
}

/* exibe uma mensagem de erro formatada */
void error(char *fmt, ...)
{
    va_list args;

    fputs("Error: ", stderr);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fputc('\n', stderr);
}

/* exibe uma mensagem de erro formatada e sai */
void fatal(char *fmt, ...)
{
    va_list args;

    fputs("Error: ", stderr);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fputc('\n', stderr);

    exit(1);
}

/* alerta sobre alguma entrada esperada */
void expected(char *fmt, ...)
{
    va_list args;

    fputs("Error: ", stderr);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fputs(" expected!\n", stderr);

    exit(1);
}

/* verifica se entrada combina com o esperado */
void match(char c)
{
    if (look != c)
        expected("'%c'", c);
    nextChar();
}

/* recebe o nome de um identificador */
char getName()
{
    char name;

    if (!isalpha(look))
        expected("Name");
    name = toupper(look);
    nextChar();

    return name;
}

/* recebe um n�mero inteiro */
char getNum()
{
    char num;

    if (!isdigit(look))
        expected("Integer");
    num = look;
    nextChar();

    return num;
}

/* emite uma instru��o seguida por uma nova linha */
void emit(char *fmt, ...)
{
    va_list args;

    putchar('\t');

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    putchar('\n');
}
