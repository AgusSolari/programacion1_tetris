/* 
 * File:   main_linux.c
 * Author: RD,AC,AS,SV
 */
#include "./backend/game.h"
#include "./frontend/easy_timer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_image.h> //NO OLVIDAR INCLUIR ALLEGRO_IMAGE EN LINKER
#include <allegro5/allegro_audio.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_acodec.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "./frontend/keyboard.h"
#include "./frontend/textblocks.h"
#include "./backend/menu.h"
#include "./backend/menu.h"
#include "./frontend/audio_lib.h"

#define BLOCKSZ 50
#define ANCHO   10
#define ALTO    16
#define PATH_TTF "./frontend/images/Arcade Regular.ttf"
#define YPOS_SCORE BLOCKSZ*12
#define YPOS_NIVEL BLOCKSZ*14

// **************************************
// *	 V A R S . G L O B A L E S		*
// **************************************
//BITMAPS
ALLEGRO_BITMAP *image = NULL;
ALLEGRO_BITMAP *muroH = NULL;
ALLEGRO_BITMAP *muroV = NULL;
ALLEGRO_BITMAP *pieza_blanca = NULL;
ALLEGRO_BITMAP *tetris_cartel = NULL;
ALLEGRO_BITMAP *diagrama_teclado = NULL;
ALLEGRO_BITMAP *jugabilidad_diagrama = NULL;
ALLEGRO_BITMAP *gameoverfoto = NULL;

//UTILIDADES ALLEGRO
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_EVENT_QUEUE * event_queue = NULL;

//UTILIDADES EXTRA
blocktext_t * score=NULL;
blocktext_t * nivel=NULL;
menu_t *principal_menu = NULL;
menu_t *pausa_menu = NULL;
menu_t *gameover_menu = NULL;

// EFECTOS DE SOline_comp_audioNIDO Y MUSICA
audio_t* lose_audio = NULL;

audio_t* menu_audio = NULL;
audio_t* game_audio = NULL;
audio_t* pause_audio = NULL;
audio_t* line_comp_audio[4] = {NULL,NULL,NULL,NULL};
audio_t* chime_audio = NULL;
audio_t* chime_select_audio = NULL;
audio_t* win_audio = NULL;


/*void playAudio(void){
    al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
}
*/






// ******************************
// *	P R O T O T I P O S		*
// ******************************

int initialize_alleg(void);
void initialize_display_game (void);
void end_program (void);
void update_display(void);
void keypress_callback(uint8_t key);
void read_events(void);
void display_menu_display(void);
void animation_row_compleate(void);
void main_game_start(void);
void screen_how_to_play (void);
uint8_t param_lvl_fetch (void);
//int init_audio(void);

// ******************************
// *	C A L L B A C K S		*
// ******************************
//CALLBACK DE EXIT GAME
void exit_game(void);
void volver_al_main_menu(void);
void restart_game(void);
void resume_game(void);

int main (void){
	if(initialize_alleg()){
        printf("Allegro FAIL!\n");
        return -1;
    } //inicializo Allegro
    
    easytimer_set_realTimeLoop(read_events); // Leer eventos durante delays

    if(!audio_install()){
        printf("Error al iniciar el audio!\n");
        end_program(); //si algo fallo termino el programa
        return -1;   
    }

    menu_audio = audio_init("main_title.wav", 1, AUDIO_MUSIC);
    game_audio = audio_init("tetris.wav", 0.4, AUDIO_MUSIC);
    pause_audio = audio_init("pausa.wav", 3, AUDIO_MUSIC);

    line_comp_audio[0] = audio_init("1_line_compl.wav", 1, AUDIO_EFFECT);
    line_comp_audio[1] = audio_init("2_line_compl.wav", 1, AUDIO_EFFECT);
    line_comp_audio[2] = audio_init("3_line_compl.wav", 1, AUDIO_EFFECT);
    line_comp_audio[3] = audio_init("win.wav", 1, AUDIO_EFFECT);
    lose_audio = audio_init("lose.wav", 1, AUDIO_EFFECT);
    chime_audio = audio_init ("chime.wav",1, AUDIO_EFFECT);
    chime_select_audio = audio_init ("chime_select.wav", 1, AUDIO_EFFECT);

    int i;
    for(i=0; i<4 ; i++){
        if(line_comp_audio[i] == NULL){
            end_program();
            return -1;
        }
    }
    if(menu_audio == NULL || game_audio == NULL || pause_audio == NULL || lose_audio == NULL || chime_audio == NULL || chime_select_audio == NULL){
        end_program(); // aca se destruye el audio
        return -1;
    }

    easytimer_set_realTimeLoop(read_events); // Leer eventos durante delays

    keyb_on_press(keypress_callback);
    keyb_use_press_callback_for_longpress(KEYB_DOWN);
    keyb_use_press_callback_for_longpress(KEYB_LEFT);
    keyb_use_press_callback_for_longpress(KEYB_RIGHT);
    
    principal_menu = menu_init(3, "MENU PRINCIPAL", NULL, MENU_ACTION_DO_NOTHING);
    pausa_menu = menu_init(3, "MENU DE PAUSA", NULL, MENU_ACTION_JUST_EXIT);
    gameover_menu = menu_init(2, "GAME OVER", NULL, MENU_ACTION_DO_NOTHING);

    if(principal_menu == NULL || pausa_menu == NULL || gameover_menu == NULL){
        printf("Error NULL menu!\n");
        end_program(); // End program destruye todo
        return -1;
    }

    // CALLBACKS DE OPCIONES DE MENU MAIN
    menu_set_option(principal_menu, 0, "JUGAR", main_game_start);
    menu_set_option(principal_menu, 1, "COMO JUGAR", screen_how_to_play );
    menu_set_option(principal_menu, 2, "SALIR", menu_force_close_current);

	// CALLBACKS DE OPCIONES DEL MENU DE PAUSA
	menu_set_option(pausa_menu, 0, "REANUDAR", resume_game);
    menu_set_option(pausa_menu, 1, "REINICIAR", restart_game);
    menu_set_option(pausa_menu, 2, "SALIR", exit_game);
    
	// CALLBACKS DE OPCIONES DEL MENU DE GAME OVER
	menu_set_option(gameover_menu, 0, "REINICIAR", restart_game);
    menu_set_option(gameover_menu, 1, "SALIR", volver_al_main_menu);


    menu_set_event_listener_display(read_events, display_menu_display);
   
    game_set_delrow_callback(animation_row_compleate);

    // MAIN MENU
    menu_run(principal_menu);
    
    end_program(); //borro todo

    return 0;

}

void exit_game(void){
    game_quit();                // Finalizar juego
    menu_force_close(pausa_menu); // Cerrar menu pausa
}

//CALLBACK DE GAMEOVER
void volver_al_main_menu (void){
    printf("volviendo al ma");
    game_quit();
    menu_force_close(gameover_menu);
    printf("in menu...\n");
}

//CALLBACK DE REINICIO DE JUEGO
void restart_game(void){
    // manage_music(game, start);
	initialize_display_game(); //inicio el display del juego
    menu_force_close_current(); // Cerrar menu pausa
    game_start(); 	//Corre el juego
}

//CALLBACK DE RENAUDAR JUEGO
void resume_game(void)
{
    // manage_music(game, start);
	initialize_display_game(); //inicio el display del juego
	menu_force_close_current();	//Cierra el menu
	game_run();	//Corre el juego
}

void main_game_start(void){
    game_data_t game_data;
    uint64_t lastMillis;
    game_start(); // iniciar el juego
    initialize_display_game();

    while ((game_data = game_get_data()).state != GAME_QUIT)
    {
        audio_play(game_audio);
        read_events(); // Leer teclado y eventos (Allegro)
        
        if(game_data.state == GAME_RUN && easytimer_get_millis()-lastMillis >= game_data.speed_interval){
            game_move_down();
            game_run();
            update_display();
            lastMillis = easytimer_get_millis();
        }

        if(game_data.state == GAME_LOSE){
            audio_force_stop(game_audio);
            audio_play(lose_audio);
            // manage_music(lose, start);
            al_draw_scaled_bitmap(gameoverfoto, 0, 0, al_get_bitmap_width(gameoverfoto), al_get_bitmap_height(gameoverfoto), BLOCKSZ*3, BLOCKSZ*3, BLOCKSZ*8, BLOCKSZ*4, 0 );
            al_flip_display();
            printf("Perdiste! The Game\n");

            while (audio_is_playing(lose_audio))
            {
                read_events();
            }
            menu_run(gameover_menu);
        }
    }
    printf("Leaving game...\n");
} // main_game_start

void animation_row_compleate(void)
{
    game_data_t game_data = game_get_data();
    if(game_row_complete[0])
    {
        int z,i;
        float reductor;
        float angulo;
        float decremento = 0.1;

        if(game_data.streak == 4){ // Si se completaron 4 filas a la vez
            al_draw_text(text_font_pointer_fetcher(),al_map_rgb(255,0,0), BLOCKSZ*6,BLOCKSZ*4,CENTRADO,"T E T R I S !");
            al_flip_display();
        }
        audio_play(line_comp_audio[game_data.streak-1]); // filas completadas - 1

        for(reductor=2.1, angulo=0; reductor>=0; angulo+=(3.1415/8)){
            for(i=0; game_row_complete[i] != 0 && i < 4 ; i++){
                for(z=1; z<=ANCHO; z++){
                    al_draw_scaled_bitmap(image, 0, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image), BLOCKSZ*z, BLOCKSZ*(game_row_complete[i]), BLOCKSZ, BLOCKSZ, 0);
                    //pongo el fondo en negro
                    al_draw_tinted_scaled_rotated_bitmap(pieza_blanca,  al_map_rgba_f(1, 1, 1, 1), al_get_bitmap_width(pieza_blanca)/2, al_get_bitmap_height(pieza_blanca)/2, (BLOCKSZ/2 +BLOCKSZ*z), (BLOCKSZ/2 +BLOCKSZ*(game_row_complete[i])),reductor, reductor, angulo, 0);
                    //Dibujo una pieza blanca, se va haciendo mas chia a medida que rota
                }//este ciclo va por columnas
                al_flip_display();
            }//este otro ciclo va por filas
            reductor-=decremento;
        }		

        for(i=0; game_row_complete[i] != 0 && i < 4 ; i++){
            for(z=1; z<=ANCHO; z++){
                al_draw_scaled_bitmap(image, 0, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image), BLOCKSZ*z, BLOCKSZ*(game_row_complete[i]), BLOCKSZ, BLOCKSZ, 0);
            } //pongo el fondo en negro de nuevo
            delete_row(game_row_complete[i]);
            game_row_complete[i]= 0;
        }
        al_flip_display();
    }
}

//CALLBACK DE CONTROLES/INSTRUCCIONES
void screen_how_to_play (void){
    int actual=1;
    al_draw_scaled_bitmap(diagrama_teclado, 0, 0, al_get_bitmap_width(diagrama_teclado), al_get_bitmap_height(diagrama_teclado), 0, 0, al_get_display_width(display), al_get_display_height(display), 0);
    al_flip_display();
    while (!keyb_is_pressed(KEYB_ESC))
    {
        read_events();
        if(keyb_is_pressed(KEYB_LEFT) && (actual==2)){
        printf("Hasta aca llega\n");
            al_draw_scaled_bitmap(diagrama_teclado, 0, 0, al_get_bitmap_width(diagrama_teclado), al_get_bitmap_height(diagrama_teclado), 0, 0, al_get_display_width(display), al_get_display_height(display), 0);
            al_flip_display();
            actual=1;
            easytimer_delay(150);
        }
        
        else if(keyb_is_pressed(KEYB_RIGHT) && (actual==1)){
            al_draw_scaled_bitmap(jugabilidad_diagrama, 0, 0, al_get_bitmap_width(jugabilidad_diagrama), al_get_bitmap_height(jugabilidad_diagrama), 0, 0, al_get_display_width(display), al_get_display_height(display), 0);
            al_flip_display();
            actual=2;
            easytimer_delay(150);
        }
    }
}

void display_menu_display(void){
    menu_t menu_data = menu_get_current_menu_data();    //consigo los datos del menu actual

	if (menu_is_available(principal_menu))
        audio_play(menu_audio);
		// manage_music(menu, start);
	if (menu_is_available(pausa_menu))
        audio_play(pause_audio);
		// manage_music(pausa, start);
    if (menu_is_available(gameover_menu)){
        audio_play(pause_audio);
        // manage_music(game, stop);
	//	manage_music(pausa, start);
    }


    if((menu_is_available(principal_menu)) || (menu_is_available(pausa_menu))){
        uint8_t id;
        al_clear_to_color(al_map_rgb(0,0,0));   //fondo negro
        al_draw_scaled_bitmap(tetris_cartel, 0, 0, al_get_bitmap_width(tetris_cartel), al_get_bitmap_height(tetris_cartel),BLOCKSZ*3, BLOCKSZ, al_get_display_width(display)-BLOCKSZ*6, BLOCKSZ*8, 0);
        //dibujo el cartel del tetris
        blocktext_t * menuprin = text_init_alleg(TXT_ROJO, 80, menu_data.title, PATH_TTF, al_get_display_width(display)/2, al_get_display_height(display)/2+BLOCKSZ*2, CENTRADO );
        if(text_drawer(menuprin)){
            text_destroy(menuprin);
            end_program(); // si hay problemas con la fuente salgo del programa
        }          //Escribo el titulo del menu actual
        text_destroy(menuprin); //DESTRUYO LA ESTRUCTURA


        for(id=0; id<menu_data.n_options; id++)
        {
            if(menu_data.current_option == id){
                blocktext_t * menuop = text_init_alleg(TXT_VERDE, 60, menu_data.option_titles[id], PATH_TTF, al_get_display_width(display)/2,(al_get_display_height(display)/2)+(BLOCKSZ*(4+id)), CENTRADO );
                if(text_drawer(menuop)){
                    text_destroy(menuop);
                    end_program(); // si hay problemas con la fuente salgo del programa
                } 
                text_destroy(menuop);//destruyo estructura
            }// si es la que esta siendo apuntada, la dibujo en verde

            else{
                blocktext_t * menuop = text_init_alleg(TXT_BLANCO, 60, menu_data.option_titles[id], PATH_TTF, al_get_display_width(display)/2, (al_get_display_height(display)/2)+(BLOCKSZ*(4+id)), CENTRADO );
                if(text_drawer(menuop)){
                    text_destroy(menuop);
                    end_program(); // si hay problemas con la fuente salgo del programa
                } 
                text_destroy(menuop);//destruyo estructura
            }   //si no, la escribo en blanco
        }   //imprimo cada opcion del menu en pantalla una abajo de la otra
            
            al_flip_display();  //MUESTRO LO QUE ESCRIBI
    }
    
    else if (menu_is_available(gameover_menu)){
        uint8_t id;
        al_clear_to_color(al_map_rgb(0,0,0));   //fondo negro
        al_draw_scaled_bitmap(tetris_cartel, 0, 0, al_get_bitmap_width(tetris_cartel), al_get_bitmap_height(tetris_cartel), BLOCKSZ*3, BLOCKSZ, al_get_display_width(display)-BLOCKSZ*8, BLOCKSZ*6, 0);
        //dibujo el cartel del tetris
        blocktext_t * menuprin = text_init_alleg(TXT_ROJO, 80, menu_data.title, PATH_TTF, al_get_display_width(display)/2, al_get_display_height(display)/2, CENTRADO );

        if(text_drawer(menuprin)){
            text_destroy(menuprin);
            end_program(); // si hay problemas con la fuente salgo del programa
        }          //Escribo el titulo del menu actual
        text_destroy(menuprin); //DESTRUYO LA ESTRUCTURA

        al_draw_textf(text_font_pointer_fetcher(), al_map_rgb(255, 255, 255), al_get_display_width(display)/2,(al_get_display_height(display)/2)+(BLOCKSZ*(2)),CENTRADO, "PUNTAJE FINAL: %i", game_get_data().score);
            for(id=0; id<menu_data.n_options; id++)
        {
            if(menu_data.current_option == id){
                blocktext_t * menuop = text_init_alleg(TXT_VERDE, 60, menu_data.option_titles[id], PATH_TTF, al_get_display_width(display)/2,(al_get_display_height(display)/2)+(BLOCKSZ*(4+id)), CENTRADO );
                if(text_drawer(menuop)){
                    text_destroy(menuop);
                    end_program(); // si hay problemas con la fuente salgo del programa
                } 
                text_destroy(menuop);//destruyo estructura
            }// si es la que esta siendo apuntada, la dibujo en verde
            else{
                blocktext_t * menuop = text_init_alleg(TXT_BLANCO, 60, menu_data.option_titles[id], PATH_TTF, al_get_display_width(display)/2, (al_get_display_height(display)/2)+(BLOCKSZ*(4+id)), CENTRADO );
                if(text_drawer(menuop)){
                    text_destroy(menuop);
                    end_program(); // si hay problemas con la fuente salgo del programa
                } 
                text_destroy(menuop);//destruyo estructura
            }   //si no, la escribo en blanco

            al_flip_display();
        }   //imprimo cada opcion del menu en pantalla una abajo de la otra
    }
}

// Leer los eventos de Allegro (teclado, display, etc)
void read_events(void){
    ALLEGRO_EVENT ev;
    if (event_queue != NULL && al_get_next_event(event_queue, &ev)) //Toma un evento de la cola
    {
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            game_quit();
        }
    }
    keyb_run(&ev); // Siempre debe ejecutarse el keyb_run
}


void keypress_callback(uint8_t key){
    printf("Keypress Callback In\n");
    if(easytimer_delay_active()){ // si hay un delay activo no hago nada
        // printf("se mantiene presionada\n");
        return;
    }
    if(menu_is_current_available()){
		
        switch (key)
        {
            case KEYB_UP:
                // manage_music(chime_select, start);
                audio_play(chime_audio);
                menu_go_up();
                printf("menu UP\n");
                break;

            case KEYB_DOWN:
                // manage_music(chime_select, start);
                audio_play(chime_audio);
                menu_go_down();
                printf("menu DOWN\n");
                break;

            case KEYB_LEFT:
                // manage_music(chime_select, start);
                audio_play(chime_audio);
                menu_go_back();
                printf("menu LEFT\n");
                break;

            case KEYB_SPACE:
            case KEYB_ENTER:
                printf("menu BTN...\n");
                // manage_music(chime, start);
				// manage_music(menu, stop);
                audio_play(chime_select_audio);
                menu_go_select();
                printf("menu BTN\n");
                break;

            default:
                break;
        }
        menu_t data = menu_get_current_menu_data();
        printf("menu: %s, option: %u %s\n", data.title, data.current_option, data.option_titles[data.current_option]);
    }
    else if(game_get_data().state == GAME_RUN){

        switch (key)
        {
        case KEYB_UP:
            printf("UP\n");
            break;

        case KEYB_DOWN:
            if(game_get_data().id == 0)
                game_insert_block();
            else
                game_move_down();
            printf("DOWN\n");
            break;

        case KEYB_LEFT:
            game_move_horizontal(0);
            printf("LEFT\n");
            break;

        case KEYB_RIGHT:
            game_move_horizontal(1);
            printf("RIGHT\n");
            break;

        case KEYB_E:
			// manage_music(chime_select, start);
            audio_play(chime_audio);
            game_rotate(1);
            printf("UPRIGHT\n");
            break;

        case KEYB_Q:
			// manage_music(chime_select, start);
            audio_play(chime_audio);
            game_rotate(0);
            printf("UPLEFT\n");
            break;

        case KEYB_SPACE:
            // printf("FAST RUN\n");
            break;

        case KEYB_ESC:
			// manage_music(game, stop);
            menu_run(pausa_menu);
            printf("Juego Pausado\n");
            break;
            

        default:
        break;
    }
    printf("game run\n");
    game_run();
    update_display(); // estamos viendo si esto buguea la eliminacion de filas
    }
    printf("Keypress Callback Out\n");
}



void update_display(void) {
    printf("Update Display In\n");
    uint8_t parametro_nivel = param_lvl_fetch();
	uint8_t x, y;
	for(x=0; x<GAME_WIDTH ; x++)
	{
		for(y=0; y<GAME_HEIGHT ; y++)
		{
			float val = (float)game_public_matrix[y][x];
			al_draw_scaled_bitmap(image, (al_get_bitmap_width(image)/8) * val, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image),BLOCKSZ + BLOCKSZ*x, BLOCKSZ*y, BLOCKSZ, BLOCKSZ, 0);
		}
	}
    for(x=1; x<5 ; x++)
	{
		for(y=0; y<parametro_nivel ; y++)
		{
            float val= (float) game_next_block_public_matrix[y][x];
            al_draw_scaled_bitmap(image, (al_get_bitmap_width(image)/8) * val, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image),BLOCKSZ*(ANCHO+2+x), BLOCKSZ*(y+1), BLOCKSZ, BLOCKSZ, 0);
            
        }
        for(y=parametro_nivel; y<8; y++){
            al_draw_scaled_bitmap(image, 0, 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image),BLOCKSZ*(ANCHO+2+x), BLOCKSZ*(y+1), BLOCKSZ, BLOCKSZ, 0);
        }//tapo los casilleros vacios
    }//DIBUJO PIEZA SIGUIENTE

    if(text_number_drawer(score, game_get_data().score)){
        end_program();
    }
	printf("SCORE:\n%u\n", game_get_data().score);
    
    if(text_number_drawer(nivel, game_get_data().game_level)){
        end_program();
    }

	al_flip_display(); //despues de esribir toda la matriz muestro lo que escribi
    printf("Update Display Out\n");
}


int initialize_alleg(void) {
    if (!al_init()) {   // ! System
        printf( "failed to initialize allegro!\n");
        return -1;
    }

    event_queue = al_create_event_queue(); // ! event_queue
    if (!event_queue) {
        printf( "failed to create event_queue!\n");
        return -1;
    }

    if(!keyb_init(event_queue)){ // ! keyboard
        al_destroy_event_queue(event_queue);
        printf("No se pudo inicializar el teclado!\n");
        return -1;
    }


    if (!al_init_image_addon()) { // ! image_addon -    necesario para manejo de imagenes 
        al_uninstall_keyboard();        
        al_destroy_event_queue(event_queue);
        printf("failed to initialize image addon !\n");
        return -1;
    }
    if (!al_init_font_addon()) { // ! font_addon
        al_uninstall_keyboard();        
        al_destroy_event_queue(event_queue);
        printf("failed to initialize font addon !\n");
        return -1;
    }
    if (!al_init_primitives_addon()) { // ! primitives_addon
        al_uninstall_keyboard();        
        al_destroy_event_queue(event_queue);
        printf("failed to initialize primitives addon !\n");
        return -1;
    }
    if (!al_init_ttf_addon()) {  // ! ttf addon
        al_uninstall_keyboard();        
        al_destroy_event_queue(event_queue);
        printf("failed to initialize ttf addon !\n");
        return -1;
    }

    image = al_load_bitmap("./frontend/images/piezastetris.png");
    if (!image) {
        al_uninstall_keyboard();        
        al_destroy_event_queue(event_queue);

        printf( "failed to load image !\n");
        return -1;
    }

    muroH = al_load_bitmap("./frontend/images/muroH.jpg");
    if (!muroH) {
        al_destroy_bitmap(image);

        al_uninstall_keyboard();        
        al_destroy_event_queue(event_queue);
        printf( "failed to load muroH !\n");

        return -1;
    }
    muroV = al_load_bitmap("./frontend/images/muroV.jpg");
    if (!muroV) {
        al_destroy_bitmap(muroH);
        al_destroy_bitmap(image);

        al_uninstall_keyboard();        
        al_destroy_event_queue(event_queue);
        printf("failed to load muroV !\n");
        return -1;
    }
    pieza_blanca = al_load_bitmap("./frontend/images/white_tile.png");
    if (!pieza_blanca) {
        al_destroy_bitmap(muroV);
        al_destroy_bitmap(muroH);
        al_destroy_bitmap(image);

        al_uninstall_keyboard();        
        al_destroy_event_queue(event_queue);
        printf("failed to load pieza_blanca !\n");
        return -1;
    }
    tetris_cartel = al_load_bitmap("./frontend/images/tetris_cartel.png");
    if (!tetris_cartel) {
        al_destroy_bitmap(pieza_blanca);
        al_destroy_bitmap(muroV);
        al_destroy_bitmap(muroH);
        al_destroy_bitmap(image);

        al_uninstall_keyboard();        
        al_destroy_event_queue(event_queue);
        printf("failed to load tetris_cartel !\n");
        return -1;
    }
    diagrama_teclado = al_load_bitmap("./frontend/images/controles.png");
    if (!diagrama_teclado) {
        al_destroy_bitmap(tetris_cartel);
        al_destroy_bitmap(pieza_blanca);
        al_destroy_bitmap(muroV);
        al_destroy_bitmap(muroH);
        al_destroy_bitmap(image);

        al_uninstall_keyboard();        
        al_destroy_event_queue(event_queue);
        printf("failed to load diagrama_teclado !\n");
        return -1;
    }
    jugabilidad_diagrama = al_load_bitmap("./frontend/images/instrucciones.png");
    if (!jugabilidad_diagrama) {
        al_destroy_bitmap(diagrama_teclado);
        al_destroy_bitmap(tetris_cartel);
        al_destroy_bitmap(pieza_blanca);
        al_destroy_bitmap(muroV);
        al_destroy_bitmap(muroH);
        al_destroy_bitmap(image);

        al_uninstall_keyboard();        
        al_destroy_event_queue(event_queue);
        printf("failed to load jugabilidad_diagrama !\n");
        return -1;
    }
        gameoverfoto = al_load_bitmap("./frontend/images/gameover.png");
    if (!gameoverfoto) {
        al_destroy_bitmap(jugabilidad_diagrama);
        al_destroy_bitmap(diagrama_teclado);
        al_destroy_bitmap(tetris_cartel);
        al_destroy_bitmap(pieza_blanca);
        al_destroy_bitmap(muroV);
        al_destroy_bitmap(muroH);
        al_destroy_bitmap(image);
        
        al_uninstall_keyboard();        
        al_destroy_event_queue(event_queue);
        printf("failed to load jugabilidad_diagrama !\n");
        return -1;
    }

    display = al_create_display(((ANCHO+8)*BLOCKSZ), ((ALTO+1)*BLOCKSZ));
    if (!display) {
        al_destroy_bitmap(jugabilidad_diagrama);
        al_destroy_bitmap(diagrama_teclado);
        al_destroy_bitmap(tetris_cartel);
        al_destroy_bitmap(pieza_blanca);
        al_destroy_bitmap(muroV);
        al_destroy_bitmap(muroH);
        al_destroy_bitmap(image);
        al_destroy_bitmap(gameoverfoto);

        al_uninstall_keyboard();        
        al_destroy_event_queue(event_queue);
        printf("failed to create display!\n");
        return -1;
    }
    al_register_event_source(event_queue, al_get_display_event_source(display));
    return 0;
}

void  initialize_display_game (void){
    blocktext_t * pieza_sig = text_init_alleg(TXT_BLANCO, 30, "PIEZA  SIGUIENTE", PATH_TTF, BLOCKSZ*(ANCHO+2.5), BLOCKSZ/4, ALINEADO_IZQUIERDA );
    blocktext_t * puntaje = text_init_alleg(TXT_BLANCO, 40, "PUNTAJE:", PATH_TTF, BLOCKSZ*(ANCHO+2.5), YPOS_SCORE, ALINEADO_IZQUIERDA );
    blocktext_t * lvl_game = text_init_alleg(TXT_BLANCO, 40, "NIVEL:", PATH_TTF, BLOCKSZ*(ANCHO+2.5), YPOS_NIVEL, ALINEADO_IZQUIERDA );
    
    score = text_init_alleg( TXT_BLANCO, 40, "", PATH_TTF, BLOCKSZ*(ANCHO+2.5), YPOS_SCORE+BLOCKSZ, ALINEADO_IZQUIERDA );
    nivel = text_init_alleg(TXT_BLANCO, 40, "", PATH_TTF, BLOCKSZ*(ANCHO+2.5), YPOS_NIVEL+BLOCKSZ, ALINEADO_IZQUIERDA );

    if(puntaje==NULL || pieza_sig==NULL || lvl_game==NULL){
        printf("problema con titulo de initialize_display_game");   
    }


    al_clear_to_color(al_map_rgb(0,0,0));
    if(text_drawer(pieza_sig)){
        text_destroy(pieza_sig);
        end_program();//si hay algun error salgo del programa
    }//Escribo pieza siguiente en pantalla

    if(text_drawer(puntaje)){
        text_destroy(puntaje);
        end_program();//si hay algun error salgo del programa
    }//Escribo pieza siguiente en pantalla
    if(text_drawer(lvl_game)){
        text_destroy(lvl_game);
        end_program();//si hay algun error salgo del programa
    }//Escribo pieza siguiente en pantalla    

    text_number_drawer(nivel, game_get_data().game_level);
    
   
    //ahora dibujo el muro horizontal
    al_draw_scaled_bitmap(muroH, 0, 0, al_get_bitmap_width(muroH), al_get_bitmap_height(muroH), 0, BLOCKSZ*(ALTO), ((ANCHO+2)*BLOCKSZ), BLOCKSZ, 0);
    //empiezo uso la ultima fila
    //dibujo las paredes
    al_draw_scaled_bitmap(muroV, 0, 0, al_get_bitmap_width(muroV), al_get_bitmap_height(muroV), 0, 0, BLOCKSZ, al_get_display_height(display)-BLOCKSZ, 0);
    al_draw_scaled_bitmap(muroV, 0, 0, al_get_bitmap_width(muroV), al_get_bitmap_height(muroV),((ANCHO+1)*BLOCKSZ) , 0, BLOCKSZ, al_get_display_height(display)-BLOCKSZ, 0);
        
    
    al_flip_display();
    
    //playAudio();
    text_destroy(pieza_sig);
    text_destroy(puntaje);  
    text_destroy(lvl_game);    //libero la memoria dinamica

}


void end_program (void){


    printf("destruyendo bitmaps...\n");
// DESTRUCCION DE BITMAPS
    al_destroy_bitmap(image);
    al_destroy_bitmap(muroH);
    al_destroy_bitmap(muroV);
    al_destroy_bitmap(pieza_blanca);    
    al_destroy_bitmap(tetris_cartel);
    al_destroy_bitmap(diagrama_teclado);
    al_destroy_bitmap(jugabilidad_diagrama);
    al_destroy_bitmap(gameoverfoto);

// DESTRUCCION DE OTRAS UTILIDADES ALLEGRO
    printf("destruyendo utils...\n");
    al_uninstall_keyboard();        
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    audio_destroy();
    font_destroyer();

//DESTRUCCION DE UTILIDADES EXTRA
    printf("destruyendo menus y texto...\n");
    menu_destroy(principal_menu);
    menu_destroy(pausa_menu);
    menu_destroy(gameover_menu);
    text_destroy(nivel);
    text_destroy(score);

    al_uninstall_system();
    printf("Game Ended\n");
    exit(0);
}
uint8_t param_lvl_fetch (void){
    uint8_t temporal=game_get_data().game_level; 
    switch (temporal)
    {
    case 1:
    case 2:
    case 3:
        temporal=8;
        break;
/*        temporal=7;
        break;
        temporal=4;
        break;*/
    case 4:
    case 5:
        temporal=5;
        break;
    case 6:
        temporal=2;
        break;
    case 7:
    case 8:
        temporal=0;
        break;

    default:
        temporal=8;
        break;
    }
    return temporal;
}
/*void deleteline (int numfil){
    int x;
    int reductor;
    float angulo;
        for(x=1; x<=GAME_WIDTH; x++){
            al_draw_scaled_bitmap(image,(al_get_bitmap_width(image)/8), 0, (al_get_bitmap_width(image)/8), al_get_bitmap_height(image), BLOCKSZ*x, BLOCKSZ*numfil, BLOCKSZ, BLOCKSZ, 0);
        }  //pongo el fondo en negro
        for(reductor=1, angulo=(3,1415/8); reductor<10; reductor++, angulo+=(3,1415/8)){
            for(x=1; x<=GAME_WIDTH; x++){
                al_draw_tinted_scaled_rotated_bitmap_region(whitepiece, 0, 0, al_get_bitmap_width(whitepiece), al_get_bitmap_height(whitepiece), al_map_rgba_f(1, 1, 1, 1), al_get_bitmap_width(whitepiece)/2,al_get_bitmap_height(whitepiece), (BLOCKSZ/2 +BLOCKSZ*x), (BLOCKSZ/2 +BLOCKSZ*numfil),1/reductor, 1/reductor, angulo, 0);
                //se va haciendo mas chia a medida que rota
            }
        }
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////AUDIO/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

