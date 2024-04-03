// C Program to demonstrate fscanf
#include <stdio.h>

// Driver Code
int main()
{
	FILE* ptr = fopen("process1.dat", "r");
	if (ptr == NULL) {
		printf("no such file.");
		return 0;
	}
	char buf[10];
    int b;
    fscanf(ptr, "%s", buf);
    fscanf(ptr, "%s", buf);
    fscanf(ptr, "%s", buf);
	while (fscanf(ptr, "%d ", &b) == 1)
		printf("%d\n", b);
		

	return 0;
}
