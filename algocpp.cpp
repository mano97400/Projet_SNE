extern "C"{
	#include "algo.h"
};

#include <iostream>
using namespace std;

#include "RiceCompressor/GsrRiceCompress.h"
#include "RiceCompressor/GsrRiceUncompress.h"
#include <RiceCompressor/GsrNotExportedHeader.h>
#include <RiceCompressor/GsrRiceMacroAndConst.h>

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

#define TIMER2_COUNTER_REGISTER 0x80000320

volatile uint32_t* timer2_counter_register = (uint32_t*)TIMER2_COUNTER_REGISTER;


int main() {
/*
	const unsigned int sizeData = 16;
	unsigned short dataBuffer[sizeData] = { 10, 11, 12, 13, 11, 10, 30, 10, 10, 20, 10, 11, 12, 13, 14, 15 };
	// the buffers are double the size of the data in case the compressed packet is greater than the original one
	unsigned int preprocessedBuffer[sizeData * 2];
	unsigned int uncompressBuffer[sizeData * 2];
	unsigned char compressedBuffer[sizeData * 4];
	unsigned short extractedBuffer[sizeData];
	int compressResult = 0;
	int preprocessResult = 0;
	int uncompressResult = 0;
	int postprocessorResult = 0;

	preprocessResult = preprocess(dataBuffer, sizeData, preprocessedBuffer, ESTIMATE_1D_H);
	compressResult = compress(preprocessedBuffer, preprocessResult, compressedBuffer, sizeData * 2);
	uncompressResult = uncompress(compressedBuffer, compressResult, uncompressBuffer, sizeData);
	postprocessorResult = postprocessor(uncompressBuffer, uncompressResult, extractedBuffer, ESTIMATE_1D_H);
	cout << "Hello\n";
*/


	// Initialisation des variables

	for(int i=0;i<Nb_Pixel;i++){
		R[i]=0;
	}
	init_tab_a(Nb_Pixel, A);

	activate_interruptglobal(interrupt_mask_register);

	// Calcul du temps de l'acquisition

	start_acquisition = get_elapsed_time();
	test_acquisition(Nb_Paquet,Nb_Bits_par_paquets,control_reg,image,rx_address);
	end_acquisition = get_elapsed_time();


	// Calcul du temps de l'accumulation

	start_accumulation = get_elapsed_time();
	test_accumulation(R,A,Nb_Pixel);
	end_accumulation = get_elapsed_time();

	// Calcul du temps de l'acquisition

	start_phase1 = get_elapsed_time();
	phase_1(Nb_Paquet,Nb_Bits_par_paquets,control_reg,image,rx_address,R,A,Nb_Pixel);
	disable_interrupt(9,interrupt_mask_register);
	end_phase1 = get_elapsed_time();

	start_overhead = get_elapsed_time();
	time_overhead(Nb_Paquet,interrupt_force_register);
	end_overhead = get_elapsed_time()/Nb_Paquet;
	//dif_overhead = (end_overhead-start_overhead)/Nb_Paquet;

	// Test
	test1();


	return 0;
}
