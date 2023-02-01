#ifndef ALGO_H_
#define ALGO_H_

#include <stdint.h>




int32_t get_elapsed_time();

///**
// * Met à 1 le bit à l'indice idx du registre. Si le bit est déjà à 1, on le laisse.
// * @param registre pointeur sur le registre à modifier
// * @parm idx indice où on doit modifier le bit
// */
void increment_index_register(volatile uint32_t* registre, uint32_t idx);

///**
// *  Met à 0 le bit à l'indice idx du registre.  Si le bit est déjà à 0, on le laisse.
// *  @param registre pointeur sur le registre à modifier
// *  @param idx indice où on doit modifier le bit
// */
void decrement_index_register(volatile uint32_t* registre, uint32_t idx);

void disable_interrupt(uint32_t irq, volatile uint32_t* interrupt_mask_register);

void force_interrupt(uint32_t irq, volatile uint32_t* interrupt_force_register);

void enable_interrupt(uint32_t irq,volatile uint32_t* interrupt_mask_register);


void activate_interrupt(uint32_t irq, void* handler,volatile uint32_t* interrupt_mask_register);

void start_timer(uint32_t* timer_counter_register, uint32_t reload_value) ;



void arm_reception( uint32_t* address, int control_reg,uint32_t* rx_address);

void test_acquisition(int Nb_Paquet, int Nb_Bits_par_paquets,int control_reg, uint16_t* image, volatile uint32_t* rx_address);

void test_accumulation(uint16_t R[144],uint16_t A[144],int Nb_Pixel);


void time_overhead(int Nb_Paquet,volatile uint32_t* interrupt_force_register);



void init_tab_a(int Nb_Pixel,uint16_t A[144]);



void handler_inter_9(int* Nb_starts_treated,int Nb_Paquet, int Nb_Bits_par_paquets,int control_reg, uint16_t* image,volatile uint32_t* rx_address,uint16_t R[144],uint16_t A[144],int Nb_Pixel);


void handler_inter_10();


void test1();


void test_phase();

void phase_1(int Nb_Paquet, int Nb_Bits_par_paquets,int control_reg, uint16_t* image,volatile uint32_t* rx_address,uint16_t R[144],uint16_t A[144],int Nb_Pixel);


void phase_2();

void activate_interruptglobal(volatile uint32_t* interrupt_mask_register);

#endif
