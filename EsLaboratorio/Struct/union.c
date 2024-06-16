#include <stdio.h>

// o si popola uno o l'al
union intch {
	int i;
	struct {
		char c0;
		char c1;
		char c2;
		char c3;
	};
};

int main(int argc, char *argv[]) {
	union intch ic = {.c1 = 1, .c2 = 2};
	//ic.i = 0x01020304;
	printf("%d %d %d %d\n", ic.c0, ic.c1, ic.c2, ic.c3);
}