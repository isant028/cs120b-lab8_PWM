/*	Author: Isabella Santiago
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 8  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * DEMO LINK: 
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
    static double current_frequency;

    if (frequency != current_frequency) {
        if(!frequency)
            TCCR3B &= 0x08;
        else
            TCCR3B |= 0x03;

        if(frequency < 0.954)
            OCR3A = 0xFFFF;
        else if (frequency > 31250)
            OCR3A = 0x0000;
        else
            OCR3A = (short) (8000000 / (128 * frequency)) - 1;
        
        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;

}

double seq[8]= {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};// note sequence C4, D, E, F, G, A, B, C5

enum state_sounds {start, wait, inc, inc_wait, dec, dec_wait, on, on_wait} state_sound;
unsigned char power = 0x00;
unsigned char decr = 0x00;
unsigned char incr = 0x00;
unsigned char i =0x00; //for loop
unsigned char check; //checks if the speaker is on or off to play sound when incrementing/dec it
void Tick_sound(){
    
    switch(state_sound){
        case start:
            state_sound = wait; 
            break; 
        
        case wait:
            if(power){ //if first button is pressed, go to on/off state
                state_sound = on;
            }
            else if(decr){ // if 2bd button is pressed, go to decr state
                state_sound = dec; 
            }
            else if(incr){ //if 3rd button is pressed go to incr state
                state_sound = inc; 
            }
            else{ //if no butttons are pressed stay here
                state_sound = wait;  
            }
            break;

        case inc:
            state_sound = inc_wait; 
            break;

        case inc_wait:
            if(incr){ //while incr button is pressed stay here
                state_sound = inc_wait;
            }
            else{ //any other button is pressed in this state go to wait 
                state_sound = wait; 
            }
            break;

        case dec:
            state_sound = dec_wait;
            break;

        case dec_wait:
            if(decr){ // while decr button is pressed stay here
                state_sound = dec_wait;
            }
            else{ //if not go to wait state 
                state_sound = wait; 
            }
            break;

        case on:
            state_sound = on_wait; 
            break;
        
        case on_wait:
            if(power){ //stay here when power button is pressed
                state_sound = on_wait;
            }
            else{ //else go to wait state
                state_sound = wait; 
            }
            break; 
        
    }
    switch(state_sound){
        case start:
            set_PWM(0);
            break; 
        
        case wait:
            break;

        case inc:
            if(i <0x07){ //7  since there are 8 notes and start at 0 and always even if on/off
                i++; //increments 
            }
            if(check ==0x01){
                set_PWM(seq[i]);
            }
            break;

        case inc_wait:
            break;

        case dec:
            if(i > 0){ //does not go below 0 nad alwayd even iff on or off
                i--;//decrements 
            }
            if(check ==0x01){
                set_PWM(seq[i]);
            }
            break;
            break;

        case dec_wait:
            break;

        case on:
            if(check == 0x00){ //basically if off, turn on and play sound
                check = 0x01; //set check to one showing it is on 
                set_PWM(seq[i]);
            }
            else{
                check = 0x00; //if already on turn it off
                set_PWM(0); //do not play sound if it is off
            }
            break;
        
        case on_wait:
            break; 
        
    }

}
int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    PWM_on();
    state_sound = start; 
	while (1) {
        power = ~PINA & 0x01; //first button is turn on/off
        decr = ~PINA & 0x02; //second button is to decrement note
        incr = ~PINA & 0x04; //third button is to increment button 
	    Tick_sound();
	}
    return 1; 
}	

