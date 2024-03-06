/* 
 * File:   main.c
 * Author: nikgal-local
 *
 * Created on den 28 februari 2021, 17:33
 */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#define _XTAL_FREQ 8000000

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTRC oscillator; port I/O function on both RA6/OSC2/CLKO pin and RA7/OSC1/CLKI pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB2      // CCP1 Pin Selection bit (CCP1 function on RB2)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
/*
 * 
 */
unsigned int VOLT_MSB = 0;
unsigned int VOLT_LSB = 0;

unsigned int CURR_MSB = 0;
unsigned int CURR_LSB = 0;

int main(int argc, char** argv) {
    OSCCON = 0b01110100;    //Setup frequency of MCU to 8MHz 
    
    ADCON0 = 0b11001001;    //Setup analog to digital converter --|
    ADCON1 = 0b10000000;    //                                 <--|
    
    TRISB = 0b00110010;     //Setup RB0-RB7 as outputs
    TRISA = 0b11111111;     //Setup RA0-RA7 as inputs

    SSPCON = 0b00100100;    //Setup SPI fnctions of MCU --|
    SSPSTAT = 0b01000000;   //                         <--|
    RB0 = 0;
    RB3 = 0;
    
    while(1)    
    {   
        ADCON0 = 0b11001001;    //Setup analog to digital converter --|
        ADCON0bits.GO = 1;
        while (ADCON0bits.GO == 1);
        VOLT_MSB = ADRESH;
        VOLT_LSB = ADRESL;
        ADCON0 = 0b11000001;    //Setup analog to digital converter --|
        ADCON0bits.GO = 1;
        while (ADCON0bits.GO == 1);
        CURR_MSB = ADRESH;
        CURR_LSB = ADRESL;
        
        if (BF == 0)
        {
            while(BF == 0);
            if (SSPBUF == 152)
            {
                SSPBUF = VOLT_MSB;
                while(BF == 0);
                SSPBUF = VOLT_LSB;
                while(BF == 0);
                
                SSPBUF = CURR_MSB;
                while(BF == 0);
                SSPBUF = CURR_LSB;
                while(BF == 0);
            }
            else if(SSPBUF == 95) //start charge
            {
                RB0 = 1;
                RB3 = 0;
                SSPBUF = 175;
                while(BF == 0);
            }
            else if(SSPBUF == 80) //stop charge
            {
                RB0 = 0;
                SSPBUF = 160;
                while(BF == 0);
            }
            else if(SSPBUF == 127) //start discharge
            {
                RB0 = 0;
                RB3 = 1;
                SSPBUF = 191;
                while(BF == 0);
            }
            else if(SSPBUF == 112) //stop discharge
            {
                RB3 = 0;
                SSPBUF = 176;
                while(BF == 0);
            }
            else
            {
                SSPBUF = 0;
            }       
        }
        else
            SSPBUF = 0;
    }
return (EXIT_SUCCESS);
}

