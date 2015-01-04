#include<math.h>
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

#define TOKENSIZE	100
#define HASHMAX		1000
#define MAXINPUT	5000

struct node{
	long i;			/* to store integer value				t = 'i'		*/
	char *s;		/* to store symbol					t = 's'		*/
	double f;		/* to store floating value				t = 'f'		*/
	struct node *head;	/* to store the address of first element of a list	t = 'l'		*/
	struct node *(*fptr) (struct node *); /* to store address of function		t = 'F'		*/

	struct node **currentEnv;/* to store address of current Environment array	t = 'e'		*/
	struct node *outerEnv;	 /* to store address of outer Environment Node		t = 'e'		*/

	struct node *parms;	/* to store address of list of parameters		t = 'p'		*/
	struct node *body;	/* to store address of list of body elements		t = 'p'		*/
	struct node *env;	/* to store address of the current Environment Node	t = 'p'		*/

	char t;			/* to specify whether the node has an int or a float or a symbol or a function pointer or an Environment or a Procedure	*/
	struct node *next;	/* to store the address of next node					*/
};

void repl(void);
long hash(char *w);
void checkProgram(char *p);
struct node *newNode(char *w);
struct node *isString(char *w);
struct node *tokenize(char *p);
struct node *isNumber(char *w);
struct node *standard_env(void);
void print(struct node *current);
int length(struct node *current);
struct node *parse(char *program);
struct node *pop(struct node **href);
struct node *copyNode(struct node *old);
void appendNode(struct node **h, struct node *w);
struct node *read_from_list(struct node **tokens);
struct node **findEnv(char *w, struct node *envNode);
struct node *eval(struct node *x, struct node *envNode);
struct node *updateEnv(struct node *(*fptr) (struct node *), char *s);
struct node *Env(struct node *parms, struct node *args, struct node *outerEnv);

struct node *lt(struct node *head);
struct node *gt(struct node *head);
struct node *le(struct node *head);
struct node *ge(struct node *head);
struct node *min(struct node *head);
struct node *max(struct node *head);
struct node *len(struct node *head);
struct node *cdr(struct node *head);
struct node *car(struct node *head);
struct node *mul(struct node *head);
struct node *sub(struct node *head);
struct node *add(struct node *head);
struct node *cons(struct node *head);
struct node *sine(struct node *head);
struct node *expt(struct node *head);
struct node *div_(struct node *head);
struct node *logBe(struct node *head);
struct node *power(struct node *head);
struct node *cosine(struct node *head);
struct node *absval(struct node *head);
struct node *sqroot(struct node *head);
struct node *begin(struct node *current);

main()
{
	repl(); // A prompt-read-eval-print loop
}

void repl(void)
{
	char *quit;
	struct node *result;
	char program[MAXINPUT];
	struct node *global_env;
	struct node *head = NULL;

	global_env = standard_env();
	while("True"){
		result = NULL;
		printf("\nlisp.in.c]=> ");
		
		if((quit = fgets(program,MAXINPUT,stdin)) == NULL){
			printf("\nEnd of input stream reached\nMoriturus te saluto!!!\n");
			exit(0);
		}
		printf("\n");

		if(result = eval(parse(program),global_env)){
			printf("Value: ");
			print(result);
			printf("\n");
		}
		else
			printf("(nil)\n");
	}
}


struct node *parse(char *program)
{
	struct node *head = NULL;

	checkProgram(program);	// checking syntax of the program
	head = tokenize(program);

	return read_from_list(&head);
}

void checkProgram(char *p)
{
	int count = 0;

	while(*p){
		if(*p == '(')
			++count;
		else if(*p == ')')
			--count;
		++p;
	}

	if(count != 0){
		printf("error> unequal number of '(' and ')'!!! exiting...\n");
		exit(0);
	}

}

struct node *tokenize(char *p)
{
	char word[TOKENSIZE],*w;
	struct node *head = NULL;

	while(*p){
		w = word;
		while(isspace(*p))
			++p;
		if(*p == '\0')
			return head;
		else if(*p == '(' || *p == ')'){
			*w++ = *p++;
			*w = '\0';
			appendNode(&head,newNode(word));
		}
		else{
			while(*p && !isspace(*p) && *p != '(' && *p != ')')
				*w++ = *p++;
			*w = '\0';
			appendNode(&head,newNode(word));
		}
	}
	return head;
}

struct node *read_from_list(struct node **tokens)
{
	struct node *token;

	token = pop(tokens);
	if(token != NULL && (token->t == 's' || token->t == 'S') && strcmp(token->s,"(") == 0){
		struct node *L = (struct node *)malloc(sizeof(struct node));;
		L->t = 'l';
		L->head = NULL;
		L->next = NULL;

		while((*tokens)->t != 'S' || strcmp((*tokens)->s,")") != 0)
			appendNode(&(L->head),read_from_list(tokens));

		pop(tokens); // pop off ')'
		return L;
	}
	else
		return token;
}

struct node *pop(struct node **href)
{
	struct node *tmp = *href;

	if(tmp == NULL)
		return NULL;

	*href = (*href)->next;
	tmp->next = NULL;

	return tmp;
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
	struct node *h = NULL;

	if(h = isNumber(w)){ // int or float
		h->next = NULL;
		return h;
	}
	else
		return isString(w); // string or symbol
}

struct node *isNumber(char *w)
{
	int i = 0;
	long sum = 0;
	long fra = 0;
	int sign = 1;
	struct node *new = NULL;

	if(*w == '-'){
		sign = -1;
		if(*++w == '\0') // just a symbol "-"
			return new;
	}
	while(isdigit(*w)){
		sum = sum * 10 + (*w - '0');
		++w;
	}
	if(*w == '\0'){	// integer
		new = (struct node *)malloc(sizeof(struct node));
		new->i = sum * sign;
		new->t = 'i';
		return new;
	}
	else if(*w == '.'){
		++w;
		while(isdigit(*w)){
			fra = fra * 10.0 + (*w - '0');
			++w;
			++i;
		}
		if(*w == '\0'){	// float
			new = (struct node *)malloc(sizeof(struct node));
			new->f = ((double)sum + fra/pow(10,i)) * sign;
			new->t = 'f';
			return new;
		}
	}

	return new; // neither an integer nor a float return NULL
		
}

struct node *isString(char *w)
{
	char *str;
	int len = strlen(w);
	struct node *h = (struct node *)malloc(sizeof(struct node));

	if(*w == '"' && *(w+len-1) == '"'){
		str = (char *)calloc(len-1,sizeof(char));
		strncpy(str,w+1,len-2);
		str[len-2] = '\0';
		h->t = 's';
	}
	else{
		str = strdup(w);
		h->t = 'S';
	}

	h->s = str;
	h->next = NULL;

	return h;
}
	
struct node *standard_env(void)
{
	struct node *new = (struct node *)malloc(sizeof(struct node));
	struct node **env = (struct node **)calloc(HASHMAX,sizeof(struct node *));

	env[hash("+")] = updateEnv(add,"+");
	env[hash("-")] = updateEnv(sub,"-");
	env[hash("*")] = updateEnv(mul,"*");
	env[hash("/")] = updateEnv(div_,"/");


	env[hash(">")] = updateEnv(gt,">");
	env[hash("<")] = updateEnv(lt,"<");
	env[hash(">=")] = updateEnv(ge,">=");
	env[hash("<=")] = updateEnv(le,"<=");


	env[hash("sqrt")] = updateEnv(sqroot,"sqrt");
	env[hash("abs")] = updateEnv(absval,"abs");
	env[hash("pow")] = updateEnv(power,"pow");
	env[hash("exp")] = updateEnv(expt,"exp");
	env[hash("cos")] = updateEnv(cosine,"cos");
	env[hash("sin")] = updateEnv(sine,"sin");
	env[hash("log")] = updateEnv(logBe,"log");


	env[hash("begin")] = updateEnv(begin,"begin");
	env[hash("min")] = updateEnv(min,"min");
	env[hash("max")] = updateEnv(max,"max");
	env[hash("len")] = updateEnv(len,"len");
	env[hash("cdr")] = updateEnv(cdr,"cdr");
	env[hash("car")] = updateEnv(car,"car");
	env[hash("cons")] = updateEnv(cons,"cons");

	env[hash("pi")] = newNode("3.14");


	new->t = 'e';
	new->currentEnv = env;
	new->outerEnv = NULL;
	new->next = NULL;

	return new;
}

long hash(char *w)
{
	int i = 0;
	long h = 0;
	char *s = w;

	while(*w){
		h = h + (*w) * pow(10,i);
		w++;
		i++;
	}

	return h%HASHMAX;
}

struct node *updateEnv(struct node * (*fptr) (struct node *), char *s)
{
	struct node *new = (struct node *)malloc(sizeof(struct node));

	new->t = 'F';
	new->fptr = fptr;
	new->s = strdup(s);
	new->next = NULL;

	return new;
}

struct node *eval(struct node *x, struct node *envNode)
{
	if(x == NULL){
		printf("x is nil\n");
		return NULL;
	}

	if(x->t == 'S'){						// variable reference
		char *var = x->s;
		return findEnv(var,envNode)[hash(var)];
	}

	else if(x->t != 'l')						// constant literal
		return copyNode(x);

	else if(x->head->t == 'S' && strcmp(x->head->s,"quote") == 0){	// (quote exp)
		return x->head->next;
	}
	else if(x->head->t == 'S' && strcmp(x->head->s,"if") == 0){	// (if test conseq alt)
		struct node *test, *conseq, *alt, *cond;

		test = copyNode(x->head->next);
		conseq = copyNode(x->head->next->next);
		alt = x->head->next->next->next;

		cond = eval(test,envNode);

		switch(cond->t){
			case 'i':
				if(cond->i)
					return eval(conseq,envNode);
			case 'f':
				if(cond->f)
					return eval(conseq,envNode);
			case 's':
				if(cond->s)
					return eval(conseq,envNode);
		}
		return eval(alt, envNode);
	}

	else if(x->head->t == 'S' && strcmp(x->head->s,"define") == 0){	// (define var exp)
		char *var = x->head->next->s;
		struct node *exp = x->head->next->next;
		envNode->currentEnv[hash(var)] = eval(exp, envNode);
		return NULL;
	}

	else if(x->head->t == 'S' && strcmp(x->head->s,"set!") == 0){	// (set! var exp)
		char *var = x->head->next->s;
		struct node *exp = x->head->next->next;
		findEnv(var,envNode)[hash(var)] = eval(exp, envNode);
		return NULL;
	}

	else if(x->head->t == 'S' && strcmp(x->head->s,"lambda") == 0){	// (lambda (var...) body)
		struct node *new = (struct node *)malloc(sizeof(struct node));

		new->t = 'p';
		new->parms = x->head->next->head;
		new->body = x->head->next->next;
		new->env = envNode;
		new->next = NULL;
		return new;
	}

	else{								// (proc arg...)
		struct node *proc, *current, *args = NULL;

		proc = eval(copyNode(x->head), envNode);
		current = x->head->next;
		while(current != NULL){
			appendNode(&args,copyNode(eval(current,envNode)));
			current = current->next;
		}

		if(proc->t == 'p')
			return eval(proc->body,Env(proc->parms,args,proc->env));
		else
			return (proc->fptr)(args);
	}
}

struct node *Env(struct node *parms, struct node *args, struct node *outerEnv)
{
	struct node *newEnvNode = (struct node *)malloc(sizeof(struct node));

	newEnvNode->t = 'e';
	newEnvNode->currentEnv = (struct node **)calloc(HASHMAX,sizeof(struct node *));
	newEnvNode->outerEnv = outerEnv;
	newEnvNode->next = NULL;

	while(parms != NULL && args != NULL){
		newEnvNode->currentEnv[hash(parms->s)] = copyNode(args);
		parms = parms->next;
		args = args->next;
	}

	return newEnvNode;
}

struct node **findEnv(char *w, struct node *envNode)
{
	struct node *env = NULL;

	if(envNode == NULL){
		printf("error> Unbound variable: %s\n",w);
		exit(0);
	}

	if((env = envNode->currentEnv[hash(w)]) != NULL)
		return envNode->currentEnv;

	return findEnv(w,envNode->outerEnv);
}

struct node *copyNode(struct node *old)
{
	if(old == NULL)
		return NULL;

	struct node *new = (struct node *)malloc(sizeof(struct node));
	*new = *old;
	new->next = NULL;

	return new;
}

void print(struct node *current)
{
	if(current == NULL)
		return;

	switch(current->t){
		case 'i':// int
			printf(" %ld",current->i);
			break;
		case 'f':// float
			printf(" %f",current->f);
			break;
		case 'F':// function
			printf("<built-in function %s>",current->s);
			break;
		case 's':// string
			printf(" \"%s\"",current->s);
			break;
		case 'S':// symbol
			printf(" %s",current->s);
			break;
		case 'p':// procedure
			printf("<lisp.in.c.Procedure object at %p>",current);
			break;
		case 'l':// list
			printf(" (");
			print(current->head);
			printf(" )");
			break;
	}

	print(current->next);
}

int length(struct node *current)
{
	int l = 0;
	while(current != NULL){
		l++;
		current = current->next;
	}

	return l;
}

/****** build-in functions ******/
struct node *div_(struct node *head)
{
	int l = 0;
	double result;
	int flagi = 0;
	int flagf = 0;
	long resulti = 1;
	double resultf = 1;
	struct node *new = NULL;
	struct node *current = head;

	while(current != NULL){
		l++;
		if(current->t == 'f')
			flagf = 1;
		current = current->next;
	}

	if(l > 1){
		switch(head->t){
			case 'i':
				if(flagf)
					resultf = (double)head->i;
				else
					resulti = head->i;
				flagi = 1;
				break;
			case 'f':
				resultf = head->f;
				flagf = 1;
				break;
			case 's':
				printf("error> The object \"%s\", passed as argument to / is not the correct type\n",head->s);
				exit(0);
		}
		head = head->next;
	}


	while(head != NULL){
		switch(head->t){
			case 'i':
				if(flagf)
					resultf /= (double)head->i;
				else
					resulti /= head->i;
				flagi = 1;
				break;
			case 'f':
				resultf /= head->f;
				flagf = 1;
				break;
			case 's':
				printf("error> The object \"%s\", passed as argument to / is not the correct type\n",head->s);
				exit(0);
		}
		head = head->next;
	}

	new = (struct node *)malloc(sizeof(struct node));
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
	long resulti = 1;
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
				printf("error> The object \"%s\", passed as argument to * is not the correct type\n",head->s);
				exit(0);
		}
		head = head->next;
	}

	new = (struct node *)malloc(sizeof(struct node));
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
	long resulti = 0;
	double resultf = 0;
	struct node *new = NULL;

	if(length(head) > 1){
		switch(head->t){
			case 'i':
				resulti = head->i;
				flagi = 1;
				break;
			case 'f':
				resultf = head->f;
				flagf = 1;
				break;
			case 's':
				printf("error> The object \"%s\", passed as argument to / is not the correct type\n",head->s);
				exit(0);
		}
		head = head->next;
	}
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
				printf("error> The object \"%s\", passed as argument to - is not the correct type.\n",head->s);
				exit(0);
		}
		head = head->next;
	}

	new = (struct node *)malloc(sizeof(struct node));
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
	long resulti = 0;
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
				printf("error> The object \"%s\", passed as argument to + is not the correct type.\n",head->s);
				exit(0);
		}
		head = head->next;
	}

	new = (struct node *)malloc(sizeof(struct node));
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

struct node *lt(struct node *head)
{
	int l;
	double x,y;
	struct node *new = (struct node *)malloc(sizeof(struct node));

	if((l = length(head)) < 2){
		new->t = 'i';
		new->i = 1;
		new->next = NULL;
		return new;
	}

	switch(head->t){
		case 'i':
			x = (double)head->i;
			break;
		case 'f':
			x = head->f;
			break;
		case 's':
			printf("error> The object \"%s\", passed as the first argument to < is not the correct type.\n",head->s);
			exit(0);
	}

	head = head->next;
	while(head != NULL){
		switch(head->t){
			case 'i':
				y = (double)head->i;
				break;
			case 'f':
				y = head->f;
				break;
			case 's':
				printf("error> The object \"%s\", passed as argument to < is not the correct type.\n",head->s);
				exit(0);
		}
		if(x >= y){
			new->t = 'i';
			new->i = 0;
			new->next = NULL;
			return new;
		}
		x = y;
		head = head->next;
	}

	new->t = 'i';
	new->i = 1;
	new->next = NULL;
	return new;
}

struct node *gt(struct node *head)
{
	int l;
	double x,y;
	struct node *new = (struct node *)malloc(sizeof(struct node));

	if((l = length(head)) < 2){
		new->t = 'i';
		new->i = 1;
		new->next = NULL;
		return new;
	}

	switch(head->t){
		case 'i':
			x = (double)head->i;
			break;
		case 'f':
			x = head->f;
			break;
		case 's':
			printf("error> The object \"%s\", passed as the first argument to > is not the correct type.\n",head->s);
			exit(0);
	}

	head = head->next;
	while(head != NULL){
		switch(head->t){
			case 'i':
				y = (double)head->i;
				break;
			case 'f':
				y = head->f;
				break;
			case 's':
				printf("error> The object \"%s\", passed as argument to > is not the correct type.\n",head->s);
				exit(0);
		}
		if(x <= y){
			new->t = 'i';
			new->i = 0;
			new->next = NULL;
			return new;
		}
		x = y;
		head = head->next;
	}

	new->t = 'i';
	new->i = 1;
	new->next = NULL;
	return new;
}

struct node *le(struct node *head)
{
	int l;
	double x,y;
	struct node *new = (struct node *)malloc(sizeof(struct node));

	if((l = length(head)) < 2){
		new->t = 'i';
		new->i = 1;
		new->next = NULL;
		return new;
	}

	switch(head->t){
		case 'i':
			x = (double)head->i;
			break;
		case 'f':
			x = head->f;
			break;
		case 's':
			printf("error> The object \"%s\", passed as the first argument to <= is not the correct type.\n",head->s);
			exit(0);
	}

	head = head->next;
	while(head != NULL){
		switch(head->t){
			case 'i':
				y = (double)head->i;
				break;
			case 'f':
				y = head->f;
				break;
			case 's':
				printf("error> The object \"%s\", passed as argument to <= is not the correct type.\n",head->s);
				exit(0);
		}
		if(x > y){
			new->t = 'i';
			new->i = 0;
			new->next = NULL;
			return new;
		}
		x = y;
		head = head->next;
	}

	new->t = 'i';
	new->i = 1;
	new->next = NULL;
	return new;
}

struct node *ge(struct node *head)
{
	int l;
	double x,y;
	struct node *new = (struct node *)malloc(sizeof(struct node));

	if((l = length(head)) < 2){
		new->t = 'i';
		new->i = 1;
		new->next = NULL;
		return new;
	}

	switch(head->t){
		case 'i':
			x = (double)head->i;
			break;
		case 'f':
			x = head->f;
			break;
		case 's':
			printf("error> The object \"%s\", passed as the first argument to >= is not the correct type.\n",head->s);
			exit(0);
	}

	head = head->next;
	while(head != NULL){
		switch(head->t){
			case 'i':
				y = (double)head->i;
				break;
			case 'f':
				y = head->f;
				break;
			case 's':
				printf("error> The object \"%s\", passed as argument to >= is not the correct type.\n",head->s);
				exit(0);
		}
		if(x < y){
			new->t = 'i';
			new->i = 0;
			new->next = NULL;
			return new;
		}
		x = y;
		head = head->next;
	}

	new->t = 'i';
	new->i = 1;
	new->next = NULL;
	return new;
}

struct node *logBe(struct node *head)
{
	double result;
	struct node *new = NULL;

	if(head->next != NULL){
		printf("error> \n\nlog has been called with %d arguments; it requires exactly 1 argument\n",length(head));
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
			printf("error> The object \"%s\", passed as the first argument to log() is not the correct type.\n",head->s);
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
		printf("error> \n\nsin has been called with %d arguments; it requires exactly 1 argument\n",length(head));
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
			printf("error> The object \"%s\", passed as the first argument to sin() is not the correct type.\n",head->s);
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
		printf("error> \n\ncos has been called with %d arguments; it requires exactly 1 argument\n",length(head));
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
			printf("error> The object \"%s\", passed as the first argument to cos() is not the correct type.\n",head->s);
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
		printf("error> \n\nexp has been called with %d arguments; it requires exactly 1 argument\n",length(head));
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
			printf("error> The object \"%s\", passed as the first argument to exp() is not the correct type.\n",head->s);
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
		printf("error> \n\npow has been called with %d arguments; it requires exactly 2 argument\n",l);
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
			printf("error> The object \"%s\", passed as the first argument to pow() is not the correct type.\n",head->s);
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
			printf("error> The object \"%s\", passed as the second argument to pow() is not the correct type.\n",head->next->s);
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
	long result;
	struct node *new = NULL;

	if(head->next != NULL){
		printf("error> \n\nabs has been called with %d arguments; it requires exactly 1 argument\n",length(head));
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
			printf("error> The object \"%s\", passed as the first argument to abs() is not the correct type.\n",head->s);
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
		printf("error> \n\nsqrt has been called with %d arguments; it requires exactly 1 argument\n",length(head));
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
			printf("error> The object \"%s\", passed as the first argument to sqrt() is not the correct type.\n",head->s);
			exit(0);
	}

	new = (struct node *)malloc(sizeof(struct node));
	new->t = 'f';
	new->f = result;
	new->next = NULL;

	return new;
}

struct node *begin(struct node *current)
{
	while(current->next != NULL)
		current = current->next;

	return current;
}

struct node *cons(struct node *head)
{
	int l;

	if((l = length(head)) != 2){
		printf("error> \n\ncons has been called with %d arguments; it requires exactly 2 argument\n",l);
		exit(0);
	}
	struct node *x = head;
	struct node *y = head->next;
	struct node *new = (struct node *)malloc(sizeof(struct node));

	new->t = 'l';
	new->head = x;
	new->next = NULL;

	if(y->t == 'l')
		x->next = y->head;
	else
		x->next = y;

	return new;
}

struct node *car(struct node *head)
{
	if(head->t != 'l'){
		printf("error> The object passed as the first argument to car, is not the correct type\n");
		exit(0);
	}
	if(head->head == NULL){
		printf("error> The object passed as the first argument to car, is not the correct type\n");
		exit(0);
	}

	return copyNode(head->head);
}

struct node *cdr(struct node *head)
{
	if(head->t != 'l'){
		printf("error> The object passed as the first argument to cdr, is not the correct type\n");
		exit(0);
	}

	struct node *new = (struct node *)malloc(sizeof(struct node));
	new->t = 'l';
	new->head = head->head->next;
	new->next = NULL;

	return new;
}

struct node *len(struct node *head)
{
	struct node *new = NULL;

	if(head->t != 'l'){
		printf("error> The object passed as an argument to length, is not a list\n");
		exit(0);
	}

	new = (struct node *)malloc(sizeof(struct node));
	new->i = length(head->head);
	new->t = 'i';
	new->next = NULL;

	return new;
}

struct node *max(struct node *head)
{
	double maxnum;
	struct node *maxNode = head;

	if(head == NULL){
		printf("error> \n\nmax has been called with 0 arguments; it requires at least 1 argument\n");
		exit(0);
	}

	switch(head->t){
		case 'i':
			maxnum = (double)head->i;
			break;
		case 'f':
			maxnum = head->f;
			break;
		default:
			printf("error> The object passed as an argument to max, is not a real number\n");
			exit(0);
	}

	head = head->next;
	while(head != NULL){
		switch(head->t){
			case 'i':
				if(maxnum < head->i){
					maxnum = (double)head->i;
					maxNode = head;
				}
				break;
			case 'f':
				if(maxnum < head->f){
					maxnum = head->f;
					maxNode = head;
				}
				break;
			default:
				printf("error> The object passed as an argument to max, is not a real number\n");
				exit(0);
		}
		head = head->next;
	}

	return copyNode(maxNode);
}

struct node *min(struct node *head)
{
	double minnum;
	struct node *minNode = head;

	if(head == NULL){
		printf("error> min has been called with 0 arguments; it requires at least 1 argument\n");
		exit(0);
	}

	switch(head->t){
		case 'i':
			minnum = (double)head->i;
			break;
		case 'f':
			minnum = head->f;
			break;
		default:
			printf("error> The object passed as an argument to min, is not a real number\n");
			exit(0);
	}

	head = head->next;
	while(head != NULL){
		switch(head->t){
			case 'i':
				if(minnum > head->i){
					minnum = (double)head->i;
					minNode = head;
				}
				break;
			case 'f':
				if(minnum > head->f){
					minnum = head->f;
					minNode = head;
				}
				break;
			default:
				printf("error> The object passed as an argument to max, is not a real number\n");
				exit(0);
		}
		head = head->next;
	}

	return copyNode(minNode);
}
