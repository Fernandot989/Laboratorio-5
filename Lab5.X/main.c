/******************************************************************
Universidad del Valle de Guatemala
IE2023 Programación de Microcontroladores
Autor: Fernando Donis
Compilador: PIC-AS (v2.36), MPLAB X IDE (v6.00)
Proyecto: Laboratorio 4
Hardware: PIC16F887
Creado: 24/08/2022
Última Modificación: 17/09/2022
******************************************************************/

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic16f887.h>
#define _XTAL_FREQ 50000
/**************************************************
 Variables
 ***************************************************/
int ADC1, ADC2;
unsigned char i;
/**************************************************
 Prototipos de funciones
 ***************************************************/
void setup (void);
void setupintosc(void);
void setupADC (void);
void setupPWM (void);
void setupPWM2 (void);
/**************************************************
 Código Principal
 ***************************************************/

void main(void) {
    setup();
    setupintosc();
    setupPWM();
    setupPWM2();
    setupADC();
    while(1){
/**************************************************
 Primer ADC activa PWM del puerto RC2
 ***************************************************/
        ADCON0bits.CHS = 0b0000;           //Se establece el primer el puerto AN0 para convertir
        ADCON0bits.GO = 1;                 
        while(ADCON0bits.GO == 1){;}       
        ADIF = 0;
        ADC1 = ADRESH; 
        i = ADC1/16;                        //ADC se divide en 16
        if(i > 7 && 16 > i){                //Si se mantiene en 7 y 16 este se ejecutará
            CCPR1L = i;                     //CCPR1L es igual al valor de "i"
            __delay_ms(100);   
        }else {i = i;}
 /**************************************************
 Segundo ADC activa PWM2 del puerto RC3
 ***************************************************/
        ADCON0bits.CHS = 0b0001;           //Se establece AN1 como puerto a convertir
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO == 1){;}
        ADIF = 0;
        ADC2 = ADRESH;
        i = ADC2/16;                        //Se establece el mismo parametró qu eel anterior
        if(i > 7 && 16 > i){
            CCPR2L = i;
            __delay_ms(100);   
        }else {i = i;}
    }
    return;
}

void setup (void) {
    ANSELH = 0;
    TRISB = 0;
    PORTB = 0;
}

void setupintosc(){
    OSCCONbits.IRCF = 0b011;
    OSCCONbits.SCS = 1;
}

void setupPWM(void){
    TRISCbits.TRISC2 = 1;
    PR2 = 155;
    CCP1CONbits.P1M = 0b00;
    CCP1CONbits.CCP1M = 0b1100;
    CCP1CONbits.DC1B = 0b11;
    CCPR1L = 11;
    TMR2IF = 0;
    T2CONbits.T2CKPS = 0b11;    
    TMR2ON = 1;
    while(!TMR2IF);
    TRISCbits.TRISC2 = 0;
}

void setupPWM2(void){
    TRISCbits.TRISC1 = 1;
    PR2 = 155;
    CCP2CONbits.CCP2M = 0b1111;
    CCP2CONbits.DC2B1 = 0b1;
    CCP2CONbits.DC2B0 = 0b1;
    CCPR2L = 11;
    TMR2IF = 0;
    T2CONbits.T2CKPS = 0b11;    
    TMR2ON = 1;
    while(!TMR2IF);
    TRISCbits.TRISC1 = 0;
}

void setupADC(void){
    TRISAbits.TRISA0 = 1;
    ANSELbits.ANS1 = 1;
    
    TRISAbits.TRISA1 = 1;
    ANSELbits.ANS1 = 1;
    
    TRISAbits.TRISA2 = 1;
    ANSELbits.ANS2 = 1;
    
    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADCS0 = 1;
    
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;
    
    ADCON1bits.ADFM = 0;
    
    ADCON0bits.ADON = 1;
    __delay_us(100);
}
