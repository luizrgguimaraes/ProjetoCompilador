/*
The Cradle - O Berço

O código abaixo foi escrito por Felipo Soranz e é uma adaptação
do código original em Pascal escrito por Jack W. Crenshaw em sua
série "Let's Build a Compiler".

Este código é de livre distribuição e uso.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#define MAXNAME 30
#define MAXNUM 5

char look; /* O caracter lido "antecipadamente" (lookahead) */

/* protótipos */
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

void skipWhite();
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

/* inicialização do compilador */
void init()
{
	printf("init %c\n",look);
    	nextChar();
		skipWhite();
}


void sinal(){
	printf("sinal %c\n",look);

	if(look == '-'){
		match('-');
		emit("XOR AX, AX");
	}


}

void ident(){
	printf("ident %c\n",look);

	char name[MAXNAME+1];
	getName(name);
	if(look == '('){
		match('(');
		match(')');
		emit("CALL %s",name);
	}else{
		emit("MOV AX,[%s]",name);
	}
}

void factor(){
	printf("factor %c\n",look);

	sinal();

	char num[MAXNUM+1];
	

	if(look == '('){
		match('(');
		expression();
		match(')');
	}else if(look >= 'a' && look <='z'){
		ident();
	}else{
		getNum(num);
		emit("MOV AX, %s", num);

	}
}

void term(){
	printf("term %c\n",look);

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
	printf("expression %c\n",look);
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
	printf("assignment %c\n",look);

	char name[MAXNAME+1];
	getName(name);
	match('=');
	expression();
	emit("MOV [%s], AX",name);

}

void multiply(){
	printf("multiply %c\n",look);

	match('*');
	factor();
	emit("POP BX");
	emit("IMUL BX");
}

void divide(){
	printf("divide %c\n",look);
	match('/');
	factor();
	emit("POP BX");
	emit("XCHG AX, BX");
	emit("CWD");
	emit("IDIV BX");
}
void add(){
	printf("add %c\n",look);

	match('+');
	term();
	emit("POP BX");
	emit("ADD AX, BX");
}
void subtract(){
	printf("subtract %c\n",look);

	match('-');
	term();
	emit("POP BX");
	emit("SUB AX, BX");
	emit("NEG AX");
}

void skipWhite(){
	while(look == ' ' || look == '\t')
		nextChar();
}

int isAddOp(char c){
	return (c == '+' || c == '-');
}

/* lê próximo caracter da entrada */
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
	skipWhite();
}

/* recebe o nome de um identificador */
char getName(char *name)
{
    	int i;
    	if (!isalpha(look))
        	expected("Name");
	for(i=0;isalnum(look);i++){
		if(i >= MAXNAME)
			fatal("Identifier too long!");
		name[i] = toupper(look);
		nextChar();
	}
	name[i] = '\0';
	skipWhite();
}

/* recebe um número inteiro */
char getNum(char *num){
    	int i;
    	if (!isdigit(look))
        	expected("Integer");
	for(i=0;isdigit(look);i++){
		if(i >= MAXNUM)
			fatal("Integer too long!");
		num[i] = look;
		nextChar();
	}
	num[i] = '\0';
	skipWhite();
}

/* emite uma instrução seguida por uma nova linha */
void emit(char *fmt, ...)
{
    va_list args;

    putchar('\t');

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    putchar('\n');
}
