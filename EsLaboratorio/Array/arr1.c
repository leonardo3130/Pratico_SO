#include <stdio.h>

char s[] = "ciao";

int main(int argc, char *argv[]) {
	// int v[10] = {1,2,3,4};
	int v[] = {[3 ... 9] = 33, [7] = 35, [12] = 12};
    //      0   1   2   3   4   5   6   7   8   9
    // v = {1   2   3   4   33  33  33  33  33  33}
    // v = {1   2   3   4   33  33  33  35  33  33}

	for (int i = 0; i < sizeof(v)/sizeof(*v); i++)
		printf("%d\t", v[i]);

    // 0 0 0 33 33 33 33 35 33 33 0 0 12 
	printf("\n");
}	