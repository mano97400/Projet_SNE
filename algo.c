#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <bcc/bcc.h>


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

void disable_interrupt(uint32_t irq, volatile uint32_t* interrupt_mask_register){
	decrement_index_register(interrupt_mask_register, irq);
}

/*
* Trigger the interrupt in parameter.
* Used the value of the macro INTERRUPT_FORCE_REGISTER
* @param irq number of interruption to trigger*/

void force_interrupt(uint32_t irq, volatile uint32_t* interrupt_force_register){
	increment_index_register(interrupt_force_register, irq);
}
void enable_interrupt(uint32_t irq,volatile uint32_t* interrupt_mask_register) {
	*interrupt_mask_register = *interrupt_mask_register | (1 << irq);
}

void activate_interrupt(uint32_t irq, void* handler,volatile uint32_t* interrupt_mask_register){
	increment_index_register(interrupt_mask_register, irq);
	bcc_isr_register(irq,handler,0);
}



void start_timer(uint32_t* timer_counter_register, uint32_t reload_value) {
	*timer_counter_register = reload_value; // Timer x Value register
	*(timer_counter_register + 1) = reload_value; // Timer x Reload value register
	*(timer_counter_register + 2) = 0xF; //0b1111; // Timer x Control register
}




void arm_reception( uint32_t* address, int control_reg,uint32_t* rx_address) {
	rx_address = address;
	control_reg = 1; // transmission enabled
}

void test_acquisition(int Nb_Paquet, int Nb_Bits_par_paquets,int control_reg, uint16_t* image,volatile uint32_t* rx_address) {
	uint16_t* image_ptr = image;
	for (int i=0; i< Nb_Paquet; i++) {
		arm_reception((uint32_t*)image_ptr,control_reg,rx_address);
		image_ptr +=Nb_Bits_par_paquets;
	}
}

void test_accumulation(uint16_t R[144],uint16_t A[144],int Nb_Pixel){
	for (int i = 0; i< Nb_Pixel; i++) {
		A[i]=A[i]+R[i];
	}
}

void time_overhead(int Nb_Paquet,volatile uint32_t* interrupt_force_register){
	 for(int k = 0; k < Nb_Paquet; k++){
	 	force_interrupt(10,interrupt_force_register);
	 }
 }


void init_tab_a(int Nb_Pixel,uint16_t A[144]){
	for(int i=0;i<Nb_Pixel;i++){
		A[i]=0;
	}
}


void handler_inter_9(int* Nb_starts_treated,int Nb_Paquet, int Nb_Bits_par_paquets,int control_reg, uint16_t* image,volatile uint32_t* rx_address,uint16_t R[144],uint16_t A[144],int Nb_Pixel){
	test_acquisition(Nb_Paquet,Nb_Bits_par_paquets, control_reg,image,rx_address);
	test_accumulation(R,A,Nb_Pixel);
	*Nb_starts_treated++;
	if(*Nb_starts_treated==10){
		//end = get_elapsed_time();
		init_tab_a(A,Nb_Pixel);
	}
}

void handler_inter_10(){

}

void test1(){

}
void test_phase(){

}

// Acquisition / Accumulation
void phase_1(int Nb_Paquet, int Nb_Bits_par_paquets,int control_reg, uint16_t* image,volatile uint32_t* rx_address,uint16_t R[144],uint16_t A[144],int Nb_Pixel){
	for (int i = 0; i < 10; i++) {
		test_acquisition(Nb_Paquet,Nb_Bits_par_paquets, control_reg,image,rx_address);
		test_accumulation(R,A,Nb_Pixel);
	}
	init_tab_a(Nb_Pixel,A);

}




void activate_interruptglobal(volatile uint32_t* interrupt_mask_register){
	activate_interrupt(10,&handler_inter_10,interrupt_mask_register);

}

volatile int pop =5;


//int main() {
//
//	// Initialisation des variables
//
//	for(int i=0;i<Nb_Pixel;i++){
//		R[i]=0;
//	}
//	init_tab_a();
//	activate_interrupt(10,handler_inter_10);
//	activate_interrupt(9,handler_inter_9);
//
//	// Calcul du temps de l'acquisition
//
//	start_acquisition = get_elapsed_time();
//	test_acquisition();
//	end_acquisition = get_elapsed_time();
//
//
//	// Calcul du temps de l'accumulation
//
//	start_accumulation = get_elapsed_time();
//	test_accumulation();
//	end_accumulation = get_elapsed_time();
//
//	// Calcul du temps de l'acquisition
//
//	start_phase1 = get_elapsed_time();
//	phase_1();
//	while(Nb_starts_treated<10){
//		//printf("Ahaha");
//	}
//	disable_interrupt(9);
//	end_phase1 = get_elapsed_time();
//
//	start_overhead = get_elapsed_time();
//	time_overhead();
//	end_overhead = get_elapsed_time()/Nb_Paquet;
//	//dif_overhead = (end_overhead-start_overhead)/Nb_Paquet;
//	test1();
//	// Test
//	return 0;
//}
