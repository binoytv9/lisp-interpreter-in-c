#include<stdio.h>

#define MAX 1000

struct node{
	char *word;		// to store the syntax word
	struct node *next;	// to store the address of next node
	struct node *block;	// to store address of sub-block
};

main()
{
	struct node *head = NULL;
	char program[MAX] = "(begin (define r 10) (* pi (* r r)))";

	head = createList(program);
	print(head);
}
