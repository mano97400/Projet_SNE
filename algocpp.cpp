extern "C"{
	#include "algo.h"
};

#include <algocpp.hpp>

void convert_uint_to_short(uint16_t tab1[Nb_Pixel], unsigned short tab2[Nb_Pixel*2] ){
	for(int i=0;i<Nb_Pixel;i++){
		tab2[2 * i] = tab1[i] >> 8;
		tab2[2 * i + 1] = tab1[i] & 0xFF;
	}
}

int floor(int size){
	if((size%2)!=0){
		return (size/2)+1;
	}
	return size/2;
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
		cout << compressResult;
		return compressResult;
}

queue<uint16_t> test_transmission(int compressedBufferSize){
	// Liste FIFO à retourner
	queue<uint16_t> list_fifo ;
	const int nbr_paquet=floor(compressedBufferSize);
	uint16_t paquets[nbr_paquet];

	// On découpe les paquets
	for(int i=0;i<nbr_paquet;i++){
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

	// Activation de l'interruption 10
	activate_interrupt_10(interrupt_mask_register);


/*
	// --- Pour une image --- //

	//clear_cache();
	// Calcul du temps de l'acquisition d'une image
	start_acquisition = get_elapsed_time();
	test_acquisition(Nb_Paquet,Nb_Pixel_par_paquets,control_reg,image,rx_address);
	end_acquisition = get_elapsed_time();

	//clear_cache();
	// Calcul du temps de l'accumulation d'une image
	start_accumulation = get_elapsed_time();
	test_accumulation(R,A,Nb_Pixel);
	end_accumulation = get_elapsed_time();

	// Calcul de l'overhead
	start_overhead = get_elapsed_time();
	time_overhead(Nb_Paquet,interrupt_force_register);
	end_overhead = get_elapsed_time()/Nb_Paquet;

	//clear_cache();
	// Compression d'une image
	start_compress= get_elapsed_time();
	int sizeCompressBuffer= test_compression();
	end_compress = get_elapsed_time();

	//clear_cache();
	// Transmission d'une image compressée
	start_transmission = get_elapsed_time();
	test_transmission(sizeCompressBuffer);
	end_transmission = get_elapsed_time();

	test_algo();

*/

	// --- Pour 10 images --- //
	test_reset();
	//clear_cache();
	// *** PHASE 1 *** //
	start_phase1 = get_elapsed_time();
	phase_1(Nb_Paquet,Nb_Pixel_par_paquets,control_reg,image,rx_address,R,A,Nb_Pixel);
	end_phase1 = get_elapsed_time();
	test_phase1();

	test_reset();
	//clear_cache();
	// *** PHASE 2 *** //
	start_phase2 = get_elapsed_time();
	phase_2();
	end_phase2 = get_elapsed_time();
	test_phase2();

	test_phase_1_2();


	return 0;
}
