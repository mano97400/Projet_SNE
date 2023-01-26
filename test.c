/*
 * test.c
 *
 *  Created on: 26 janv. 2023
 *      Author: ASUS
 */


#include<stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <bcc/bcc.h>
#define INTERRUPT_PENDING_REGISTER 0x80000204
#define INTERRUPT_FORCE_REGISTER 0x80000208
#define INTERRUPT_MASK_REGISTER 0x80000240
#define INTERRUPT_NUMBER 16
#define RX_ADRESS 0x40000000;
volatile uint32_t* interrupt_pending_register = (uint32_t*)INTERRUPT_PENDING_REGISTER;
volatile uint32_t* interrupt_force_register = (uint32_t*)INTERRUPT_FORCE_REGISTER;
volatile uint32_t* interrupt_mask_register = (uint32_t*)INTERRUPT_MASK_REGISTER;
volatile uint32_t* rx_address= (uint32_t*)RX_ADRESS;
int control_reg;
uint16_t* image;
long int Nb_Pixel  = 14400000;
int Nb_Paquet = 440;
int Nb_Bits_par_paquets = 32767;
volatile int32_t start;
volatile int32_t end;
volatile int32_t dif;

volatile int32_t start_overhead;
volatile int32_t end_overhead;
volatile int32_t dif_overhead;
int32_t get_elapsed_time() {
	static int32_t last_time = 0;
	int32_t elpased_time = 0;
	int32_t current_time = bcc_timer_get_us();
	elpased_time = current_time - last_time;
	last_time = current_time;
	return elpased_time;
}

void increment_index_register(volatile uint32_t* registre, uint32_t idx){
		*registre=*registre|(1<<idx);
}


void decrement_index_register(volatile uint32_t* registre, uint32_t idx){
		*registre=*registre&~(1 << idx);
}

/*
* Disable an interrupt.
* Used the value of the macro INTERRUPT_MASK_REGISTER
* @param irq number of interrupt to disable*/

void disable_interrupt(uint32_t irq){
	decrement_index_register(interrupt_mask_register, irq);
}

/*
* Trigger the interrupt in parameter.
* Used the value of the macro INTERRUPT_FORCE_REGISTER
* @param irq number of interruption to trigger*/

void force_interrupt(uint32_t irq){
	increment_index_register(interrupt_force_register, irq);
}
void enable_interrupt(uint32_t irq) {
	*interrupt_mask_register = *interrupt_mask_register | (1 << irq);
}

void activate_interrupt(uint32_t irq, void* handler){
	increment_index_register(interrupt_mask_register, irq);
	bcc_isr_register(irq,handler,0);
}

/* Handler for the interruption 10 (triggered by the timer)*/

void handler_inter_9(){

}


void arm_reception( uint32_t* address) {
	rx_address = address;
	control_reg = 1; // transmission enabled
}

void test_acquisition() {
	uint16_t* image_ptr = image;
	for (int i=0; i< Nb_Paquet; i++) {
		arm_reception((uint32_t*)image_ptr);
		image_ptr +=Nb_Bits_par_paquets;
	}
}
void time_overhead(){
	start_overhead = get_elapsed_time();
	 for(int k = 0; k < Nb_Paquet; k++){
	 	force_interrupt(9);
	 }
 }

void test1(){

}



volatile int pop =5;


int main() {
	//test1();
	activate_interrupt(9,handler_inter_9);
	printf("!!!Hello World!!!"); // prints !!!Hello World!!!
	pop = 289;
	start = get_elapsed_time();
	test_acquisition();
	end = get_elapsed_time();
	dif = end-start;
	start_overhead = get_elapsed_time();
	time_overhead();
	end_overhead = get_elapsed_time();
	dif_overhead = (end_overhead-start_overhead)/Nb_Paquet;
	test1();
	// Test mmmmmmmmcwxcxwcxwc99ll
	return 0;
}
