/******************************************************************************
Prac 2 - AVR ASM OpCode Decoder
Lizzette Ramírez Fernández 1291061
*******************************************************************************/

#include <stdio.h>
#include <inttypes.h>

const uint8_t flash_mem[] ={ /**simula la memoria*/
    0x00, 0x24, //0x2400 -> CLR
    0xA0, 0xE0, //0xE0A0 -> LDI
    0xB2, 0xE0, //0xE0B2 -> LDI
    0x0D, 0x91, //0x910D -> LD X+
    0x00, 0x30, //0x3000 -> CPI
    0xE9, 0xF7, //0xF7E9 -> BRNE
    0x11, 0x97, //0x9711 -> SBIW
    0xC0, 0xE0, //0xE0C0 -> LDI

    0xD2, 0xE0, //0xE0D2 -> LDI
    0x09, 0x91, //0x9109 -> LD Y+
    0x1E, 0x91, //0x911E -> LD -X
    0x01, 0x17, //0x1701 -> CP
    0x51, 0xF4, //0xF451 -> BRNE
    0x0A, 0x2F, //0x2F0A -> MOV
    0x0A, 0x95, //0x950A -> DEC
    0x1C, 0x2F, //0x2F1C -> MOV

    0x01, 0x17, //0x1701 -> CP
    0xB9, 0xF7, //0xF7B9 -> BRNE
    0x0B, 0x2F, //0x2F0B -> MOV
    0x1D, 0x2F, //0x2F1D -> MOV
    0x01, 0x17, //0x1701 -> CP
    0x99, 0xF7, //0xF799 -> BRNE
    0x03, 0x94, //0x9403 -> INC
    0x00, 0x00  //0x0000 -> NOP
    };

const uint16_t inst16_table[] = { /**almacena las instrucciones en su forma 'básica'*/
    {0x0}, //NOP
    {0x2400},  //CLR
    {0xE000}, //LDI
    {0x900D}, // LD +X
    {0x900E}, // LD -X
    {0x9009}, //LD Y+
    {0x3000}, //CPI
    {0xF000}, //BRNE
    {0x9700}, //SBIW
    {0x1000}, //CP
    {0x2000}, //MOV
    {0x900A}, //DEC
    {0x9003} //INC

};

/**constantes que representan a las instrucciones del arreglo.*/
enum{
    e_NOP, e_CLR, e_LDI, e_LDX, e_LDx, e_LDY, e_CPI, e_BRNE, e_SBIW, e_CP, e_MOV, e_DEC, e_INC
};


// Op Code struct
typedef union {
    uint16_t op16; // e.g.: watchdog, nop
    struct{
        uint16_t d10:10;
        uint16_t op6:6;
    }type1; // CLR
    struct{
        uint16_t k4_1:4;
        uint16_t d4:4;
        uint16_t k4_2:4;
        uint16_t op4:4;
    }type2; //LDI, CPI
    struct{
        uint16_t op4:4;
        uint16_t d5:5;
        uint16_t op7:7;

    }type3; //LDX+, LD-X, LDY+, DEC, INC
    struct{
        uint16_t op3:3;
        uint16_t k7:7;
        uint16_t op6:6;
    }type4; //BRNE
    struct{
        uint16_t k4_1:4;
        uint16_t d2:2;
        uint16_t k2_2:2;
        uint16_t op8:8;
    } type5;//SBIW
    struct{
        uint16_t r4_1:4;
        uint16_t d5:5;
        uint16_t r1_2:1;
        uint16_t op6:6;
    }type6; //CP, MOV

} Op_Code_t; //fin de la union


int main()
{
    Op_Code_t *instruction;
    printf("- Practica 2: AVR OpCode -\n");
    // Decode the instructions by cycling through the array
    // procesa 2 bytes, es decir 1 instrucion
    for (uint8_t idx = 0; idx < sizeof(flash_mem); idx+=2)
    {
        instruction = (Op_Code_t*) &flash_mem[idx]; //toma la direccion de idx y lo interpreta como un OpCode_t
        if (instruction->op16 == inst16_table[e_NOP])
        {
            printf("NOP\n");
        }
        else if((instruction -> op16 &0xFC00)==inst16_table[e_CLR])
        {
            uint8_t d= instruction->type1.d10;
            //uint8_t op= instruction->type1.op6;
            printf("CLR R%d\n", d);
        }else if((instruction -> op16 &0xF000)== inst16_table[e_LDI]){
            uint8_t d = 16 + instruction->type2.d4; //se le suma 16 porque LDI trabaja con el R16 en adelante
            uint8_t k = (instruction->type2.k4_2 << 4) | instruction->type2.k4_1; //como K se divide en 2 mitades, se juntan con OR una vez desplazada la parte MSB
            printf("LDI R%d, 0x%d\n", d, k);
        }else if((instruction-> op16 & 0xFE0F)== inst16_table[e_LDX]){
            uint8_t rd= instruction->type3.d5;
            printf("LD R%d, X+\n", rd);
        }else if((instruction->op16 & 0xFE0F )== inst16_table [e_LDx]){
            uint8_t rd = instruction->type3.d5;
            printf("LD R%d, -X\n", rd);
        }else if((instruction->op16 & 0xFE0F)== inst16_table [e_LDY]){
            uint8_t rd = instruction->type3.d5;
            printf("LD R%d, Y+\n", rd);
        } else if((instruction->op16 & 0xF000)== inst16_table[e_CPI]){
            uint8_t d = 16+ instruction->type2.d4;
            uint8_t k= (instruction->type2.k4_2<<4  | instruction->type2.k4_1);
            printf("CPI R%d, 0x%d\n", d, k);
        } else if((instruction->op16 & 0xF000)== inst16_table [e_BRNE]){
            int8_t k= instruction->type4.k7;
            if (k >= 64) k= k- 128; //corregir signo si el bit mas significativo de k esta activo, quiere decir que es numero negativo
            //printf("BRNE PC %X\n", k);
            printf("BRNE PC %d\n", k+1);
        }else if((instruction->op16 &0xFF00)== inst16_table[e_SBIW]){
            uint8_t d= (instruction->type5.d2 *2)+24;  //00 R24, 01 R26, 10 R28, 11 R30
            /*
             * si d=00, 00 (0d)*2=0+24=24
             * d=01, 01 (1d)*2=2+24=26
             * d=10, 10 (2d)*2=4+24=28
             * d=11, 11 (3d)*2=6+24=30
             */
            uint8_t k= (instruction->type5.k2_2<<4 | instruction->type5.k4_1);
            printf("SBIW R%d, %d\n", d, k);
        }else if((instruction->op16 &0xF000)== inst16_table[e_CP]){
            uint8_t d = (instruction->type6.d5);
            uint8_t k = (instruction->type6.r1_2 << 4) | instruction->type6.r4_1;
            printf("CP R%d, R%d\n",d, k);
        }
        else if((instruction->op16 &0xF000)== inst16_table[e_MOV]){
            uint8_t d= instruction->type6.d5;
            uint8_t r= (instruction->type6.r1_2 << 4) | instruction->type6.r4_1;
            printf("MOV R%d, R%d\n",d, r);
        }
        else if((instruction->op16 &0xF00F)== inst16_table[e_DEC]){
            uint8_t d= instruction->type3.d5;
            printf("DEC R%d\n", d);
        }
        else if((instruction->op16 &0xF00F)== inst16_table[e_INC]){
            uint8_t d= instruction->type3.d5;
            printf("INC R%d\n", d);
        }
        else{
            printf("Desconocido\n");
        }
    }
    return 0;
}
