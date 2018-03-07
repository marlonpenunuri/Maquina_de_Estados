/*
 * File:   Calculadora.c
 * Author: Marlon
 *
 * Created on October 8, 2017, 5:53 PM
 */

#include <xc.h>
#include "comm.h"
#include "uart.h"
#include "setup_Up.h"


unsigned char result;
unsigned char dig1;
unsigned char dig2;
unsigned char dato1;
unsigned char dato2;
unsigned char byte1;
unsigned int result_bcd;
unsigned char num[3];


typedef enum{
  STATE0,
  STATE1,
}eStates;

void vfnState0(void);
void vfnState1(void);

unsigned char gbState;

  void (* const vfnapStateMachineFuncPtr[])(void) =  {
    vfnState0,
    vfnState1,
  };

void main(void){
	
	config();
	gbState = STATE0;
  
	while(1) {
		vfnapStateMachineFuncPtr[gbState]();
	}
}


void vfnState0(void){
    printf_UART ("Calculadora \n\r");
    
    for(unsigned char i=0;i<5;i++){ 
        num[i] = scanf_UART();
        if (num[i] != 0 ){
        putc_UART(num[i]);
        }
    }
    
    
    dato1=(num[0] & 0x0F);
    dato2=(num[1] & 0x0F);
    byte1 = dato1<<4 | dato2;
    dig1 = bcd2hex(byte1);
    
    
    
    dato1 = (num[3] & 0x0F);
    dato2 = (num[4] & 0x0F);
    byte1 = dato1<<4 | dato2;
    dig2 = bcd2hex(byte1);
    
    gbState = STATE1;
   
}


 
 void vfnState1(void){
     unsigned char result1;
     unsigned char result2;
     
    if (num[2] == '+'){
        result=(dig1) + (dig2); 
        result_bcd = hex2bcd(result);
        result1 = (result_bcd >> 8) & (0x00FF);
        result2 = (result_bcd) & (0x00FF);
        putc_UART('=');
        
       if (result1 != 0){
            ASCII_UART(result1);
        }
        ASCII_UART(result2);
        printf_UART("\n\r");
    }
    
    if (num[2] == '-'){
        if (dig1<dig2){
            result = (dig2)-(dig1); 
            result_bcd = hex2bcd(result);
            putc_UART('=');
            result1 = (result_bcd >> 8) & (0x00FF);
            result2 = (result_bcd) & (0x00FF);
            putc_UART('-');
            
            if (result1 != 0){
                ASCII_UART(result1);
            }
            ASCII_UART(result2);
            printf_UART("\n\r");  
        }
        else {
            result = (dig1)-(dig2); 
            result_bcd = hex2bcd(result);
            result1 = (result_bcd >> 8) & (0x00FF);
            result2 = (result_bcd) & (0x00FF);
            putc_UART('=');
            
            if (result1 != 0){
                ASCII_UART(result1);
            }
            ASCII_UART(result2);
            printf_UART("\n\r");
        }
    }
    if (num[2] == 'x'){
        result = (dig1)*(dig2); 
        result_bcd = hex2bcd(result);
        result1 = (result_bcd >> 8) & (0x00FF);
        result2 = (result_bcd) & (0x00FF);
        putc_UART('=');
        
        if (result1 != 0){
            ASCII_UART(result1);
        }
        ASCII_UART(result2);
        printf_UART("\n\r");
    }
    
    gbState=STATE0;  
 }