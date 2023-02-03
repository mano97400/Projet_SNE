#ifndef ALGOCPP_HPP_
#define ALGOCPP_HPP_

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

const long int Nb_Pixel  = 144000;
int Nb_Paquet = 144;
int Nb_Pixel_par_paquets = 1000 ;

int control_reg;
uint16_t* image;
uint16_t R[Nb_Pixel];
uint16_t A[Nb_Pixel];

volatile int32_t start_accumulation;
volatile int32_t end_accumulation=2;

volatile int32_t start_acquisition;
volatile int32_t end_acquisition;

volatile int32_t start_phase1;
volatile int32_t end_phase1;

volatile int32_t start_phase2;
volatile int32_t end_phase2;

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

// Liste fifo
queue<int> list_fifo ;

/**
 * Convertit un tableau de uint16 en unsigned short
 * @param tab1[Nb_Pixel] tableau de uint16 à convertir
 * @param tab2[Nb_Pixel*2] tableau résultant de la conversion
 */
void convert_uint_to_short(uint16_t tab1[Nb_Pixel], unsigned short tab2[Nb_Pixel*2] );

/**
 * Algorithme de compression d'une image
 */
int test_compression();

/**
 * Retourne la valeur enitère supérieur du paramètre size
 */
int floor(int size);

/**
 * Tache de transmission : chaque paquet compressé sera copié vers une liste fifo qui sera retournée
 * @param compressedBufferSize taille en octet du tableau compressé
 */
queue<uint16_t> test_transmission(int compressedBufferSize);

/**
 * Lance la phase 2 : Tache de compression & tache de transmission d'une image
 */
void phase_2();

#endif
