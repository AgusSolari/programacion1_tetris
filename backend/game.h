#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define HEIGHT  16
#define WIDTH   10

char game_matrix[HEIGHT][WIDTH];

struct block_data {
    uint8_t x;
    uint8_t y;
    uint8_t id;
    uint8_t rot;
}block_data;

// Funciones del juego
void init_game(void); // inicia el juego en blanco
void run_game(void); // ejecuta el juego

// Borra la matriz
void clear_matrix(void);

// Imprime la matriz en consola
void print_matrix(void);

// Inserta un nuevo bloque arriba
void insert_block(uint8_t id);

// Desciende las coordenadas del bloque
void descend_block (void);

// Gira el bloque (1 = horario, 0 = antihorario)
void rotate_block (int direction);

// mueve el bloque en horizontal (1 = derecha, 0 = izquierda)
void move_block(int direction);

//Devuelve un ID de bloque aleatorio
uint8_t next_block (void);


#endif // GAME_H