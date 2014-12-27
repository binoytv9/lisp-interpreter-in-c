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
	env[hash("sqrt")] = dfd(sqrt);
	env[hash("abs")] = ifi(abs);
	env[hash("pow")] = dfdd(pow);
	env[hash("exp")] = dfd(exp);
	env[hash("cos")] = dfd(cos);
	env[hash("sin")] = dfd(sin);
	env[hash("log")] = dfd(log);
}

struct node * add(struct node *head)
{
	

struct node *ifi(int (*fp)(int))
{
	struct node *new = (struct node *)malloc(sizeof(struct node));

	new->t = 'F';
	new->v = '1';
	new->fp1 = fp;
	new->next = NULL;
}

struct node *dfd(double (*fp)(double))
{
	struct node *new = (struct node *)malloc(sizeof(struct node));

	new->t = 'F';
	new->v = '2';
	new->fp2 = fp;
	new->next = NULL;
}

struct node *dfdd(double (*fp)(double,double))
{
	struct node *new = (struct node *)malloc(sizeof(struct node));

	new->t = 'F';
	new->v = '3';
	new->fp3 = fp;
	new->next = NULL;
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
			new->fnum = (double)sum + fra/power(10,i);
			new->flag = 2;
			return new;
		}
	}

	return new; // neither an integer nor a float
		
}

int power(int a, int b)
{
	int pow = 1;

	if(!b)
		return 1;
	while(b--)
		pow *= a;

	return pow;
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
