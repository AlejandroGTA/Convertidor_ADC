#include <18F4620.h>
#device adc=10
#include <stdio.h>
#include <stdlib.h>
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, /*NOMCLR,*/ STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
long valorRango(float value);
float n1 =0, n2 =0;
float  channel1=1, channel2=0;

#INT_AD
void isr_adc(){
   set_adc_channel(0);
   channel1 = read_adc();
   set_adc_channel(1);
   channel2 = read_adc();
}

#INT_TIMER0
void isr_timer0(){
   set_timer0(1);
}

void main(void) {
   setup_oscillator(OSC_16MHZ);
   setup_adc_ports(AN0_TO_AN1);
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_timer_0(RTCC_DIV_256 | RTCC_8_BIT);
   set_timer0(1);
   
   enable_interrupts(INT_AD); 
   enable_interrupts(GLOBAL); 
   enable_interrupts(INT_TIMER0);
   
   set_tris_b(0x0);
   set_tris_e(0b0001);
   set_tris_c(0x0);
   set_tris_d(0x0);
   
   long dato=0;
while (1) { 
   
   read_adc(ADC_START_ONLY);
   
   n1 = (channel1*5)/1024;
   dato = valorRango(n1);
   output_c((long)dato);
   output_d((long)dato >> 9);   
   
   n1 = (channel2*5)/1024;
   dato = valorRango(n1);
   
   output_b((long)dato);
   output_e((long)dato >> 9);
}
}

long valorRango(float value){
   long num=0;
   if(value >= 0.0000 && value <= 0.500){
      num = 1;
   }  
   if(value >= 0.5000 && value <= 1.000){
      num = 2;
   }
   if(value >= 1 && value <= 1.5000){
      num = 4;
   }
   if(value >= 1.5 && value <= 2.0){
      num = 8;
   }
   if(value >= 2.0 && value <= 2.5){
      num = 16;
   }
   if(value >= 2.5 && value <= 3.0){
      num = 32;
   }
   if(value >= 3.0 && value <= 3.5){
      num = 64;
   }
   if(value >= 3.5 && value <= 4.0){
      num = 128;
   }
   if(value >= 4.0 && value <= 4.5){
      num = 512;
   }
   if(value >= 4.5 && value <= 4.999){
      num = 1024;
   }
   return num;
}
