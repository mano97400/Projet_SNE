#ifndef ALGOCPP_HPP_
#define ALGOCPP_HPP_

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

volatile int Nb_starts_treated = 0;

#endif
