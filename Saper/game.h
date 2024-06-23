/*****************************************************************//**
 * \file   game.h
 * \brief  Definitions and functions related to the Game structure and game state management.
 *********************************************************************/

#pragma once

#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_primitives.h"
#include "utils.h"

 /**
 * \typedef Game
 * \brief Represents the state and resources of the game.
 */

typedef struct Game {
    int rows; /**< Number of rows in the game board. */
    int cols; /**< Number of columns in the game board. */
    int mines; /**< Number of mines in the game board. */
    int** board; /**< 2D array representing the game board. -1 indicates a mine, other values indicate the number of adjacent mines. */
    bool** revealed; /**< 2D array indicating if a cell is revealed. */
    bool** flagged; /**< 2D array indicating if a cell is flagged. */
    bool game_over; /**< Indicates if the game is over. */
    bool game_won; /**< Indicates if the game is won. */
    int start_x; /**< Starting x-coordinate for rendering the game board. */
    int start_y; /**< Starting y-coordinate for rendering the game board. */
    double elapsed_time; /**< Time elapsed since the game started. */
    ALLEGRO_DISPLAY* display; /**< Pointer to the Allegro display. */
    ALLEGRO_EVENT_QUEUE* event_queue; /**< Pointer to the Allegro event queue. */
    ALLEGRO_TIMER* timer; /**< Pointer to the Allegro timer. */
    ALLEGRO_FONT* small_font; /**< Pointer to the small font used in the game. */
    ALLEGRO_FONT* medium_font; /**< Pointer to the medium font used in the game. */
    ALLEGRO_FONT* big_font; /**< Pointer to the big font used in the game. */
    ALLEGRO_BITMAP* flag_image; /**< Pointer to the flag image bitmap. */
    ALLEGRO_BITMAP* bomb_image; /**< Pointer to the bomb image bitmap. */
    ALLEGRO_COLOR color; /**< Color used for drawing text and graphics. */
} Game;

void initialize_game_state(Game* game, ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_TIMER* timer, ALLEGRO_FONT* small_font, ALLEGRO_FONT* medium_font, ALLEGRO_FONT* big_font, ALLEGRO_BITMAP* flag_image, ALLEGRO_BITMAP* bomb_image);
void initialize_game(int difficulty, Game* game);
void cleanup_resources(Game* game);


