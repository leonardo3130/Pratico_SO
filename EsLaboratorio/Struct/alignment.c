#include <stdio.h>

struct A {
	char a1;    // sizeof(char) == 1
	long a2;    // size of(long) == 8
	char a3;    // sizeof(char) == 1
}__attribute__((packed));   // compatta la memoria 

struct B {
	char a1;    // sizeof(char) == 1
	long a2;    // size of(long) == 8
	char a3;    // sizeof(char) == 1
};      // di fatto qui vengono usati 24 byte

struct C {
	char b1;
	char b2;
	long b3;
};

int main() {
	printf("%d %d %d\n", sizeof(struct A), sizeof(struct B), sizeof(struct C));
}