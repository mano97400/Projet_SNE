extern "C"{
	#include "algo.h"
};

#include<bits/stdc++.h>
using namespace std;

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


const long int Nb_Pixel  = 1440;
int Nb_Paquet = 144;
int Nb_Bits_par_paquets = 10 ;

int control_reg;
uint16_t* image;
uint16_t R[Nb_Pixel];
uint16_t A[Nb_Pixel];

volatile int32_t start_accumulation;
volatile int32_t end_accumulation;

volatile int32_t start_acquisition;
volatile int32_t end_acquisition;


volatile int32_t start_phase1;
volatile int32_t end_phase1;

volatile int32_t start_overhead;
volatile int32_t end_overhead;

volatile int32_t start_compress;
volatile int32_t end_compress;

volatile int32_t start_compress2;
volatile int32_t end_compress2;

volatile int32_t start_transmission;
volatile int32_t end_transmission;

volatile int Nb_starts_treated = 0;

#define TIMER2_COUNTER_REGISTER 0x80000320

volatile uint32_t* timer2_counter_register = (uint32_t*)TIMER2_COUNTER_REGISTER;

unsigned char compressedBuffer[Nb_Pixel * 4];

// List fifo
queue<int> list_fifo ;


void convert_uint_to_short(uint16_t tab1[Nb_Pixel], unsigned short tab2[Nb_Pixel*2] ){
	for(int i=0;i<Nb_Pixel;i++){
		tab2[2 * i] = tab1[i] >> 8;
		tab2[2 * i + 1] = tab1[i] & 0xFF;
	}
}

int test_compression(){
		unsigned int preprocessedBuffer[Nb_Pixel * 2];
		int compressResult = 0;
		int preprocessResult = 0;
		unsigned short dataBuffer[Nb_Pixel*2];

		convert_uint_to_short(A, dataBuffer);

		start_compress2 = get_elapsed_time();
		preprocessResult = preprocess(dataBuffer, Nb_Pixel, preprocessedBuffer, ESTIMATE_1D_H);
		compressResult = compress(preprocessedBuffer, preprocessResult, compressedBuffer, Nb_Pixel * 2);
		end_compress2= get_elapsed_time();

		return compressResult;
}

void print_queue(queue<uint16_t> q){
    while (!q.empty())
    {
        cout << q.front() << " ";
        q.pop();
    }
    cout << endl;
}


queue<uint16_t> test_transmission(int compressedBufferSize){
	// Liste FIFO à retourner
	queue<uint16_t> list_fifo ;

	uint16_t paquets[3];

	// On découpe les paquets
	for(int i=0;i<3;i++){
		paquets[i] =compressedBuffer[2*i+1];
		paquets[i] = (paquets[i] << 8) | compressedBuffer[2*i];

		list_fifo.push(paquets[i]);
	}

	return list_fifo;

}


void phase_2(){
	int size=test_compression();
	test_transmission(size);
}


int main() {


	// Initialisation des variables

	for(int i=0;i<Nb_Pixel;i++){
		R[i]=100;
	}
	init_tab_a(Nb_Pixel, A);

	activate_interruptglobal(interrupt_mask_register);

	/*// Calcul du temps de l'acquisition d'une image

	start_acquisition = get_elapsed_time();
	test_acquisition(Nb_Paquet,Nb_Bits_par_paquets,control_reg,image,rx_address);
	end_acquisition = get_elapsed_time();


	// Calcul du temps de l'accumulation d'une image

	start_accumulation = get_elapsed_time();
	test_accumulation(R,A,Nb_Pixel);
	end_accumulation = get_elapsed_time();*/

	// Calcul du temps de la phase 1

	start_phase1 = get_elapsed_time();
	phase_1(Nb_Paquet,Nb_Bits_par_paquets,control_reg,image,rx_address,R,A,Nb_Pixel);
	disable_interrupt(9,interrupt_mask_register);
	end_phase1 = get_elapsed_time();

	start_overhead = get_elapsed_time();
	time_overhead(Nb_Paquet,interrupt_force_register);
	end_overhead = get_elapsed_time()/Nb_Paquet;

	// Compression d'une image
//	start_compress= get_elapsed_time();
	int sizeCompressBuffer= test_compression();
//	end_compress = get_elapsed_time();

	// Transmission d'une image compressée
	start_transmission= get_elapsed_time();
	test_transmission(sizeCompressBuffer);
	end_transmission= get_elapsed_time();

	init_tab_a(Nb_Pixel,A);

	// Fonction Breakpoint pour le script gdb
	test1();

	return 0;
}
