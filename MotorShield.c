#include <8051.h>

#define M1 P1_0
#define M2 P1_2
#define M3 P1_4
#define M4 P1_6

#define M1B P1_1
#define M2B P1_3
#define M3B P1_5
#define M4B P1_7

char fatia = 0;
char OFC = 0;

char vels[] = {6,3,5,7};
char rev[] = {0,0,0,0};
char select = 0;

char i;

unsigned  char Tab7SegCComum[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};
unsigned char ValorDisplay[]={0, 0}; /*selecionado, velocidade*/
unsigned char StatusDisplay=0;

char on = 1;

void ShowDisplay()
{
    switch (StatusDisplay)
    {
        case 0: 
            P0_7=0;
            P0_6=1;
            P2=Tab7SegCComum[ValorDisplay[0]];
            if(rev[select]) P2_7 = 1;
            StatusDisplay++;
            break;
        case 1:
            P0_6=0;
            P0_7=1;
            P2=Tab7SegCComum[ValorDisplay[1]];
            StatusDisplay=0;
            break;  
    }
}


/*
void Timer0() __interrupt (1) { 
    ShowDisplay();
}
*/


int getPWM(int desired){
    if(fatia >= desired) return 1;
    return 0;
}


void Timer0() __interrupt (1) {

	if(!on) return;

    OFC++;

    if(OFC>= 10){
        OFC = 0;
        fatia++;
        if(fatia > 9){
            fatia = 0;
        }
        
        for(i = 0; i < 4; i++){
            
            int saida = getPWM(vels[i]);
            if(rev[i]) saida = 1 - saida;
            switch(i){
                case 0:
                    M1 = saida;
                    M1B = 1-saida;
                    break;
                case 1:
                    M2 = saida;
                    M2B = 1-saida;
                    break;
                case 2:
                    M3 = saida;
                    M3B = 1-saida;
                    break;
                case 3:
                    M4 = saida;
                    M4B = 1-saida;
                    break;
            }
        }        
    }

    ValorDisplay[1]= select;
    ValorDisplay[0]= vels[select];
    ShowDisplay();   
}




char VarreLinha(char Coluna)
 {
    char retornaTecla=0;
    switch (Coluna)
    {
        case  0:
            if(P0_1==0) retornaTecla = '4';
            break;                                 
        case  1: 
            if(P0_0==0) retornaTecla = '2';
            if(P0_1==0) retornaTecla = '5';
            if(P0_2==0) retornaTecla = '8';
            break;
        case  2:  
            if(P0_1==0) retornaTecla = '6';
            if(P0_2==0) retornaTecla = '9';
            break;                                                            
    }
    return retornaTecla;
}

char LerTeclado()
{
    char i;
    char TeclaLida=0;
    for (i=0; i<3; i++)    /* Varre Coluna*/
    {          
        
        switch (i)
        {
        case 0: P0_3=0;
            if (TeclaLida==0) TeclaLida = VarreLinha(i);
            P0_3=1;
            break; 
        case 1: P0_4=0;
            if (TeclaLida==0) TeclaLida = VarreLinha(i);
            P0_4=1;
            break; 
        case 2: P0_5=0;
            if (TeclaLida==0) TeclaLida = VarreLinha(i);
            P0_5=1;
            break;     
        }
    }
    return TeclaLida;
}

void turnOff(){
	M1 = 1;
	M2 = 1;
	M3 = 1;
	M4 = 1;
	M1B = 1;
	M2B = 1;
	M3B = 1;
	M4B = 1;
	P2_0 = 0;
	P2_1 = 0;
	P2_2 = 0;
	P2_3 = 0;
	P2_4 = 0;
	P2_5 = 0;
	P2_6 = 0;
	P2_7 = 0;
}

void intKeypad(void) __interrupt(0){
    /*rev ^= 1;*/
    switch(LerTeclado()){
        case '4':
            select --;
            if(select < 0) select = 3;
            break;
        case '6':
            select ++;
            if(select > 3) select = 0;
            break;
        case '2':
            if(vels[select] < 9) vels[select]++;
            break;
        case '8':
            if(vels[select] > 0) vels[select]--;
            break;
        case '5':
            rev[select] ^= 1; 
            break;
        case '9':
        	on ^= 1;
        	turnOff();
        	break;
        default:
        	P1_7 ^=1;
    }
}

void main()
{ 
    
    TMOD = (TMOD & 0xF0) | 0x02;
    ET0 = 1;
    EX0 = 1;
    EA = 1;
    IT0 = 1;
    
    TH0=0x9C;
    TR0 = 1;
  
    while(1){
        
    }
}

