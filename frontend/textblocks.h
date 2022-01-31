#ifndef _TEXT_H_
#define _TEXT_H_

#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_image.h> 
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdint.h>
#include <stdbool.h>


//Estructura de datos de los bloques de texto
typedef struct 
{
    ALLEGRO_COLOR block_color;   //color del fondo
    ALLEGRO_COLOR font_color;   //color del fondo
    int font_size;              // tamanio de la letra
    char* texto;                //texto a imprimir
    char* tipo_letra;           //fuente del texto
    int posx;                   //posicion en x del texto
    int posy;                   //posicion en y del texto
    int alineacion;             //alineacion a la izquierda, derecha o centro. Si es centro, las coordenadas x e y son del centro del texto
} blocktext_t;

typedef enum{
    ALINEADO_IZQUIERDA,
    CENTRADO,
    ALINEADO_DERECHA
}text_alineacion_t;

blocktext_t* text_init_alleg ( ALLEGRO_COLOR color_bloque, ALLEGRO_COLOR color_font, int tamanio_letra, char* palabras, char* nombre_letra, int x, int y, int align);
//DESCRIPCION: Inicializa un Bloque de Texto, reserva memoria dinamica, se ejecuta 1 vez al principio del programa por cada texto que se desea escribir.
// - [color_bloque]: color a asignarle al fondo del bloque, es un rectangulo. se le pasa un ALLEGRO_COLOR, usando al_map_rgb(r, g, b)
// - [color_font]: color de la fuente. Lo mismo que el bloque. La combinacion de parametros r, g, b de la funcion de alegro devuelve el color deseado
// - [tamanio_letra]: tamanio de la letra, el fondo se ajusta al tamanio de la letra asi que no hace falta cambiarlo
// - [palabras]: el texto que se desea imprimir en pantalla
// - [tipo_letra]: un string que contiene el path para el archivo de la fuente
// - [x]:posicion en x del display, depende del parametro de alineacion que punto se toma. si esta alineado a la derecha, x e y seran de la esquina superior derecha
// - [y]: lo mismo pero posicion en y del display.
// - [align]: parametro de alineacion de texto. Usar el Enum text_alinecion_t
//RETORNA: NULL en caso de que haya un error y si no un puntero a la estructura blocktext_t
// EJEMPLO: char * pathway_fuente= "./frontend/images/arial.ttf"
//blocktext_t * GAME_OVER = text_init_alleg(al_map_rgb(255,0,0), al_map_rgb(0,255,0), 30, "GAME OVER", pathway_fuente, 350, 100)
//

void text_drawer (blocktext_t* bloque);
//DESCRIPCION: Dibuja en el display un bloque de texto usando los datos de la estructura blocktext_t. 
// - [bloque]: Puntero a la estructura del bloque para obtener los datos
//ATENCION: NO FLIPEA EL DISPLAY, ESO SE HACE A MANO


int text_gloabal_font_changer (blocktext_t * bloque);
//DESCRIPCION: Cambia el tipo y tamanio de letra. Se va a usar un unico ALLEGRO_FONT global para el tipo de letra. De ser necesario...  
//cambiar el estilo o tamanio esta funcion se encarga de ello. 
// - [bloque]: Puntero a estructura la cual contiene todos los datos necesarios para cambiar el tipo de letra
//RETORNA -1 si hubo error, 0 si fue exitoso

void text_colour_changer (blocktext_t * bloque, ALLEGRO_COLOR color_fondo, ALLEGRO_COLOR color_letras);
//DESCRIPCION: Cambia el color tanto del fondo del bloque como el de la letra.
//Puede ser usada para OCULTAR EL BLOQUE, solo le pasas el mismo color de fondo y letra
// - [bloque]: Puntero a estructura para cambiar los colores
// - [color_fondo]:Nuevo color a poner en el fondo. Usar al_map_rgb para enviar el color
// - [color_letras]: Nuevo color de las letras. Idem
//ATENCION: SOLO CAMBIA LOS DATOS, NO IMPRIME EN PANTALLA. PARA ESO LLAMAR NUEVAMENTE A text_drawer

void text_location_changer (blocktext_t * bloque, int newX, int newY);
//DESCRIPCION: Cambia las coordenadas en x e y del bloque. Acordarse que depende del parametro. Si es alineado a la izq, las coordenadas
//seran de la esquina superior izq del texto
// - [newX]: nueva coordenada en X
// - [newY]: nueva coordenada en Y
//ATENCION: SOLO CAMBIA LOS DATOS, NO IMPRIME EN PANTALLA. PARA ESO LLAMAR NUEVAMENTE A text_drawer


void text_font_changer (blocktext_t* bloque,int tamanio, char * font_family);
//DESCRIPCION: Cambia el tipo y tamanio de letra de un bloque de texto. Lo hace mediante la modificacion de la estructura.
//Para cambiar el tipo de letra global se necesita llamar a text_gloabal_font_changer 
// - [bloque]: puntero a texto que se le quiere cambiar 
// - [tamanio]: nuevo tamanio a elegir
// - [font_family]: path al archivo con el nuevo tipo de letra  
//ATENCION: SOLO CAMBIA LOS DATOS, NO IMPRIME EN PANTALLA. PARA ESO LLAMAR NUEVAMENTE A text_gloabal_font_changer y text_drawer

void text_destroy (blocktext_t * bloque);
//DESCRIPCION: destruye toda la memoria reservada para cada estructura
// - [bloque]: puntero a estructura que se quiere destruir
//OBLIGATORIO LLAMARLA AL FINAL DEL PROGRAMA

#endif //_TEXT_H_