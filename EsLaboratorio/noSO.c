typedef unsigned char uint8_t; /**< Definizione del tipo di dato uint8_t come unsigned char */

#define DDRC (*(volatile uint8_t *) 0x27) /**< Registro di direzione del Port C */
#define PORTC (*(volatile uint8_t *) 0x28) /**< Registro di output del Port C */

int main(void)
{
    volatile int i; 
    DDRC |= 1 << 5; 
    for (;;) {
        for (i=0; i < 32767; i++)
            ;
        PORTC ^= 1 << 5; 
    }
}
