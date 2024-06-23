/*****************************************************************//**
 * \file   game.c
 * \brief  Functions for initializing, managing, and cleaning up the game.
 *********************************************************************/

#include <stdlib.h>
#include <time.h>
#include "game.h"

 /**
  * \brief Initializes the game with the provided Allegro resources.
  * \param game Pointer to the Game structure.
  * \param display Pointer to the Allegro display.
  * \param event_queue Pointer to the Allegro event queue.
  * \param timer Pointer to the Allegro timer.
  * \param small_font Pointer to the small font used in the game.
  * \param medium_font Pointer to the medium font used in the game.
  * \param big_font Pointer to the big font used in the game.
  * \param flag_image Pointer to the flag image bitmap.
  * \param bomb_image Pointer to the bomb image bitmap.
  */
void initialize_game_state(Game* game, ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_TIMER* timer, ALLEGRO_FONT* small_font, ALLEGRO_FONT* medium_font, ALLEGRO_FONT* big_font, ALLEGRO_BITMAP* flag_image, ALLEGRO_BITMAP* bomb_image) {
    game->display = display;
    game->event_queue = event_queue;
    game->timer = timer;
    game->small_font = small_font;
    game->medium_font = medium_font;
    game->big_font = big_font;
    game->flag_image = flag_image;
    game->bomb_image = bomb_image;
    game->game_over = false;
    game->game_won = false;
}

/**
 * \brief Initializes the game board based on the selected difficulty.
 * \param difficulty The difficulty level of the game (1 = easy, 2 = medium, 3 = hard).
 * \param game Pointer to the Game structure.
 */
void initialize_game(int difficulty, Game* game) {
    switch (difficulty) {
    case 1:
        game->rows = 8;
        game->cols = 8;
        game->mines = 10;
        break;
    case 2:
        game->rows = 12;
        game->cols = 12;
        game->mines = 20;
        break;
    case 3:
        game->rows = 16;
        game->cols = 16;
        game->mines = 40;
        break;
    }

    game->start_x = (SCREEN_WIDTH - (game->cols * CELL_SIZE)) / 2;
    game->start_y = (SCREEN_HEIGHT - (game->rows * CELL_SIZE)) / 2;

    // Dynamically allocating memory for game boards 
    game->board = (int**)malloc(game->rows * sizeof(int*));
    for (int i = 0; i < game->rows; i++) {
        game->board[i] = (int*)calloc(game->cols, sizeof(int));
    }

    game->revealed = (bool**)malloc(game->rows * sizeof(bool*));
    for (int i = 0; i < game->rows; i++) {
        game->revealed[i] = (bool*)calloc(game->cols, sizeof(bool));
    }

    game->flagged = (bool**)malloc(game->rows * sizeof(bool*));
    for (int i = 0; i < game->rows; i++) {
        game->flagged[i] = (bool*)calloc(game->cols, sizeof(bool));
    }
    //

    // Randomly placing mines on the board
    int placed_mines = 0;
    while (placed_mines < game->mines) {
        int i = rand() % game->rows;
        int j = rand() % game->cols;
        if (game->board[i][j] != -1) {
            game->board[i][j] = -1;
            placed_mines++;
        }
    }
    //

    // Setting up the number of mines surrounding each cell
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if (game->board[i][j] == -1) {
                continue;
            }
            if (i - 1 >= 0 && game->board[i - 1][j] == -1)
                game->board[i][j]++;
            if (i + 1 < game->rows && game->board[i + 1][j] == -1)
                game->board[i][j]++;
            if (i - 1 >= 0 && j - 1 >= 0 && game->board[i - 1][j - 1] == -1)
                game->board[i][j]++;
            if (i + 1 < game->rows && j - 1 >= 0 && game->board[i + 1][j - 1] == -1)
                game->board[i][j]++;
            if (i - 1 >= 0 && j + 1 < game->cols && game->board[i - 1][j + 1] == -1)
                game->board[i][j]++;
            if (i + 1 < game->rows && j + 1 < game->cols && game->board[i + 1][j + 1] == -1)
                game->board[i][j]++;
            if (j - 1 >= 0 && game->board[i][j - 1] == -1)
                game->board[i][j]++;
            if (j + 1 < game->cols && game->board[i][j + 1] == -1)
                game->board[i][j]++;
        }
    }
    //

    game->elapsed_time = 0.0;
}

/**
 * \brief Cleans up all resources used by the game.
 * \param game Pointer to the Game structure.
 */
void cleanup_resources(Game* game) {
    if (game->board) {
        for (int i = 0; i < game->rows; i++) {
            free(game->board[i]);
        }
        free(game->board);
    }

    if (game->revealed) {
        for (int i = 0; i < game->rows; i++) {
            free(game->revealed[i]);
        }
        free(game->revealed);
    }

    if (game->flagged) {
        for (int i = 0; i < game->rows; i++) {
            free(game->flagged[i]);
        }
        free(game->flagged);
    }

    if (game->event_queue) {
        al_destroy_event_queue(game->event_queue);
    }

    if (game->timer) {
        al_destroy_timer(game->timer);
    }

    if (game->display) {
        al_destroy_display(game->display);
    }

    if (game->small_font) {
        al_destroy_font(game->small_font);
    }

    if (game->medium_font) {
        al_destroy_font(game->medium_font);
    }

    if (game->big_font) {
        al_destroy_font(game->big_font);
    }

    if (game->flag_image) {
        al_destroy_bitmap(game->flag_image);
    }

    if (game->bomb_image) {
        al_destroy_bitmap(game->bomb_image);
    }

}
