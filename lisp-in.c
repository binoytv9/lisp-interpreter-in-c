#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

#define MAX 1000

struct node{
	char *word;		// to store the syntax word
	struct node *next;	// to store the address of next node
	struct node *block;	// to store address of sub-block
};

void checkProgram(char *p);
struct node *newNode(char *w);
void print(struct node *current);
struct node *createList(char *p);
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
	struct node *h = NULL;

	h = (struct node*)malloc(sizeof(struct node));
	h->word = strdup(w);
	h->next = NULL;

	return h;
}

void print(struct node *current)
{
	while(current != NULL){
		printf(" %s",current->word);
		current = current->next;
	}
}
