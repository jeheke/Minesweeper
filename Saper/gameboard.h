/*****************************************************************//**
 * \file   gameboard.h
 * \brief  Functions for rendering the game graphics and managing game state updates.
 *********************************************************************/

#pragma once

#include "game.h"

void draw_board(Game* game);
void reveal_cell(int i, int j, Game* game);
void toggle_flag(int row, int col, Game* game);
void check_game_won(Game* game);
void update_timer(Game* game);

