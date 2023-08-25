#include "float-2-hex.h"


int main (int argc, char * argv[]) {
	double dinput;
	float finput;	
	unsigned long long int dhex = 0;
	unsigned int fhex = 0;

	char input[200] = {0};
	char * val = NULL;
	int fd = 0;

	{
		int opt = 0;

		while ((opt = getopt(argc, argv, GETOPT_STRING)) != -1) {
			switch (opt) {
				case 'f':
					break;
				case 'd':
					fd = 1;
					break;
				case 'H':
					printf("Usage: ./float-2-hex [OPTION ...]\n");
					printf("\t-f convert the input into floats for hex output (this is the default)\n");
					printf("\t-d convert the input into doubles for hex output\n");
					printf("\t-H display this help message and exit\n");
					break;
				default: 
					break;
			}
		}
	}

	val = fgets(input, 200, stdin);
	input[strlen(input) - 1] = '\0';
	while(val != NULL){
		sscanf(input, "%lf", &dinput);
		dhex = *(unsigned long long int*)&dinput;
		finput = (float) dinput;
		fhex = *(unsigned int*) &finput;

		if(fd == 1){
			printf("%-40s\t%.16le\t%.16lf\t0x%016llx\n", input, dinput, dinput, dhex);
		}
		else {
			printf("%-40s\t%.10e\t%.10f\t0x%08x\n", input, finput, finput, fhex);
		}
		val = fgets(input, 200, stdin);
		input[strlen(input) - 1] = '\0';
	}
}

