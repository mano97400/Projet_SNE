#ifndef ALGO_H_
#define ALGO_H_

#include <stdint.h>

const long int size=144000;

/**
 * Retourne le temps écoulé en µs entre deux appels de la fonction
 */
int32_t get_elapsed_time();

/**
 * Met à 1 le bit à l'indice idx du registre. Si le bit est déjà à 1, on le laisse.
 * @param registre pointeur sur le registre à modifier
 * @parm idx indice où on doit modifier le bit
 */
void increment_index_register(volatile uint32_t* registre, uint32_t idx);

/**
 *  Met à 0 le bit à l'indice idx du registre.  Si le bit est déjà à 0, on le laisse.
 *  @param registre pointeur sur le registre à modifier
 *  @param idx indice où on doit modifier le bit
 */
void decrement_index_register(volatile uint32_t* registre, uint32_t idx);

/**
* Enable an interrupt and set the handler.
* Used the value of the macro INTERRUPT_MASK_REGISTER.
* @param irq number of the interrupt to activate
* @param handler function to call when the interrupt \a irq is triggered
*/
void enable_interrupt(uint32_t irq,volatile uint32_t* interrupt_mask_register);

/**
* Disable an interrupt.
* Used the value of the macro INTERRUPT_MASK_REGISTER
* @param irq number of interrupt to disable
*/
void disable_interrupt(uint32_t irq, volatile uint32_t* interrupt_mask_register);

/*
* Trigger the interrupt in parameter.
* Used the value of the macro INTERRUPT_FORCE_REGISTER
* @param irq number of interruption to trigger
* @param interrupt_force_register adress of the force register
*/
void force_interrupt(uint32_t irq, volatile uint32_t* interrupt_force_register);

/**
* Enable an interrupt and set the handler.
* Used the value of the macro INTERRUPT_MASK_REGISTER.
* @param irq number of the interrupt to activate
* @param handler function to call when the interrupt \a irq is triggered
* @param interrupt_mask_register adress of the mask register
*/
void activate_interrupt(uint32_t irq, void* handler,volatile uint32_t* interrupt_mask_register);

/**
 * Handler de l'interruption 10
 */
void handler_inter_10();

/**
 * Appelle la fonction activate_interrupt pour l'interruption 10
 *  @param interrupt_mask_register adresse du registre de mask
 */
void activate_interrupt_10(volatile uint32_t* interrupt_mask_register);

/**
 * Réarme la réception après chaque reception, en copiant dans le descripteur DMA l'adresse du prochain paquet
 * @param adress adresse où on va copier le prochain paquet
 * @param control_reg registre qui indique le bit d'activation
 * @param rx_adress descripteur DMA
 */
void arm_reception( uint32_t* address, int control_reg,uint32_t* rx_address);

/**
 * Algorithme d'acquisition des images
 * @param Nb_Paquet Nombre de paquet
 * @param Nb_Pixel_par_paquets Nombre de pixel par paquet
 * @param control_reg registre qui indique le bit d'activation
 * @param image tableau de l'image à acquérir
 * @param rx_address descripteur DMA
 */
void test_acquisition(int Nb_Paquet, int Nb_Pixel_par_paquets,int control_reg, uint16_t* image, volatile uint32_t* rx_address);
/**
 * Algorithme d'accumulation des images, sommation de pixel à pixel des images
 * @param R[size] Tableau résultant de la sommation
 * @param A[size] Tableau de l'image actuelle
 * @param Nb_Pixel Nombre de pixel constituant l'image
 *
 */
void test_accumulation(uint16_t R[size], uint16_t A[size],int Nb_Pixel);

/**
 * Permet de modéliser le temps lié aux interruptions déclenchées lors de la reception de chaque paquet
 * @param Nb_Paquet Nombre de paquet
 * @param interrupt_mask_register adresse du registre de force
 */
void time_overhead(int Nb_Paquet,volatile uint32_t* interrupt_force_register);

/**
 * Initialise le tableau A entré en argument à 0
 * @Nb_Pixel Nb_Pixel Nombre de pixel constituant l'image
 * @param A[size] tableau A à initialisé
 */
void init_tab_a(int Nb_Pixel,uint16_t A[size]);

/**
 * Lance la phase 1 : Tache Acquisition & et tache Accumulation pour 10 images
 * @param Nb_Paquet Nombre de paquet
 * @param Nb_Pixel_par_paquets Nombre de pixel par paquet
 * @param control_reg registre qui indique le bit d'activation
 * @param image tableau de l'image à acquérir
 * @param rx_address descripteur DMA
 * @param R[size] Tableau résultant de la sommation
 * @param A[size] Tableau de l'image actuelle
 * @param Nb_pixel Nombre de pixel constituant l'image
 *
 */
void phase_1(int Nb_Paquet, int Nb_Pixel_par_paquets,int control_reg, uint16_t* image,volatile uint32_t* rx_address,uint16_t R[size],uint16_t A[size],int Nb_Pixel);

/**
 * Fonction de breakpoint pour le script gdb : Affiche la durée des differents algorithmes
 */
void test_algo();

/**
 * Fonction de breakpoint pour le script gdb : Affiche la performance du processeur pour la phase 1
 */
void test_phase1();

/**
 * Fonction de breakpoint pour le script gdb :  Affiche la performance du processeur pour la phase 2
 */
void test_phase2();

/**
 * Fonction de breakpoint pour le script gdb :  Affiche les durée desphase 1 & 2
 */
void test_phase_1_2();

/**
 * Fonction de breakpoint pour le script gdb : Reset les statistique de la performance du processeur
 */
void test_reset();

/**
 * Vide le cache
 */
void clear_cache();


#endif
