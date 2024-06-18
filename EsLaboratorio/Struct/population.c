#include <stdio.h>

struct point {
    int x;
    int y;
};

typedef struct point P;

void printpoint(struct point p){
	printf("%d %d\n", p.x, p.y);
}

// per il puntatore si usa ->
void printpointpoint(struct point *p){
	printf("%d %d\n", p->x, p->y);
}


int main(int argc, char *argv[]) {
    // se un termine non è inizializzato prende NULL e va a 0

	struct point p0 = {3,4};
    // struct point p0 = {.y = 4};     // 0, 4
	P p1;

	p1 = p0;
	printf("%d %d\n", p1.x, p1.y);

    int xx = 42;
	printpoint((P){.y = 33, .x = xx});
	printpointpoint(&(P){.x = xx, .y = 34});    
}