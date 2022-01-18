#include <stdio.h>
#include <assert.h>

#include "./backend/game.h"
#include "./backend/menu.h"
#include "./testing/joystick.h"
#include "./testing/easy_timer.h"
#include "./testing/matrix_handler.h"
#include "./testing/rpi_display.h"

void key_press_callback(uint8_t key);
void update_game_display(void);
void main_game_start(void);
void update_menu_display(void);

menu_t *main_menu = NULL;
menu_t *pause_menu = NULL;

void exit_game(void){
    game_init();
    main_menu->state = MENU_IDLE;
    pause_menu->state = MENU_CLOSE;
    menu_current_menu = main_menu;
}

void restart_game(void){
    menu_close_current();
    game_start();
}

int main(void){

    printf("Inicializando...\n");

    rpi_init_display();
    game_init();

    dpad_init();
    dpad_on_press(key_press_callback);
    dpad_use_press_callback_for_longpress(DPAD_DOWN);
    dpad_use_press_callback_for_longpress(DPAD_LEFT);
    dpad_use_press_callback_for_longpress(DPAD_RIGHT);

    main_menu = menu_init(4, "MENU", NULL, MENU_ACTION_DO_NOTHING);
    assert(menu_initialized(main_menu));
    menu_set_option(main_menu, 0, "JUGAR", main_game_start);
    menu_set_option(main_menu, 1, "CARGAR PARTIDA", NULL);
    menu_set_option(main_menu, 2, "TOP SCORES", NULL);
    menu_set_option(main_menu, 3, "SALIR", menu_close_current);

    pause_menu = menu_init(4, "PAUSA", NULL, MENU_ACTION_EXIT);
    assert(menu_initialized(pause_menu));
    menu_set_option(pause_menu, 0, "REANUDAR", menu_close_current);
    menu_set_option(pause_menu, 1, "GUARDAR", NULL);
    menu_set_option(pause_menu, 2, "REINICIAR", restart_game);
    menu_set_option(pause_menu, 3, "SALIR", exit_game);


    menu_set_event_listener_display(dpad_read, update_menu_display);

    menu_run(main_menu);

    menu_destroy(main_menu);
    menu_destroy(pause_menu);
    rpi_clear_display();

    return 0;
}

void update_menu_display(void){
    printf("\n\n\n\n\n\n\n");
    printf(menu_current_menu->title);
    putchar('\n');

    uint8_t id;
    for(id=0; id<menu_current_menu->n_options; id++){
        if(menu_current_menu->current_option == id)
            putchar('>');
        else
            putchar(' ');

        if(menu_current_menu->option[id].text == NULL)
            printf("null");
        else
            printf(menu_current_menu->option[id].text);
        putchar('\n');
    }
}

void main_game_start(void){

    game_data_t game_data;
    uint64_t lastMillis;

    game_start();
    while ((game_data = game_get_data()).state != GAME_QUIT)
    {
        dpad_read();
        
        if(game_data.state == GAME_RUN && get_millis()-lastMillis >= game_data.speed_interval){
            if(game_data.id == 0)
                game_insert_block(id_next_block[0]);

            else
                game_move_down();
            game_run();
            update_game_display();
            lastMillis = get_millis();
        }

        if(game_data.state == GAME_LOSE){
            printf("Perdiste! The Game\n");
            break;
            #warning BREAK HARDCODEADO
        }
    }
}

void update_game_display(void){
    matrix_hand_t mat_handler;
    assert(mat_init(&mat_handler, HEIGHT, WIDTH));
    MAT_COPY_FROM_2D_ARRAY(&mat_handler, game_public_matrix, HEIGHT, WIDTH);
    printf("MAT_HANDLER:\n");
    rpi_copyToDis(&mat_handler, 0, 0);

	matrix_hand_t public_next_mat;
	assert(mat_init(&public_next_mat, 12, 4));
	MAT_COPY_FROM_2D_ARRAY(&public_next_mat, next_block_public_matrix, 12,4);
	rpi_copyToDis(&public_next_mat, 0, 11);
	mat_print(&public_next_mat);

    rpi_run_display();
    printf("SCORE:\n%u\n", game_get_data().score);
}

void key_press_callback(uint8_t key){
    
    assert(menu_current_menu != NULL);
    if(menu_current_menu->state == MENU_IDLE){

        switch (key)
        {
            case DPAD_UP:
                menu_current_menu->state = MENU_UP;
                printf("menu UP\n");
                break;

            case DPAD_DOWN:
                menu_current_menu->state = MENU_DOWN;
                printf("menu DOWN\n");
                break;

            case DPAD_LEFT:
                menu_current_menu->state = MENU_EXIT;
                printf("menu LEFT\n");
                break;

            case DPAD_BTN:
                menu_current_menu->state = MENU_SELECT;
                printf("menu RIGHT\n");
                break;

            default:
                break;
        }
    }else if(game_get_data().state == GAME_RUN){
        switch (key)
        {
            case DPAD_UP:
                printf("game UP\n");
                break;

            case DPAD_DOWN:
                if(game_get_data().id == 0)
                    game_insert_block(id_next_block[0]);
                else
                    game_move_down();
                printf("game DOWN\n");
                break;

            case DPAD_LEFT:
                game_move_horizontal(0);
                printf("game LEFT\n");
                break;

            case DPAD_RIGHT:
                game_move_horizontal(1);
                printf("game RIGHT\n");
                break;

            case DPAD_UPRIGHT:
                game_rotate(1);
                printf("game UPRIGHT\n");
                break;

            case DPAD_UPLEFT:
                game_rotate(0);
                printf("game UPLEFT\n");
                break;

            case DPAD_BTN:
                menu_run(pause_menu);
                printf("game BTN\n");
                break;

            default:
                break;
        }
        game_run();
        update_game_display();
    }else{
        printf("Error state. title: %s, menu: %u, game: %u\n", menu_current_menu->title, menu_current_menu->state, game_get_data().state);
    }
}
