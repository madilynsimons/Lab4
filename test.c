#include <stdio.h>
#include <stdlib.h>

int theSame()
{
	// if all 1s, specialized
	// if all 0s, denormalized
	// else normalized

	int i = 0;
	int next = 1;

	char string[] = "hhhhhhhhhh";
	char string2[] = "hhhhhh2";
	char string3[] = "kshdfkjhsdj";

	// check to see if they're all the same

	while(string[next] == string[i] & string[next] != '\0')
	{
		next++;
		i++;
	}
	if(string[i] == 'h') return 1;

	return 0;
}

int main() {
    if(theSame() == 1) printf("TRUE\n");
    else printf("FALSE\n");
}