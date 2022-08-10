
#include    <xc.h>
#include  <pic16f887.h>
#include  "config.h"
#include "adc.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "LCD.h"
unsigned int value_adc = 0;//16 bits
unsigned char counter_digito = 0;
double temp = 0.0;
unsigned char lm35 = 0;


#define BETA 4090
#define resistance 10
//#define RL_VALUE (10) 
//#define RO_VALUE_CLEAN_AIR ( 9.83)
//#define RL_VALUE (10)//define the load resistance on the board, in kilo-ohms


void main(void) {
    //OSCCON = 0b01110001;
    OSCCON = 0x71;
    LCD_Init();
    TRISD = 0x00;
    TRISE=0x00; // salida puerto E
    TRISA0 = 1;// CONFIGU
    ANSEL = 0x01; //  Configura el Puerto como Entrada Analógica.
    TRISC1=1; //CONFIGURACION ENTRADA GAS
    TRISC0=1; //CONFIGURACION ENTRADA SHOCK
    TRISC3=1; //configuracion  Magic cup
    
//ADC CONFIGURACION SENSOR 1
    
    ADCON1bits.ADFM = 1; //  Justificación Derecha (modo-8bits).
    ADCON1bits.VCFG0 = 0; //  Selecciona Voltajes de Referencia (5v-0v).
    ADCON1bits.VCFG1 = 0; 
    
    ADCON0bits.CHS = 0b0000; //  Selecciona el Canal Analógico AN0.
    ADCON0bits.ADCS = 0b01; //  Tiempo de Co nversión Fosc/8.
    
    ADCON0bits.ADON = 1;//  Habilita el Módulo AD.
    
  
    __delay_ms(3000);
    
    while (1) {
        
        //camptura temperatura
        ADCON0bits.GO_DONE = 1; //  Inicia la COnversió AD.
        while (ADCON0bits.GO_DONE); //  Espera a que termine la conversión AD.
        value_adc = ADRESH; //  Lectura de valor AD.
        value_adc = (value_adc << 8) + ADRESL;
        temp = value_adc;
        
        //convertir temperatura
        long a = (long)(1023 - temp);
        float tempC = (float)(BETA /(log((1025.0 * 10/ a - 10) / 10) + BETA / 298.0) - 273.0);
        
       //imprime la temperatura
       
        char Temperature [10];
        sprintf(Temperature, "%d%cC", (int)tempC,0xdf );
        LCD_String_xy(1,0,"TEMP:");
        LCD_String(Temperature);
        LCD_String("  ");
         __delay_ms(2000);
        
        // SENSOR DE GAS
        if(PORTCbits.RC1==1) //CONFIGRUACION COMO SALIDA sensor de gas
        {
            
            LCD_String_xy(2,0,"Gas");
            
        }
        
        else if(PORTCbits.RC1==0){
            
            LCD_String_xy(2,0,"No gas");
            
        }
         __delay_ms(5000);
         LCD_Clear();
         
        //SENSOR SHOCK 
         if(PORTCbits.RC0 == 1) //CONFIGRUACION COMO SALIDA)
        {
            
            LCD_String_xy(1,0,"shock");
            
        }
        
        else if(PORTCbits.RC0 == 0){
            LCD_String_xy(1,0,"no shock");
        }
         
        
         //SENSOR Lesson Magic cup
         
            if(PORTCbits.RC3==1) //CONFIGRUACION COMO SALIDA)
        {
            
            LCD_String_xy(2,0," cup");
            
        }
        
        else if(PORTCbits.RC3==0){
            
            LCD_String_xy(2,0,"no cup");
            
        }
         
        __delay_ms(5000);
        LCD_Clear();
        //configuracion de los leds
        if (  tempC <20){
            PORTE=0x02;
        }
        
        else  if ( tempC > 21 & tempC<25){
                
                  PORTE=0x01;
            }
            else {
                PORTE=0x04;
           
            }
         __delay_ms(2000);
    LCD_String("  ");
        
    }
    
        
}
       

     