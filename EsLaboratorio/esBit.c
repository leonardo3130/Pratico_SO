// #include <stdio.h>
// char c = 1 << 7;
// unsigned char d = 1 << 7;

// int main() {
// 	printf("%d\n", c >> 1);
// 	printf("%d\n", d >> 1);
// 	printf("%08hhb\n", c);
// 	printf("%08hhb\n", d);
// 	printf("%08hhb\n", c >> 1);
// 	printf("%08hhb\n", d >> 1);
// }

// C Program to demonstrate use
// of left shift  operator
#include <stdio.h>
 
int main()
{
    // a = 5(00000101), 
    // b = 9(00001001)
    unsigned char a = 5, b = 9;
 
    // The result is 00000010 => 2
    printf("a >> 1 = %d\n", (a >> 1));
 
    // The result is 00010010 => 18
    printf("b << 1 = %d\n", (b << 1));

    // The result is 00000001 => 1
    printf("a AND b = %d\n", (a & b));

    // The result is 00001101 => 13
    printf("a OR b = %d\n", (a | b));

    // The result is 00001100 => 12
    printf("a XOR b = %d\n", (a ^ b));

    // The result is 11111010 => 250
    printf("~a = %d\n", (unsigned char) ~a);

    // The result is 01111101 => 246
    printf("~a >> 1 = %d\n", (unsigned char) (~b) >> 1);

    // The result is 00000001 => 1
    printf("a AND b = %d\n", (a && b));

    // The result is 00000001 => 1
    printf("a OR b = %d\n", (a || b));

    // The result is 00000000 => 0
    printf("!a = %d\n", !a);


    return 0;
}