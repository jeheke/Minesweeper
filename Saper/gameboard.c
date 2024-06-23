/*****************************************************************//**
 * \file   gameboard.h
 * \brief  Functions for rendering the game graphics and managing game state updates.
 *********************************************************************/

#include "gameboard.h"

 /**
  * \brief Draws the game board and updates the display.
  * \param game Pointer to the Game structure.
  */
void draw_board(Game* game) {
    al_clear_to_color(al_map_rgb(255, 255, 255));

    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            int x = game->start_x + j * CELL_SIZE;
            int y = game->start_y + i * CELL_SIZE;

            al_draw_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, al_map_rgb(0, 0, 0), 2);

            if (game->revealed[i][j]) {
                if (game->board[i][j] == -1) { // Revealing a cell with mine
                    al_draw_filled_rectangle(x + 1, y + 1, x + CELL_SIZE - 1, y + CELL_SIZE - 1, al_map_rgb(255, 0, 0));
                    al_draw_scaled_bitmap(game->bomb_image, 0, 0, al_get_bitmap_width(game->bomb_image), al_get_bitmap_height(game->bomb_image), x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, 0);
                }
                else {
                    al_draw_filled_rectangle(x + 1, y + 1, x + CELL_SIZE - 1, y + CELL_SIZE - 1, al_map_rgb(192, 192, 192));
                    if (game->board[i][j] > 0) {
                        switch (game->board[i][j]) { // Choosing a color based on how many mines are adjacent to the cell
                        case 1: game->color = al_map_rgb(0, 0, 255); break;
                        case 2: game->color = al_map_rgb(0, 128, 0); break;
                        case 3: game->color = al_map_rgb(255, 0, 0); break;
                        case 4: game->color = al_map_rgb(0, 0, 128); break;
                        case 5: game->color = al_map_rgb(128, 0, 0); break;
                        case 6: game->color = al_map_rgb(0, 128, 128); break;
                        case 7: game->color = al_map_rgb(0, 0, 0); break;
                        case 8: game->color = al_map_rgb(128, 128, 128); break;
                        default: game->color = al_map_rgb(0, 0, 0); break;
                        }
                        al_draw_textf(game->small_font, game->color, x + CELL_SIZE / 2, y + CELL_SIZE / 2 - al_get_font_ascent(game->small_font) / 2, ALLEGRO_ALIGN_CENTER, "%d", game->board[i][j]);
                    }
                }
            }
            else if (game->flagged[i][j]) { // Flagging a cell
                al_draw_scaled_bitmap(game->flag_image, 0, 0, al_get_bitmap_width(game->flag_image), al_get_bitmap_height(game->flag_image), x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, 0);
            }
        }
    }
    update_timer(game);
    al_flip_display();
}

/**
 * \brief Reveals the cell at the specified coordinates.
 * \param i The row index of the cell.
 * \param j The column index of the cell.
 * \param game Pointer to the Game structure.
 */
void reveal_cell(int i, int j, Game* game) {
    if (i < 0 || i >= game->rows || j < 0 || j >= game->cols || game->revealed[i][j] || game->flagged[i][j]) {
        return;
    }

    game->revealed[i][j] = true;

    if (game->board[i][j] == 0) { // Reveal all empty adjacent cells
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                if (x == 0 && y == 0)
                    continue;
                reveal_cell(i + x, j + y, game);
            }
        }
    }

    if (game->board[i][j] == -1) { // Game over condition
        game->game_over = true;
    }
}

/**
 * \brief Toggles the flag state of the cell at the specified coordinates.
 * \param row The row index of the cell.
 * \param col The column index of the cell.
 * \param game Pointer to the Game structure.
 */
void toggle_flag(int row, int col, Game* game) {
    if (!game->revealed[row][col]) {
        game->flagged[row][col] = !game->flagged[row][col];
    }
}

/**
 * \brief Checks if the game is won.
 * \param game Pointer to the Game structure.
 */
void check_game_won(Game* game) {
    int revealed_count = 0;
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if (game->revealed[i][j]) {
                revealed_count++;
            }
        }
    }

    if (revealed_count == game->rows * game->cols - game->mines) { // Win condition
        game->game_won = true;
    }
}

/**
 * \brief Updates the game timer.
 * \param game Pointer to the Game structure.
 */
void update_timer(Game* game) {
    if (!game->game_over && !game->game_won) {
        game->elapsed_time = al_get_timer_count(game->timer) / 60.0;
    }
    char buffer[50];
    sprintf_s(buffer, sizeof(buffer), "%.2f", game->elapsed_time);
    al_draw_text(game->small_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, game->start_y - 50, ALLEGRO_ALIGN_CENTER, buffer);
}
