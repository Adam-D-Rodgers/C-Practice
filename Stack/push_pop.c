#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <assert.h>

#include "push_pop.h"

// The valid commands:
//   status
//   push %reg
//   push $val
//   pop %reg
int is_verbose = 0;
FILE *ofile = NULL;

static unsigned long stack_bot = DEF_STACK_BOT; // the high address for the stack
static unsigned long stack_limit = DEF_STACK_LIMIT; // the low address for the stack
static unsigned long rsp = DEF_RSP; // the initial value for the rsp register
static unsigned long *stack = NULL; // points to the bytes for the stack
static unsigned long registers[REG_RDX + 1] = { // the general purpose registers
						// These are just some default values.
	REG_RAX + 20
		, REG_RBX + 30
		, REG_RCX + 40
		, REG_RDX + 50
};

static void pop(char *);
static void push_reg(char *);
static void push_value(unsigned long int );

	static void
pop(char *reg)
{
	int i = 0;
	switch(reg[2]) {
		case 'a':
			i = 0;
			break;
		case 'b': 
			i = 1;
			break;
		case 'c': 
			i = 2;
			break;
		case 'd': 
			i = 3;
			break;
		default: 
			return;
	}
	registers[i] = stack[(stack_bot - rsp) / REG_SIZE]; 
	rsp += 8;
}

	static void
push_reg(char *reg)
{
	int i = 0;
	switch(reg[2]) {
		case 'a':
			i = 0;
			break;
		case 'b': 
			i = 1;
			break;
		case 'c': 
			i = 2;
			break;
		case 'd': 
			i = 3;
			break;
		default: 
			return;
	}

	if(rsp == stack_limit){
		fprintf(stderr, "Stack Overflow");
		return;
	}
	rsp -= 8;
	stack[(stack_bot - rsp) / REG_SIZE] = registers[i]; 
	return;
}

	static void
push_value(unsigned long int reg)
{
	if(rsp == stack_limit) {
		fprintf(stderr, "Stack Overflow");
		return;
	}
	rsp -= 8;
	stack[(stack_bot - rsp) / REG_SIZE] = reg; 
	return;
}


	int
main(int argc, char *argv[])
{
	FILE *ifile = stdin;
	char input[200] = {0};
	char * val = NULL;
	char * token = NULL;
	long int stack_size = 0;
	
	ofile = stdout;

	{
		int opt = 0;


		while ((opt = getopt(argc, argv, GETOPT_STRING)) != -1) {
			switch (opt) {
				case 'i':
					ifile = fopen(optarg, "r");
					if(!ifile) {
						fprintf(stderr, "FAILED TO OPEN FILE");
						return EXIT_FAILURE;
					}
					break;
				case 'o':
					ofile = fopen(optarg, "w");
					if(!ofile){ 
						fprintf(stderr, "FAILED TO OPEN FILE");
						return EXIT_FAILURE;
					}
					break;
				case 'b':
					stack_bot = strtoul(optarg, NULL, 16);
					break;
				case 'l':
					stack_limit = strtoul(optarg, NULL, 16);
					break;
				case 's':
					rsp = strtoul(optarg, NULL, 16);
					break;
				case 'v':
					is_verbose++;
					break;
				case 'h':
					printf("%s\n\toptions: %s\n", argv[0], GETOPT_STRING);
					printf("\ti: name of input file\n");
					printf("\to: name of output file\n");
					printf("\tb: stack_bottom (high address, as hex)\n");
					printf("\tl: stack limit (low address, as hex)\n");
					printf("\ts: beginning %%rsp value (within the stack, as hex)\n");
					printf("\tv  verbose output\n");
					printf("\th  help\n");

					exit(EXIT_SUCCESS);
					break;
				default:
					break;
			}
		}
	} 

	
	if(stack_bot <= stack_limit || stack_bot % 8 || stack_limit % 8 || rsp % 8 || rsp > stack_bot || rsp < stack_limit){
		fprintf(stderr, "ERROR ERROR ERROR\n");
		return EXIT_FAILURE;
	}
	stack_size = stack_bot - stack_limit + 0x8;
	stack = calloc(stack_size, REG_SIZE);

	val = fgets(input, 200, ifile);
	while(val != NULL) {
		input[strlen(input) - 1] = '\0';
		token = strtok(input, " \t");
		if(strcmp(token, "push") == 0){
			token = strtok(NULL, " \t");	
			if(token[0] == '$'){
				token[0] = '0';
				push_value(strtoul(token, NULL, 16));		
			}
			else if(token[0] == '%'){
				push_reg(token);
			}
		}
		else if (strcmp(token, "pop") == 0){ 
			token = strtok(NULL, " \t");
			pop(token);
		}
		else if (strcmp(token, "status") == 0){
			stack_status(stack, registers, rsp, stack_bot, stack_limit, ofile); 
		}
		else {
			fprintf(stderr, "ERROR ERROR ERROR\n");
		}
		val = fgets(input, 200, ifile);
	}	
	
	if(ofile) fclose(ofile);
	if(ifile) fclose(ifile);
	if(stack) {
		free(stack);
		stack = NULL;
	}
	
	return EXIT_SUCCESS;
}
