#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

#define MAX 1000

struct node{
	int i;			// to store integer value
	double f;		// to store floating value
	char *s;		// to store symbol
	char t;			// to specify whether the node has an integer or a float or a symbol
	struct node *next;	// to store the address of next node
};

struct number{
	int flag;
	int inum;
	double fnum;
};

int power(int a, int b);
void checkProgram(char *p);
struct node *newNode(char *w);
void print(struct node *current);
struct node *createList(char *p);
struct number *isNumber(char *w);
struct node *appendNode(struct node *h, char *w);

main()
{
	struct node *head = NULL;
	char program[MAX] = "(begin (define r 10) (* pi (* r r)))";

	checkProgram(program);	// checking syntax of the program

	head = createList(program);
	printf("\n\n");
	print(head);
	printf("\n\n");
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

struct node *createList(char *p)
{
	char word[100],*w;
	struct node *head = NULL;

	while(*p){
		w = word;
		if(isspace(*p))
			++p;
		else if(!isalnum(*p)){
			*w++ = *p++;
			*w = '\0';
			head = appendNode(head,word);
		}
		else{
			while(isalnum(*p))
				*w++ = *p++;
			*w = '\0';
			head = appendNode(head,word);
		}
	}
	return head;
}

struct node *appendNode(struct node *h, char *w)
{
	struct node *current = h;

	if(h == NULL)
		return newNode(w);

	while(current->next != NULL)
		current = current->next;
	current->next = newNode(w);

	return h;
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

	return new;
		
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

void print(struct node *current)
{
	while(current != NULL){
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
		}
		current = current->next;
	}
}
