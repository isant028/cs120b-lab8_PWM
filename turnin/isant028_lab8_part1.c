/*	Author: Isabella Santiago
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 8  Exercise 1
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

enum state_sounds {start, wait, st_e, st_c, st_d, } state_sound;
unsigned char e = 0x00;
unsigned char c = 0x00;
unsigned char d = 0x00;
void Tick_sound(){
    switch(state_sound){
        case start:
            state_sound = wait; 
            break; 
        
        case wait:
            if(e && !c && !d ){ //if only 3rd button is pushed got to e sate
                state_sound = st_e;
            }
            else if(!e && c && !d ){ //if only  1st buttoin is pushed, go to c state
                state_sound = st_c;
            }
            else if(!e && !c && d ){ //if only 2nd button is pushed, go to d state 
                state_sound = st_d;
            }
            else{ // no buttons are pushed/ more than 1 button pushed go to wait state and no sound 
                state_sound = wait; 
            }
            break;

        case st_e:
            if(e && !c && !d ){ //if only 3rd button is pushed got to e sate
                state_sound = st_e;
            }
            else if(!e && c && !d ){ //if only  1st buttoin is pushed, go to c state
                state_sound = st_c;
            }
            else if(!e && !c && d ){ //if only 2nd button is pushed, go to d state 
                state_sound = st_d;
            }
            else{ // no buttons are pushed/ more than 1 button pushed go to wait state and no sound 
                state_sound = wait; 
            }
            break;
        
        case st_c:
            if(e && !c && !d ){ //if only 3rd button is pushed got to e sate
                state_sound = st_e;
            }
            else if(!e && c && !d ){ //if only  1st buttoin is pushed, go to c state
                state_sound = st_c;
            }
            else if(!e && !c && d ){ //if only 2nd button is pushed, go to d state 
                state_sound = st_d;
            }
            else{ // no buttons are pushed/ more than 1 button pushed go to wait state and no sound 
                state_sound = wait; 
            }
            break;

        case st_d:
            if(e && !c && !d ){ //if only 3rd button is pushed got to e sate
                state_sound = st_e;
            }
            else if(!e && c && !d ){ //if only  1st buttoin is pushed, go to c state
                state_sound = st_c;
            }
            else if(!e && !c && d ){ //if only 2nd button is pushed, go to d state 
                state_sound = st_d;
            }
            else{ // no buttons are pushed/ more than 1 button pushed go to wait state and no sound 
                state_sound = wait; 
            }
            break;
    }
    switch (state_sound)
    {
    case start:
        set_PWM(0);
        break;
    case wait:
        set_PWM(0);
        break;

    case st_e:
        set_PWM(329.63);
        break;

    case st_c:
        set_PWM(261.63);
        break;
    
    case st_d:
        set_PWM(293.66);
        break; 
        
    default:
        set_PWM(0);
        break;
    }

}
int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    PWM_on();
    state_sound = start; 
	while (1) {
        c = ~PINA & 0x01; //first button is c note
        d = ~PINA & 0x02; //second button is d note
        e = ~PINA & 0x04; //third button is e note
	    Tick_sound();
	}
    return 1; 
}	

