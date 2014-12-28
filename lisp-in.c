#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

#define MAX 1000

struct node{
	int i;			/* to store integer value				t = 'i'		*/
	char *s;		/* to store symbol					t = 's'		*/
	double f;		/* to store floating value				t = 'f'		*/
	struct node *head;	/* to store the address of first element of a list	t = 'l'		*/

	char v;			/* to store which type of function pointer		t = 'F'		*/
	int (*fp1) (int);		// v = '1'
	double (*fp2) (double);		// v = '2'
	double (*fp3) (double,double);	// v = '3'

	char t;			// to specify whether the node has an integer or a float or a symbol or a function pointer
	struct node *next;	// to store the address of next node
};

struct number{
	int flag;
	int inum;
	double fnum;
};

struct list{
	char *w;
	struct list *lnext;
};

int power(int a, int b);
void checkProgram(char *p);
char *pop(struct list **href);
struct node *newNode(char *w);
struct list *lnewNode(char *w);
struct list *tokenize(char *p);
void print(struct node *current);
struct number *isNumber(char *w);
void printList(struct list *current);
struct list *appendList(struct list *h, char *w);
void appendNode(struct node **h, struct node *w);
struct node *read_from_list(struct list **tokens);

main()
{
	struct list *lhead = NULL;
	struct node *nhead = NULL;
	char program[MAX] = "(begin (define r 10) (* pi (* r r)))";

	checkProgram(program);	// checking syntax of the program

	lhead = tokenize(program);

	printf("\n\n");
	printList(lhead);
	printf("\n\n");

	nhead = read_from_list(&lhead);
	printf("\n\n");
	print(nhead);
	printf("\n\n");
}

struct node *env[HASHMAX];

struct node *standard_env(void)
{
	env[hash("+")] = updateEnv(add);
	env[hash("-")] = updateEnv(sub);
	env[hash("*")] = updateEnv(mul);
	env[hash("/")] = updateEnv(div);

	env[hash(">")] = updateEnv(gt);
	env[hash("<")] = updateEnv(lt);
	env[hash(">=")] = updateEnv(ge);
	env[hash("<=")] = updateEnv(le);

	env[hash("sqrt")] = updateEnv(sqroot);
	env[hash("abs")] = updateEnv(absval);
	env[hash("pow")] = updateEnv(power);
	env[hash("exp")] = updateEnv(expt);
	env[hash("cos")] = updateEnv(cosine);
	env[hash("sin")] = updateEnv(sine);
	env[hash("log")] = updateEnv(logBe);
}

struct node *div(struct node *head)
{
	double result;
	int flagi = 0;
	int flagf = 0;
	int resulti = 1;
	double resultf = 1;
	struct node *new = NULL;

	if(length(head) > 1)
		


	while(head != NULL){
		switch(head->t){
			case 'i':
				resulti /= head->i;
				flagi = 1;
				break;
			case 'f':
				resultf /= head->f;
				flagf = 1;
				break;
			case 's':
				printf("The object \"%s\", passed as argument to / is not the correct type.\n\n",head->s);
				exit(0);
		}
		head = head->next;
	}

	new = (struct node *)malloc(sizeof(strucct node));
	if((flagi && flagf) || flagf){
		new->t = 'f';
		new->f = (double)resulti * resultf;
	}
	else if(flagi){
		new->t = 'i';
		new->i = resulti;
	}
	else{
		new->t = 'i';
		new->i = 1;
	}
	new->next = NULL;

	return new;
}

struct node *mul(struct node *head)
{
	double result;
	int flagi = 0;
	int flagf = 0;
	int resulti = 1;
	double resultf = 1;
	struct node *new = NULL;

	while(head != NULL){
		switch(head->t){
			case 'i':
				resulti *= head->i;
				flagi = 1;
				break;
			case 'f':
				resultf *= head->f;
				flagf = 1;
				break;
			case 's':
				printf("The object \"%s\", passed as argument to * is not the correct type.\n\n",head->s);
				exit(0);
		}
		head = head->next;
	}

	new = (struct node *)malloc(sizeof(strucct node));
	if((flagi && flagf) || flagf){
		new->t = 'f';
		new->f = (double)resulti * resultf;
	}
	else if(flagi){
		new->t = 'i';
		new->i = resulti;
	}
	else{
		new->t = 'i';
		new->i = 1;
	}
	new->next = NULL;

	return new;
}

struct node *sub(struct node *head)
{
	double result;
	int flagi = 0;
	int flagf = 0;
	int resulti = 0;
	double resultf = 0;
	struct node *new = NULL;

	while(head != NULL){
		switch(head->t){
			case 'i':
				resulti -= head->i;
				flagi = 1;
				break;
			case 'f':
				resultf -= head->f;
				flagf = 1;
				break;
			case 's':
				printf("The object \"%s\", passed as argument to - is not the correct type.\n\n",head->s);
				exit(0);
		}
		head = head->next;
	}

	new = (struct node *)malloc(sizeof(strucct node));
	if((flagi && flagf) || flagf){
		new->t = 'f';
		new->f = (double)resulti + resultf;
	}
	else if(flagi){
		new->t = 'i';
		new->i = resulti;
	}
	else{
		new->t = 'i';
		new->i = 0;
	}
	new->next = NULL;

	return new;
}

struct node *add(struct node *head)
{
	double result;
	int flagi = 0;
	int flagf = 0;
	int resulti = 0;
	double resultf = 0;
	struct node *new = NULL;

	while(head != NULL){
		switch(head->t){
			case 'i':
				resulti += head->i;
				flagi = 1;
				break;
			case 'f':
				resultf += head->f;
				flagf = 1;
				break;
			case 's':
				printf("The object \"%s\", passed as argument to + is not the correct type.\n\n",head->s);
				exit(0);
		}
		head = head->next;
	}

	new = (struct node *)malloc(sizeof(strucct node));
	if((flagi && flagf) || flagf){
		new->t = 'f';
		new->f = (double)resulti + resultf;
	}
	else if(flagi){
		new->t = 'i';
		new->i = resulti;
	}
	else{
		new->t = 'i';
		new->i = 0;
	}
	new->next = NULL;

	return new;
}

struct node *logBe(struct node *head)
{
	double result;
	struct node *new = NULL;

	if(head->next != NULL){
		printf("log has been called with %d arguments; it requires exactly 1 argument.",length(head));
		exit(0);
	}

	switch(head->t){
		case 'i':
			result = log(head->i);
			break;
		case 'f':
			result = log(head->f);
			break;
		case 's':
			printf("The object \"%s\", passed as the first argument to log() is not the correct type.\n\n",head->s);
			exit(0);
	}

	new = (struct node *)malloc(sizeof(struct node));
	new->t = 'f';
	new->f = result;
	new->next = NULL;

	return new;
}

struct node *sine(struct node *head)
{
	double result;
	struct node *new = NULL;

	if(head->next != NULL){
		printf("sin has been called with %d arguments; it requires exactly 1 argument.",length(head));
		exit(0);
	}

	switch(head->t){
		case 'i':
			result = sin(head->i);
			break;
		case 'f':
			result = sin(head->f);
			break;
		case 's':
			printf("The object \"%s\", passed as the first argument to sin() is not the correct type.\n\n",head->s);
			exit(0);
	}

	new = (struct node *)malloc(sizeof(struct node));
	new->t = 'f';
	new->f = result;
	new->next = NULL;

	return new;
}

struct node *cosine(struct node *head)
{
	double result;
	struct node *new = NULL;

	if(head->next != NULL){
		printf("cos has been called with %d arguments; it requires exactly 1 argument.",length(head));
		exit(0);
	}

	switch(head->t){
		case 'i':
			result = cos(head->i);
			break;
		case 'f':
			result = cos(head->f);
			break;
		case 's':
			printf("The object \"%s\", passed as the first argument to cos() is not the correct type.\n\n",head->s);
			exit(0);
	}

	new = (struct node *)malloc(sizeof(struct node));
	new->t = 'f';
	new->f = result;
	new->next = NULL;

	return new;
}

struct node *expt(struct node *head)
{
	double result;
	struct node *new = NULL;

	if(head->next != NULL){
		printf("exp has been called with %d arguments; it requires exactly 1 argument.",length(head));
		exit(0);
	}

	switch(head->t){
		case 'i':
			result = exp(head->i);
			break;
		case 'f':
			result = exp(head->f);
			break;
		case 's':
			printf("The object \"%s\", passed as the first argument to exp() is not the correct type.\n\n",head->s);
			exit(0);
	}

	new = (struct node *)malloc(sizeof(struct node));
	new->t = 'f';
	new->f = result;
	new->next = NULL;

	return new;
}

struct node *power(struct node *head)
{
	int l;
	double x,y;
	double result;
	struct node *new = NULL;

	if((l = length(head)) != 2){
		printf("pow has been called with %d arguments; it requires exactly 2 argument.",l);
		exit(0);
	}

	switch(head->t){
		case 'i':
			x = (double)head->i;
			break;
		case 'f':
			x = head->f;
			break;
		case 's':
			printf("The object \"%s\", passed as the first argument to pow() is not the correct type.\n\n",head->s);
			exit(0);
	}

	switch(head->next->t){
		case 'i':
			y = (double)head->next->i;
			break;
		case 'f':
			y = head->next->f;
			break;
		case 's':
			printf("The object \"%s\", passed as the second argument to pow() is not the correct type.\n\n",head->next->s);
			exit(0);
	}

	result = pow(x,y);
	new = (struct node *)malloc(sizeof(struct node));
	new->t = 'f';
	new->f = result;
	new->next = NULL;

	return new;
}

struct node *absval(struct node *head)
{
	int result;
	struct node *new = NULL;

	if(head->next != NULL){
		printf("abs has been called with %d arguments; it requires exactly 1 argument.",length(head));
		exit(0);
	}

	switch(head->t){
		case 'i':
			result = abs(head->i);
			break;
		case 'f':
			result = abs(head->f);
			break;
		case 's':
			printf("The object \"%s\", passed as the first argument to abs() is not the correct type.\n\n",head->s);
			exit(0);
	}

	new = (struct node *)malloc(sizeof(struct node));
	new->t = 'i';
	new->i = result;
	new->next = NULL;

	return new;
}

struct node *sqroot(struct node *head)
{
	double result;
	struct node *new = NULL;

	if(head->next != NULL){
		printf("sqrt has been called with %d arguments; it requires exactly 1 argument.",length(head));
		exit(0);
	}

	switch(head->t){
		case 'i':
			result = sqrt(head->i);
			break;
		case 'f':
			result = sqrt(head->f);
			break;
		case 's':
			printf("The object \"%s\", passed as the first argument to sqrt() is not the correct type.\n\n",head->s);
			exit(0);
	}

	new = (struct node *)malloc(sizeof(struct node));
	new->t = 'f';
	new->f = result;
	new->next = NULL;

	return new;
}

int hash(char *w)
{
	int i = 0;
	int h = 0;

	while(*w){
		h = h + (*w)*power(10,i);
		w++;
		i++;
	}
	return h%HASHMAX;
}

struct node *eval(struct node *x)
{
	if(x->t == 's'){				// variable reference
		var = x->s;
		return env[hash(var)];
	}

	else if(x->t != 'l')				// constant literal
		return copyNode(x);

	else if(strcmp(x->head->s,"quote") == 0){	// (quote exp)
		return x->head->next;

	else if(strcmp(x->head->s,"if") == 0){		// (if test conseq alt)
		test = copyNode(x->head->next);
		conseq = copyNode(x->head->next->next);
		alt = copyNode(x->head->next->next->next);

		cond = eval(test,env);

		switch(cond->t){
			case 'i':
				if(cond->i)
					return eval(conseq,env);
				break;
			case 'f':
				if(cond->f)
					return eval(conseq,env);
				break;
			case 's':
				if(cond->s)
					return eval(conseq,env);
				break;
			default:
				printf("\n\ncheck here\n\n");
				break;
		}
		return eval(alt, env);
	}

	else if(strcmp(x->head->s,"define") == 0){	// (define var exp)
		var = x->head->next->s;
		exp = copyNode(x->head->next->next);
		env[hash(var)] = eval(exp, env);
		return;
	}

	else{						// (proc arg...)
		proc = eval(copyNode(x->head), env);
		current = x->head->next;

		while(current != NULL){
			appendNode(&head,copyNode(current));
			current = current->next;
		}

		switch(proc->v){
			case '1':
				return (proc->fp1)(head);
				break;
			case '2':
				return (proc->fp2)(head);
				break;
			case '3':
				return (proc->fp3)(head);
				break;
		}
	}
		

}

struct node *copyNode(struct node *old)
{
	struct node *new = (struct node *)malloc(sizeof(struct node));

	new->t = old->t;
	switch(new->t){
		case 'i':
			new->i = old->i;
			break;
		case 's':
			new->s = old->s;
			break;
		case 'f':
			new->f = old->f;
			break;
		case 'l':
			new->head = old->head;
			break;
		case 'F':
			new->v = old->v;
			switch(new->v){
				case '1':
					new->fp1 = old->fp1;
					break;
				case '2':
					new->fp2 = old->fp2;
					break;
				case '3':
					new->fp3 = old->fp3;
					break;
			}
			break;
	}
	new->next = NULL;

	return new;
}

void checkProgram(char *p)
{
	int count = 0;

	if(!strlen(p)){
		printf("\n\nunexcepted End of File!!! exiting...\n\n");
		exit(0);
	}

	while(*p){
		if(*p == '(')
			++count;
		else if(*p == ')')
			--count;
		++p;
	}

	if(count != 0){
		printf("\n\nunequal number of '(' and ')' !!! exiting...\n\n");
		exit(0);
	}

}

struct list *tokenize(char *p)
{
	char word[100],*w;
	struct list *head = NULL;

	while(*p){
		w = word;
		if(isspace(*p))
			++p;
		else if(!isalnum(*p)){
			*w++ = *p++;
			*w = '\0';
			head = appendList(head,word);
		}
		else{
			while(isalnum(*p))
				*w++ = *p++;
			*w = '\0';
			head = appendList(head,word);
		}
	}
	return head;
}

struct list *appendList(struct list *h, char *w)
{
	
	struct list *current = h;

	if(h == NULL)
		return lnewNode(w);

	while(current->lnext != NULL)
		current = current->lnext;
	current->lnext = lnewNode(w);

	return h;
}

struct list *lnewNode(char *w)
{
	struct list *new = (struct list *)malloc(sizeof(struct list));

	new->w = strdup(w);
	new->lnext = NULL;

	return new;
}

struct node *read_from_list(struct list **tokens)
{
	char *token;
	struct node *L = NULL;

	token = pop(tokens);
	if(strcmp(token,"(") == 0){
		L = newNode(token);

		while(strcmp((*tokens)->w,")") != 0)
			appendNode(&(L->head),read_from_list(tokens));

		pop(tokens); // pop off ')'
		return L;
	}
	else
		return newNode(token);
	
		
}

char *pop(struct list **href)
{
	char *w = (*href)->w;
	struct list *tmp = *href;
	*href = (*href)->lnext;
	free(tmp);

	return w;
}


void appendNode(struct node **h, struct node *w)
{
	struct node *current = *h;

	if(current == NULL){
			*h = w;
		return;
	}

	while(current->next != NULL)
		current = current->next;
		current->next = w;
}

struct node *newNode(char *w)
{
	struct node *h = (struct node*)malloc(sizeof(struct node));
	struct number *num = NULL;

	if(num = isNumber(w)){
		if(num->flag == 1){
			h->i = num->inum;
			h->t = 'i';
		}
		else{
			h->f = num->fnum;
			h->t = 'f';
		}
	}
	else if(strcmp(w,"(") == 0){
		h->t = 'l';
		h->head = NULL;
	}
	else{
		h->s = strdup(w);
		h->t = 's';
	}
	h->next = NULL;

	return h;
}

struct number *isNumber(char *w)
{
	int i = 0;
	int sum = 0;
	int fra = 0;
	struct number *new = NULL;

	while(isdigit(*w)){
		sum = sum * 10 + (*w - '0');
		++w;
	}
	if(*w == '\0'){	// integer
		new = (struct number *)malloc(sizeof(struct number));
		new->inum = sum;
		new->flag = 1;
		return new;
	}
	else if(*w == '.'){
		while(isdigit(*w)){
			fra = fra * 10.0 + (*w - '0');
			++w;
			++i;
		}
		if(*w == '\0'){	// float
			new = (struct number *)malloc(sizeof(struct number));
			new->fnum = (double)sum + fra/pow(10,i);
			new->flag = 2;
			return new;
		}
	}

	return new; // neither an integer nor a float
		
}

void printList(struct list *current)
{
	while(current != NULL){
		printf(" %s",current->w);
		current = current->lnext;
	}
}


void print(struct node *current)
{
	if(current == NULL)
		return;
	switch(current->t){
		case 'i':
			printf(" %d",current->i);
			break;
		case 'f':
			printf(" %f",current->f);
			break;
		case 's':
			printf(" %s",current->s);
			break;
		case 'l':
			printf(" (");
			print(current->head);
			printf(" )");
			break;
	}

	print(current->next);
}
