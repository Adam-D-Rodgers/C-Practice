#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

#define MAX_LINE_LEN 1000

typedef struct node_s {
	char *str;
	struct node_s *prev;
	struct node_s *next;
} node_t;

typedef struct llist_s {
	node_t *first;
	node_t *last;
} llist_t;

node_t *new_node(char *);
void insert_node(llist_t *, char *);
void walk_forward(llist_t *);
void walk_reverse(llist_t *);
void free_list(llist_t *);

	node_t *
new_node(char *str)
{
	// 1. Allocate a new node
	// 2. Assign values to all of its data members.
	node_t * node = malloc(sizeof(node_t));
	node->str = calloc(strlen(str) + 1, sizeof(char));
	strcpy(node->str, str);
	node->prev = NULL;
	node->next = NULL;

	// Leaving these in your code will help make sure you've correctly
	// allocated a new node and assigned values to its data members.
	// alocate and assign before these statements.
	assert(node != NULL);
	assert(node->next == NULL);
	assert(node->prev == NULL);
	//assert(node->str != str);

	// 3. return the new node
	return node;
}

	void
insert_node(llist_t *list, char *str)
{
	// 1. Allocate a new node by calling new_node()
	node_t * node = new_node(str);

	// 2. Insert the new node into the ordered linked list at the correct
	//    location
	if(list->first == NULL){
		list->first = list->last = node;
		return;
	}
	node_t * pos = list->first;

	if(strcmp(str, list->first->str) < 0){
		list->first->prev = node;
		node->next = list->first;
		list->first = node;
		return;
	}	

	//Find the position that would be inserted
	while(pos->next != NULL && strcmp(str, pos->next->str) > 0){
		pos = pos->next;
	}

	if(pos->next) pos->next->prev = node;
	node->next = pos->next;
	pos->next = node;
	node->prev = pos;
	if(!node->next) list->last = node;
}

	void
walk_forward(llist_t *list)
{
	// 1. Print "--- forward ---\n"
	printf("--- forward ---\n");
	// 2. Walk forward through the list printing the string for each
	node_t * node = list->first;

	while(node != NULL){
		printf("%s\n", node->str);
		node = node->next;
	}
	//    node in the list. These should all be in increasing order.
	// 3. Print a newline
	printf("\n");
}

	void
walk_reverse(llist_t *list)
{
	// 1. Print "--- reverse ---\n"
	printf("--- reverse ---\n");
	// 2. Walk backward through the list printing the string for each
	//    node in the list. These should all be in decreasing order.

	node_t * node = list->last;

	while(node != NULL){
		printf("%s\n", node->str);
		node = node->prev;
	}
	// 3. Print a newline
	printf("\n");
}

	void
free_list(llist_t *list)
{
	// 1. walk through the list and free all allocated memory
	node_t * node = list->first;
	while(node){
		list->first = list->first->next;
		free(node->str);
		node->str = NULL;
		free(node);
		node = list->first;
	}
	list->last = NULL;

	// Leaving these in your code will make sure you've set the
	// first and last pointers to NULL. It does not make sure you've
	// deallocated all the memory. Free all memory before the following
	// statements.
	assert(list->first == NULL);
	assert(list->last == NULL);
}

int
//main(int argc, char *argv[])
main(void)
{
	llist_t list = {NULL, NULL};

	//#define TESTING
#ifdef TESTING
	// This chunk of code will give you a short test to make sure
	// you can insert nodes into the ordered linked list correctly.
	// It does NOT read any input from stdin.

	insert_node(&list, "y"); //y,b,x,a,c,z
	insert_node(&list, "b");
	insert_node(&list, "x");
	insert_node(&list, "a");
	insert_node(&list, "c");
	insert_node(&list, "z");

	assert(strcmp(list.first->str, "a") == 0);
	assert(strcmp(list.first->next->str, "b") == 0);
	assert(strcmp(list.first->next->next->str, "c") == 0);
	assert(strcmp(list.first->next->next->next->str, "x") == 0);

	assert(strcmp(list.last->str, "z") == 0);
	assert(strcmp(list.last->prev->str, "y") == 0);
	assert(strcmp(list.last->prev->prev->str, "x") == 0);
	assert(strcmp(list.last->prev->prev->prev->str, "c") == 0);
#else // TESTING
	char buf[MAX_LINE_LEN] = {0};

	// 1. Iterate through the input (stdin) and insert nodes into the
	//    ordered linked list. No input string will exceed 1000 characters
	//    in length
	while(!feof(stdin)){
		if(fgets(buf, MAX_LINE_LEN, stdin) != NULL){
			buf[strlen(buf) - 1] = '\0';
			insert_node(&list, buf);
			memset(buf, 0, MAX_LINE_LEN);
		}
	}

#endif // TESTING

	// 2. Walk forward through the list, by calling walk_forward()
	walk_forward(&list);
	// 3. Walk backward through the list, by calling walk_reverse()
	walk_reverse(&list);
	// 4. Free all allocated memory, by calling free_list()
	free_list(&list);	


	// After you've freed all the memory, these will make sure the
	// first and last pointers are set to NULL.
	assert(list.first == NULL);
	assert(list.last == NULL);

	return EXIT_SUCCESS;
}
