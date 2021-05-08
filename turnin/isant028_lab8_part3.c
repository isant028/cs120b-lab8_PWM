/*	Author: Isabella Santiago
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 8  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * DEMO LINK: 
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;


unsigned long _avr_timer_M = 1; 
unsigned long _avr_timer_cntcurr = 0; 

void TimerOn() {
  	TCCR1B = 0x0B;
  	OCR1A = 125;
  	TIMSK1 = 0x02;
  	TCNT1 = 0;
  	_avr_timer_cntcurr = _avr_timer_M;
  	SREG |= 0x80;
}

void TimerOff() {
  	TCCR1B = 0x00;
}

void TimerISR() {
  	TimerFlag = 1;
}


ISR(TIMER1_COMPA_vect) {

	_avr_timer_cntcurr--; 
	if (_avr_timer_cntcurr == 0) { 
		TimerISR(); 
		_avr_timer_cntcurr = _avr_timer_M;
	}
}


void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void set_PWM(double frequency) {
 	static double current_frequency;
  	if (frequency != current_frequency) {
   		if (!frequency) { TCCR3B &= 0x08; }
    		else { TCCR3B |= 0x03; }
    		if (frequency < 0.954) { OCR3A = 0xFFFF; }
    		else if (frequency > 31250) { OCR3A = 0x0000; }
    		else { OCR3A = (short) (8000000 / (128 * frequency)) - 1; }
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
unsigned char button = 0x00; 
double seq[14] = {659.25, 493.88, 523.25, 587.33, 523.25, 493.88, 440, 440, 523.25, 659.25,587.33, 523.25,493.88,493.88  };//tetris song
//int duration [] = {800, 400, 400, 800, 400, 400, 800, 400, 400, 800}; //length of each note 
enum state_sounds {start, wait, play, done} state_sound;
unsigned int note =0; //to keep track of notes 
//unsigned int length  = 0; //keep track og how long to play note
unsigned int dur = 0;  //keep track to where in duration array 


void Tick_sound() {
	switch(state_sound) {
		case start:
            state_sound = wait; 
            break; 
		
        case play:
			if (button && dur == 19) { //if button is oushed whike reaching to the end, go to done seate. 
				state_sound = done; 
			}
			else if (dur == 19){
				state_sound = wait;//once reach the end wait;
			}
			else{
				state_sound = play; //stay here if no button pushed and not reach end 
			}
			break;
		
        case wait:
			if (button){
				note = 0;//omce at this state everything restarts from the begionning
                dur = 0; 
				state_sound = play;//once the button is pressed, it will play the song 
			}
			break;

        case done:
			if (!button) { // if button is released in this state, go to beginning and start again
				state_sound = wait;
			}
			else{ //else stay here 
				state_sound = done;
			}
			break;
		default:
			break;
	}

	switch(state_sound) {
		case start:
			set_PWM(0.00); 
			break;
        case wait:
			set_PWM(0.00); 
			break;
		case play:
			if (dur == 0){
				set_PWM(seq[note]);
				note++;
			}
			
            else if (dur == 2){
				set_PWM(seq[note]);
				note++;
			}
			
            else if (dur == 3){
				set_PWM(seq[note]);
				note++;
			}
			
            else if (dur == 4){
				set_PWM(seq[note]);
				note++;
			}
			
            else if (dur == 6){
				set_PWM(seq[note]);
				note++;
			}
			
            else if (dur == 7){
				set_PWM(seq[note]);
				note++;
			}
			
            else if (dur == 8){
				set_PWM(seq[note]);
				note++;
			}
			
            else if (dur == 10) {
				set_PWM(seq[note]);
				note++;
			}
			
            else if (dur == 11){
				set_PWM(seq[note]);
				note++;
			}
			
            else if (dur == 12){
				set_PWM(seq[note]);
				note++;
			}
            
            else if (dur== 14){
				set_PWM(seq[note]);
				note++;
			}
            
            else if (dur == 15) {
				set_PWM(seq[note]);
				note++;
			}
            
            else if (dur == 16){
				set_PWM(seq[note]);
				note++;
			}
            
            else if (dur == 18){
				set_PWM(seq[note]);
				note++;
			}
			dur++;//cycle through this every 200 ms needed to keep time
			break;
		case done:
			set_PWM(0.00);
			break;
		default:
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	PWM_on();
	TimerSet(200);
	TimerOn();
	state_sound = start;
	while(1) {
        button = ~PINA & 0x01;
		Tick_sound();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}
