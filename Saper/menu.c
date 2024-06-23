/*****************************************************************//**
 * \file   menu.c
 * \brief  Functions for displaying game menus.
 *********************************************************************/

#include "menu.h"
#include "gameboard.h"
#include "game.h"

 /**
  * \brief Displays the main menu of the game.
  * \param game Pointer to the Game structure.
  */
void show_main_menu(Game* game) {
    while (true) {
        al_clear_to_color(al_map_rgb(255, 255, 255));

        al_draw_text(game->big_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5, ALLEGRO_ALIGN_CENTER, "Minesweeper");

        // Main menu buttons heights
        float play_y = SCREEN_HEIGHT / 2 - 75;
        float how_to_play_y = SCREEN_HEIGHT / 2;
        float exit_y = SCREEN_HEIGHT / 2 + 75;
        //

        // Calculate the width of texts
        float play_width = al_get_text_width(game->medium_font, "Play");
        float how_to_play_width = al_get_text_width(game->medium_font, "How to Play");
        float exit_width = al_get_text_width(game->medium_font, "Exit");
        //

        // Drawing main menu texts
        al_draw_text(game->medium_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 75, ALLEGRO_ALIGN_CENTER, "Play");
        al_draw_text(game->medium_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "How to Play");
        al_draw_text(game->medium_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 75, ALLEGRO_ALIGN_CENTER, "Exit");
        //

        al_flip_display();

        ALLEGRO_EVENT event;
        al_wait_for_event(game->event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { // Closing the game by clicking the X button
            cleanup_resources(game);
            exit(0);
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.mouse.button & 1) {
                int x = event.mouse.x;
                int y = event.mouse.y;

                if (x >= SCREEN_WIDTH / 2 - play_width / 2 && x <= SCREEN_WIDTH / 2 + play_width / 2 && y >= play_y && y <= play_y + al_get_font_line_height(game->medium_font)) { // Difficulty menu button
                    // Difficulty selection
                    int difficulty = show_difficulty_menu(game);
                    if (difficulty == 0)
                        continue;

                    // Game initialization
                    initialize_game(difficulty, game);
                    game->game_over = false;
                    game->game_won = false;

                    al_set_timer_count(game->timer, 0);
                    //

                    while (!game->game_over && !game->game_won) { // Main game loop
                        ALLEGRO_EVENT event;
                        al_wait_for_event(game->event_queue, &event);

                        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { // Closing the game by clicking the X button
                            cleanup_resources(game);
                            exit(0);
                        }
                        else if (event.type == ALLEGRO_EVENT_TIMER) { // Drawing main game board
                            draw_board(game);
                        }
                        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) { // Revealing a cell
                            if (event.mouse.button & 1) {
                                int col = (event.mouse.x - game->start_x) / CELL_SIZE;
                                int row = (event.mouse.y - game->start_y) / CELL_SIZE;
                                if (col >= 0 && col < game->cols && row >= 0 && row < game->rows) {
                                    reveal_cell(row, col, game);
                                    check_game_won(game);
                                }
                            }
                            else if (event.mouse.button & 2) { // Placing a flag
                                int col = (event.mouse.x - game->start_x) / CELL_SIZE;
                                int row = (event.mouse.y - game->start_y) / CELL_SIZE;
                                if (col >= 0 && col < game->cols && row >= 0 && row < game->rows) {
                                    toggle_flag(row, col, game);
                                }
                            }
                        }
                    }
                    show_game_over_screen(game);
                }
                else if (x >= SCREEN_WIDTH / 2 - how_to_play_width / 2 && x <= SCREEN_WIDTH / 2 + how_to_play_width / 2 && y >= how_to_play_y && y <= how_to_play_y + al_get_font_line_height(game->medium_font)) { // How to play menu
                    show_how_to_play(game);
                }
                else if (x >= SCREEN_WIDTH / 2 - exit_width / 2 && x <= SCREEN_WIDTH / 2 + exit_width / 2 && y >= exit_y && y <= exit_y + al_get_font_line_height(game->medium_font)) { // Exit game button
                    cleanup_resources(game);
                    exit(0);
                }
            }
        }
    }
}

/**
 * \brief Displays the "How to Play" menu.
 * \param game Pointer to the Game structure.
 */
void show_how_to_play(Game* game) {
    while (true) {
        al_clear_to_color(al_map_rgb(255, 255, 255));

        al_draw_text(game->medium_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, ALLEGRO_ALIGN_CENTER, "How to Play");
        al_draw_text(game->medium_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, ALLEGRO_ALIGN_CENTER, "Left click to reveal a cell.");
        al_draw_text(game->medium_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, ALLEGRO_ALIGN_CENTER, "Right click to flag a cell.");
        al_draw_text(game->medium_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Avoid mines to win the game.");
        al_draw_text(game->medium_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100, ALLEGRO_ALIGN_CENTER, "Click to return to main menu");

        al_flip_display();

        ALLEGRO_EVENT event;
        al_wait_for_event(game->event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { // Closing the game by clicking the X button
            cleanup_resources(game);
            exit(0);
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) { // Backing to main menu
            return;
        }
    }
}

/**
 * \brief Displays the difficulty selection menu.
 * \param game Pointer to the Game structure.
 * \return The selected difficulty level (1 = easy, 2 = medium, 3 = hard).
 */
int show_difficulty_menu(Game* game) {
    while (true) {
        al_clear_to_color(al_map_rgb(255, 255, 255));
        al_draw_text(game->medium_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, ALLEGRO_ALIGN_CENTER, "Select Difficulty");

        // Texts heights
        int easy_y = SCREEN_HEIGHT / 2 - 100;
        int medium_y = SCREEN_HEIGHT / 2;
        int hard_y = SCREEN_HEIGHT / 2 + 100;
        int return_y = SCREEN_HEIGHT / 2 + 250;
        //


        al_draw_text(game->medium_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, easy_y, ALLEGRO_ALIGN_CENTER, "Easy (8x8, 10 mines)");
        al_draw_text(game->medium_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, medium_y, ALLEGRO_ALIGN_CENTER, "Medium (12x12, 20 mines)");
        al_draw_text(game->medium_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, hard_y, ALLEGRO_ALIGN_CENTER, "Hard (16x16, 40 mines)");
        al_draw_text(game->medium_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, return_y, ALLEGRO_ALIGN_CENTER, "Return to Main Menu");
        al_flip_display();

        ALLEGRO_EVENT event;
        al_wait_for_event(game->event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { // Closing the game by clicking the X button
            cleanup_resources(game);
            exit(0);
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.mouse.button & 1) {
                int x = event.mouse.x;
                int y = event.mouse.y;

                // Calculate font height
                int font_height = al_get_font_line_height(game->medium_font);

                // Calculate the width of texts
                int easy_width = al_get_text_width(game->medium_font, "Easy (8x8, 10 mines)");
                int medium_width = al_get_text_width(game->medium_font, "Medium (12x12, 20 mines)");
                int hard_width = al_get_text_width(game->medium_font, "Hard (16x16, 40 mines)");
                int return_width = al_get_text_width(game->medium_font, "Return to Main Menu");
                //

                if (y >= easy_y && y <= easy_y + font_height && x >= SCREEN_WIDTH / 2 - easy_width / 2 && x <= SCREEN_WIDTH / 2 + easy_width / 2)
                    return 1; // Easy mode
                else if (y >= medium_y && y <= medium_y + font_height && x >= SCREEN_WIDTH / 2 - medium_width / 2 && x <= SCREEN_WIDTH / 2 + medium_width / 2)
                    return 2; // Medium mode
                else if (y >= hard_y && y <= hard_y + font_height && x >= SCREEN_WIDTH / 2 - hard_width / 2 && x <= SCREEN_WIDTH / 2 + hard_width / 2)
                    return 3; // Hard mode
                else if (y >= return_y && y <= return_y + font_height && x >= SCREEN_WIDTH / 2 - return_width / 2 && x <= SCREEN_WIDTH / 2 + return_width / 2)
                    return 0; // Return to main menu
            }
        }
    }
}

/**
 * \brief Displays the game over screen.
 * \param game Pointer to the Game structure.
 */
void show_game_over_screen(Game* game) {
    while (true) {
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
                    if (game->board[i][j] != -1) {
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
                else if (game->board[i][j] == -1) { // Drawing all the other bombs after lose
                    al_draw_filled_rectangle(x + 1, y + 1, x + CELL_SIZE - 1, y + CELL_SIZE - 1, al_map_rgb(255, 0, 0));
                    al_draw_scaled_bitmap(game->bomb_image, 0, 0, al_get_bitmap_width(game->bomb_image), al_get_bitmap_height(game->bomb_image), x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, 0);
                }
                else if (game->flagged[i][j]) { // Flagging a cell
                    al_draw_scaled_bitmap(game->flag_image, 0, 0, al_get_bitmap_width(game->flag_image), al_get_bitmap_height(game->flag_image), x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, 0);
                }
            }
        }

        update_timer(game);

        if (game->game_over) { // Losing text
            al_draw_text(game->medium_font, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, game->start_y - 150, ALLEGRO_ALIGN_CENTER, "Game Over!");
        }
        else if (game->game_won) { // Winning text
            al_draw_text(game->medium_font, al_map_rgb(0, 255, 0), SCREEN_WIDTH / 2, game->start_y - 150, ALLEGRO_ALIGN_CENTER, "You Win!");
        }

        int button_y = game->start_y + game->rows * CELL_SIZE + 20;

        // Return to main menu button
        al_draw_text(game->medium_font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, button_y, ALLEGRO_ALIGN_CENTER, "Click to return to main menu");

        al_flip_display();

        ALLEGRO_EVENT event;
        al_wait_for_event(game->event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { // Closing the game by clicking the X button
            cleanup_resources(game);
            exit(0);
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) { // Returning to main menu
            if (event.mouse.button & 1) {
                int x = event.mouse.x;
                int y = event.mouse.y;

                int button_width = al_get_text_width(game->medium_font, "Click to return to main menu");
                int button_height = al_get_font_ascent(game->medium_font);

                if (y >= button_y && y <= button_y + button_height && x >= SCREEN_WIDTH / 2 - button_width / 2 && x <= SCREEN_WIDTH / 2 + button_width / 2) {
                    return;
                }
            }
        }
    }
}
