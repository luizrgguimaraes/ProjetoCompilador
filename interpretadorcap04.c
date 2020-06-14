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
#include <string.h>

#define MAXNAME 30
#define MAXNUM 5

char look; /* O caracter lido "antecipadamente" (lookahead) */
#define MAXVAR 26

typedef struct Variavel{
	char *name;
	int valor;
}Variavel;


Variavel var[MAXVAR];

void initVar(){
	for(int i = 0; i < MAXVAR;i++){
		var[i].name = NULL;
	}
}

int getVar(char *name,int *valor){
	int ret = 0;
	for(int i = 0; i < MAXVAR;i++){
		if(var[i].name == NULL){
			ret = 0;
			break;
		}else if(strcmp(var[i].name,name)==0){
			*valor = var[i].valor;
			ret = 1;
			break;
		}
	}
	return ret;
}
int setVar(char *name,int valor){
	printf("set var - %s,%d\n",name,valor);
	int ret = 0;
	for(int i = 0; i < MAXVAR;i++){
		printf("i = %d\n",i);
		if(var[i].name == NULL){
			var[i].name = name;
			var[i].valor = valor;
			ret = 1;
			break;
		}else if(strcmp(var[i].name,name)==0){
			var[i].valor = valor;
			ret = 1;
			break;
		}
	}
	return ret;
}
/* protótipos */
void init();
void nextChar();
void error(char *fmt, ...);
void fatal(char *fmt, ...);
void expected(char *fmt, ...);
void match(char c);
char *getName();
int getNum();
void emit(char *fmt, ...);

void sinal();
void ident();
int factor();
int term();
int expression();
void assignment();

void multiply();
void divide();
void add();
void subtract();

void skipWhite();
int isAddOp(char c);
void newLine();

/* PROGRAMA PRINCIPAL */
int main()
{

	init();
	do{
		assignment();
		newLine();
	}while (look != '.');
			//expected("NewLine");
    	return 0;
}

/* inicialização do compilador */
void init()
{

	initVar();
    	nextChar();
		skipWhite();
	printf("init %c\n",look);
	
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

	char *name;
	name = getName();
	if(look == '('){
		match('(');
		match(')');
		emit("CALL %s",name);
	}else{
		emit("MOV AX,[%s]",name);
	}
}

int factor(){
	printf("factor %c\n",look);
	
	int val;
	//sinal();

	//char num[MAXNUM+1];

	if(look == '('){
		match('(');
		val = expression();
		match(')');
	}else if(isalpha(look)){
		if(getVar(getName(),&val)==0){
			fatal("Erro getVar()\n");
		}
		//val = var[getName() - 'A'];
		//val = ident();
	}else{
		val = getNum();
//		emit("MOV AX, %s", num);

	}
	printf("return factor = %d\n",val);
	
	return val;
}

int term(){
	printf("term %c\n",look);

	int val;
	val = factor();
	//factor();
	while(look == '*' || look == '/'){
		//emit("PUSH AX");
		switch(look){
			case '*':
				match('*');
				val *= factor();
				//multiply();
				break;
			case '/':
				match('/');
				val /= factor();
				//divide();
				break;

		}
	}
	printf("return term = %d\n",val);
	return val;
}

int expression(){
	//if(isAddOp(look))
	//	emit("XOR AX, AX");
	//else	
	printf("expression %c\n",look);
	int val;

	if(isAddOp(look))
		val = 0;
	else
		val = term();

	//term();
	while(look == '+' || look == '-'){
		//emit("PUSH AX");
		switch(look){
			case '+':
				//add();
				match('+');
				//printf("vai add\n");
				val += term();
				break;
			case '-':
				match('-');
				val -= term();
				//subtract();
				break;

		}
	}
	//printf("retornou valo de expression\n");
	printf("return expression = %d\n",val);
	return val;
}
void assignment(){
	printf("assignment %c\n",look);

	//char name[MAXNAME+1];
	char *name;
	name = getName();
	printf("name = %s\n",name);
	match('=');
	int val;
	val = expression();
	printf("val = %d\n",val);
	setVar(name,val);
	printf("setou var\n");
	//var[name - 'A'] = val;
	//emit("MOV [%s], AX",name);
	getVar(name,&val);
	printf("RESULTADO: %s = %d\n",name,val);
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
char *getName()
{
	//printf("1");
	char *name;
	name = (char *)malloc(sizeof(char)*(MAXNAME+1));
    	int i;
    	if (!isalpha(look))
        	expected("Name");
	for(i=0;isalnum(look);i++){
		if(i >= MAXNAME)
			fatal("Identifier too long!");
	//printf("2");
		name[i] = toupper(look);
	//printf("3");
		//name = look;
		nextChar();
	}
	name[i] = '\0';
	//printf("4");
	skipWhite();
	//return name[0];
	return name;
}

/* recebe um número inteiro */
int getNum(){
	printf("getNum - look=%c\n",look);
    	char num[MAXNUM+1];
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
	return atoi(num);
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
void newLine(){
	if(look  == '\n')nextChar();
}
