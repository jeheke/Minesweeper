/*****************************************************************//**
 * \file   main.c
 * \brief  The main entry point for the Minesweeper game.
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"
#include "utils.h"
#include "menu.h"
#include "gameboard.h"

 /**
  * \brief The main function of the game.
  * \return 0 on success, non-zero on failure.
  */

int main() {
    srand(time(NULL));

    if (!al_init()) {
        fprintf(stderr, "Failed to initialize Allegro.\n");
        return -1;
    }

    // Allegro addons initialization
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    //

    // Allegro structures initialization
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    ALLEGRO_FONT* small_font = al_load_ttf_font("smallFont.ttf", 24, 0);
    ALLEGRO_FONT* medium_font = al_load_ttf_font("mediumFont.ttf", 50, 0);
    ALLEGRO_FONT* big_font = al_load_ttf_font("bigFont.ttf", 80, 0);
    ALLEGRO_BITMAP* flag_image = al_load_bitmap("flag.png");
    ALLEGRO_BITMAP* bomb_image = al_load_bitmap("bomb.png");
    //

    // Allegro structures setup
    al_set_window_title(display, "Minesweeper");
    al_set_display_icon(display, al_load_bitmap("bomb.png"));

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);
    //

    // Initialization of game resources
    Game game;
    initialize_game_state(&game, display, event_queue, timer, small_font, medium_font, big_font, flag_image, bomb_image);
    //

    while (true) {
        // Showing main game menu
        show_main_menu(&game);
    }

    // Cleaning all resources used by the game.
    cleanup_resources(&game);
    //
    return 0;
}
