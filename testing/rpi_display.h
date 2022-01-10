#ifndef TETRIS_PROGRA_1_RPI_DISPLAY_H
#define TETRIS_PROGRA_1_RPI_DISPLAY_H

//   C O N S T A N T E S
#define RPI_WIDTH 16
#define RPI_HEIGHT 16

#include <stdint.h>


//   P R O T O T I P O S

void rpi_set_display(uint8_t x, uint8_t y, uint8_t val);
void rpi_init_display (void); // inicializa el display de la RPI
void rpi_run_display (void); // analiza el valor de la matriz publica y prende el respectivo led sobre la RPI
void rpi_copyToDis (void* fromMat, uint8_t height, uint8_t width, uint8_t x, uint8_t y); // copia la matriz "fromMat" en la matriz de la RPI

#endif //TETRIS_PROGRA_1_RPI_DISPLAY_H
