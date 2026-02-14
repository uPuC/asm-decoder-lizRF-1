/******************************************************************************
Código de ejemplos, p1 1291061, Lizzette Ramirez Fernandez 
13 Febrero 2026
******************************************************************************/

#include <stdio.h>
#include <inttypes.h>
/*
    Ejercicios de enmascaramientos.
    Se busca que imprima el valor que está comentado,
    alterando únicamente el valor anterior utilizando los operadores bitwise.
    Todas las conversiones deberían ser posibles con una sola operación.
*/
int main()
{
    uint16_t a = 0;
    uint32_t b = 0x1155;
    //printf("\na = %X", a);
//  Como ejemplo se da el analisis de la primera operacion

//1 B0B0
//       hex           bin
//  a = 0000 = 0000 0000 0000 0000  (valor original)
//      B0B0 = 1011 0000 1011 0000  (valor que se desea)
//  Observamos que todas las diferencias radica en que los bits tienen que estar habilitados,
//  por lo tanto usaremos el operador OR (|) para convertir esos bits en '1's

//    0000 0000 0000 0000  (valor original)
// OR 1011 0000 1011 0000  (mascara para lograrlo) = (1<<15) | (3<<12) | (1<<7) | (3<<4)
//------------------------------------------------
//    1011 0000 1011 0000  (valor que se desea)
    printf("\n1. B0B0 \n");
    printf("Inicio:     a = %X", a);
    a |= (1<<15) | (3<<12) | (1<<7) | (3<<4);
    printf("\nResultado:  a = %X", a);
    printf("\n____________________________________\n");

//2 BABA


    printf("\n2. BABA \n");
    printf("Inicio:     a = %X", a);
    a |= (1<<11) | (1<<9) | (1<<3) | (1<<1);
    printf("\nResultado  a = %X", a);
    printf("\n____________________________________\n");

//3 BEBE


    printf("\n3. BEBE \n");
    printf("Inicio:     a= %X", a);
    a |= (1<<10)|(1<<2);
    printf("\nResultado:  a = %X", a);
    printf("\n____________________________________\n");

//4 FE00
    printf("\n4. FE00\n");
    printf("Inicio:     a = %X", a);
    a = (a&(0xF<<12) | (1<<14) | (0xE<<8));
    printf("\nResultado:  a = %X", a);

    printf("\n____________________________________\n");


//5 FEAA

    printf("\n5. FEAA \n");
    printf("Inicio:     a = %X", a);

    a |= (0xF<<15)|(0xE<<11)|(1<<7) | (1<<5) | (1<<3) | (1<<1);

    printf("\nResultado:  a = %X", a);
    printf("\n____________________________________\n");

//6 0155

    printf("\n6. 0155\n");
    printf("\Inicio:     a = %X", a);

    a = ~ a;

    printf("\nResultado:  a = %X", a);
    printf("\n____________________________________\n");


//7 0001

    printf("\n7. 0001\n");
    printf("\Inicio:     a = %X", a);
    a &= (1);
    printf("\nResultado:  a = %X", a);
    printf("\n____________________________________\n");

//8 0100

    printf("\n8. 0100\n");
    printf("\Inicio:     a = %X", a);
    a = (a| (1<<8))&(1<<8);

    printf("\nResultado:  a = %X", a);
    printf("\n____________________________________\n");


    /************************************
     * Ejercicios de apuntadores
     ************************************/

    b = ((~b) << 16) | b;
    //uint16_t *ptr16 = &b;
    //uint8_t *ptr8 = &b;

    uint16_t *ptr16 = (uint16_t *)&b;
    uint8_t  *ptr8  = (uint8_t *)&b;


//9 DEADBEEF
    //para poner 0 en los bits 4,8,17,29
    printf("\n9. DEADBEEF\n");
    printf("Inicio:     b = %X", b);
    b= ((b& ((0xD<<28) | (0xF<<24) | (0xF<<20) | (0xD<<16) | (0xF<<12) | (0xE<<4) |(0xE<<0)))
         | ((1<<28) | (5<<16) | (0xA<<12) | (0xE<<8) | (0xA<<4) | (0xB<<0)));
    printf("\nResultado:  b = %X", b);
    printf("\n____________________________________\n");




    /************************************
     * Ejercicios de structs con campos de bits
     ************************************/

    // Crear una esturctura de 32 bits y mappearla sobre la variable 'b'
    //10 BADDCODE

    printf("\n10. BADDCODE\nEstructura de 32 bits\n");
    printf("Inicio:     b = %X", b);



    //struct de 32 bits dividida como su parte alta y baja
    struct estr32b{
        uint16_t bl;
        uint16_t bh;
    };

    //union que se interpreta de distintas formas pero comparte el mismo espacio de memoria
    union{
        uint32_t entB;   //ve los 32 bits completos
        struct estr32b partes;
    }u;

    /**es como decir: tengo 4 bytes, pueden verse como un entero de 32 bits o como dos enteros de 16 bits"*/

    //struct estr32b *ptrB;
    //ptrB = &b;

    //struct estr32b *ptrB;
    //ptrB = (struct estr32b *)&b;


    b= (b&((0xB<<28) |(0XB<<24) | (0XD<<20) | (0xF<<16) | (0xC<<12) | (0xD<<4)|(0XE<<0)) | ((2<<28)| (5<<20) | (4<<12) | (1<<4)));
    //printf("\nResultado:  B = %X%X", ptrB->bh, ptrB->bl);
    u.entB= b;

    // en memoria se guarda como: DE CO DD BA

    printf("\nResultado:  B = %X%X", u.partes.bh, u.partes.bl);

    printf("\n____________________________________\n");


    return 0;
}
