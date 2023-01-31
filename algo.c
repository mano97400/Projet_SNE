#include<stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <bcc/bcc.h>

#define INTERRUPT_PENDING_REGISTER 0x80000204
#define INTERRUPT_FORCE_REGISTER 0x80000208
#define INTERRUPT_MASK_REGISTER 0x80000240
#define RX_ADRESS 0x40000000;

volatile uint32_t* interrupt_pending_register = (uint32_t*)INTERRUPT_PENDING_REGISTER;
volatile uint32_t* interrupt_force_register = (uint32_t*)INTERRUPT_FORCE_REGISTER;
volatile uint32_t* interrupt_mask_register = (uint32_t*)INTERRUPT_MASK_REGISTER;
volatile uint32_t* rx_address= (uint32_t*)RX_ADRESS;


long int Nb_Pixel  = 14400000;
int Nb_Paquet = 440;
int Nb_Bits_par_paquets = 524280;

int control_reg;
uint16_t* image;
uint16_t R[14400000];
uint16_t A[14400000];

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
		*registre=*registre|(1<<idx);#include<stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <bcc/bcc.h>
#include "GsrRiceCompress.h"
#include "GsrPreprocessMode.h"


#define INTERRUPT_PENDING_REGISTER 0x80000204
#define INTERRUPT_FORCE_REGISTER 0x80000208
#define INTERRUPT_MASK_REGISTER 0x80000240
#define RX_ADRESS 0x40000000;

volatile uint32_t* interrupt_pending_register = (uint32_t*)INTERRUPT_PENDING_REGISTER;
volatile uint32_t* interrupt_force_register = (uint32_t*)INTERRUPT_FORCE_REGISTER;
volatile uint32_t* interrupt_mask_register = (uint32_t*)INTERRUPT_MASK_REGISTER;
volatile uint32_t* rx_address= (uint32_t*)RX_ADRESS;


long int Nb_Pixel  = 1440;
int Nb_Paquet = 144;
int Nb_Bits_par_paquets = 10;

int control_reg;
uint16_t* image;
uint16_t R[1440];
uint16_t A[1440];
uint16_t Preprocess_A[2*1440];

volatile int32_t start_accumulation;
volatile int32_t end_accumulation;

volatile int32_t start_acquisition;
volatile int32_t end_acquisition;

volatile int32_t start_phase1;
volatile int32_t end_phase1;

volatile int32_t start_overhead;
volatile int32_t end_overhead;


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

#define TIMER2_COUNTER_REGISTER 0x80000320

volatile uint32_t* timer2_counter_register = (uint32_t*)TIMER2_COUNTER_REGISTER;



void start_timer(uint32_t* timer_counter_register, uint32_t reload_value) {
*timer_counter_register = reload_value; // Timer x Value register
*(timer_counter_register + 1) = reload_value; // Timer x Reload value register
*(timer_counter_register + 2) = 0xF; //0b1111; // Timer x Control register
}


volatile int Nb_starts_treated = 0;
/* Handler for the interruption 9 (triggered by the timer)*/




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

void test_accumulation(){
	for (int i = 0; i< Nb_Pixel; i++) {
		A[i]=A[i]+R[i];
	}
}

void time_overhead(){
	 for(int k = 0; k < Nb_Paquet; k++){
	 	force_interrupt(10);
	 }
 }


void init_tab_a(){
	for(int i=0;i<Nb_Pixel;i++){
		A[i]=0;
	}
}


void handler_inter_9(){
	test_acquisition();
	test_accumulation();
	Nb_starts_treated++;
	if(Nb_starts_treated==10){
		//end = get_elapsed_time();
		init_tab_a();
	}
}

void handler_inter_10(){

}

void test1(){

}
void test_phase(){

}

void phase_1(){

	uint32_t period = 100000; // T = 100ms
	start_timer(timer2_counter_register, period);
}

void phase_2(){

	preprocess(A,Nb_Pixel,Preprocess_A,ESTIMATE_1D_H);
}

volatile int pop =5;



int main() {

	// Initialisation des variables

	for(int i=0;i<Nb_Pixel;i++){
		R[i]=0;
	}
	init_tab_a();
	activate_interrupt(10,handler_inter_10);
	activate_interrupt(9,handler_inter_9);

	// Calcul du temps de l'acquisition

	start_acquisition = get_elapsed_time();
	test_acquisition();
	end_acquisition = get_elapsed_time();


	// Calcul du temps de l'accumulation

	start_accumulation = get_elapsed_time();
	test_accumulation();
	end_accumulation = get_elapsed_time();

	// Calcul du temps de l'acquisition

	start_phase1 = get_elapsed_time();
	phase_1();
	while(Nb_starts_treated<10){
		//printf("Ahaha");
	}
	disable_interrupt(9);
	end_phase1 = get_elapsed_time();

	start_overhead = get_elapsed_time();
	time_overhead();
	end_overhead = get_elapsed_time()/Nb_Paquet;
	//dif_overhead = (end_overhead-start_overhead)/Nb_Paquet;
	test1();
	// Test
	return 0;
}
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

void test_accumulation(){
	for (int i = 0; i< Nb_Pixel; i++) {
		A[i]=A[i]+R[i];
	}
}

void time_overhead(){
	 for(int k = 0; k < Nb_Paquet; k++){
	 	force_interrupt(9);
	 }
 }


void init_tab_a(){
	for(int i=0;i<Nb_Pixel;i++){
		A[i]=0;
	}
}

void test1(){

}

void phase_1(){
	init_tab_a();
	for(int i =0;i<10;i++){
		test_acquisition();

		test_accumulation();
	}
	 init_tab_a();
}


volatile int pop =5;



int main() {
	for(int i=0;i<Nb_Pixel;i++){
		R[i]=0;
	}

	//test1();
	activate_interrupt(9,handler_inter_9);

	start = get_elapsed_time();
	//phase_1();
	test_accumulation();
	end = get_elapsed_time();
	dif = end-start;

	start_overhead = get_elapsed_time();
	time_overhead();
	end_overhead = get_elapsed_time();
	dif_overhead = (end_overhead-start_overhead)/Nb_Paquet;
	test1();
	// Test
	return 0;
}
