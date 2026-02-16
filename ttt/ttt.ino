#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "actions.h"
#include "board.h"
#include "minimax.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BTN_SELECT 2
#define BTN_DOWN 3
#define BTN_RIGHT 4

#define CTX_SELECT 0
#define CTX_YOUR_TURN 1
#define CTX_AI_THINKING 2
#define CTX_TIE 3
#define CTX_WIN 4

char user = ' ';
bool AI_turn = false;
char winner = VOID;

GameBoard board;
GameBoard prev_board; // For drawing purposes

static const uint8_t PROGMEM X_bmp[] = {
    0b00000000, 0b00000000,
    0b01000000, 0b10000000,
    0b00100001, 0b00000000,
    0b00010010, 0b00000000,
    0b00001100, 0b00000000,
    0b00001100, 0b00000000,
    0b00010010, 0b00000000,
    0b00100001, 0b00000000,
    0b01000000, 0b10000000,
    0b00000000, 0b00000000
};
static const uint8_t PROGMEM O_bmp[] = {
    0b00000000, 0b00000000,
    0b00011110, 0b00000000,
    0b00100001, 0b00000000,
    0b01000000, 0b10000000,
    0b01000000, 0b10000000,
    0b01000000, 0b10000000,
    0b01000000, 0b10000000,
    0b00100001, 0b00000000,
    0b00011110, 0b00000000,
    0b00000000, 0b00000000
};

int sel_i = 0;
int sel_j = 0;

unsigned long last_blink = 0;
bool blink_on = true;

unsigned long last_display_update = 0;
const unsigned long DISPLAY_UPDATE_INTERVAL = 50; // ~20 FPS

// Rastreamento de estado anterior para evitar redesenho desnecessário
int prev_sel_i = -1;
int prev_sel_j = -1;
char prev_user = ' ';
int prev_context = -1;
bool prev_blink_on = true;
char prev_winner = ' ';

void setup() {
    delay(500);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }
    display.display();
    delay(2000);
    print_context(CTX_SELECT);
    delay(1500);
    display.clearDisplay();
    display.display();

    pinMode(BTN_SELECT, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
    randomSeed(analogRead(A0));
}

void loop() {
    unsigned long current_time = millis();

    // Rate limiting: não atualiza display a cada iteração
    if (current_time - last_display_update < DISPLAY_UPDATE_INTERVAL) {
        return;
    }
    last_display_update = current_time;

    if (user == ' ') {
        user = select_player();
        delay(200);
    } else {
        bool game_over = board.terminal();
        char current_player = board.player();

        // ===== CONTEXT LOGIC =====
        int current_context = CTX_SELECT;
        char current_winner = VOID;

        if (game_over) {
            current_winner = board.winner();
            winner = current_winner;

            if (current_winner == VOID) {
                current_context = CTX_TIE;
            } else {
                current_context = CTX_WIN;
            }
        } else if (user == current_player) {
            current_context = CTX_YOUR_TURN;
        } else {
            current_context = CTX_AI_THINKING;
        }

        // ===== AI MOVE LOGIC =====
        if (user != current_player && !game_over) {
            if (AI_turn) {
                if (board.initial()) {
                    Action random_first_move = {
                        random(0,3),
                        random(0,3)
                    };
                    board.make_move(random_first_move);
                } else {
                    Action move = minimax(board);
                    board.make_move(move);
                }                
                AI_turn = false;
            } else {
                AI_turn = true;
            }
        }

        // ===== PLAYER MOVE LOGIC =====
        if (user == current_player && !game_over) {
            if (current_time - last_blink > 400) {
                blink_on = !blink_on;
                last_blink = current_time;
            }

            // DOWN
            if (digitalRead(BTN_DOWN) == LOW) {
                sel_i++;
                if (sel_i >= SIZE) sel_i = 0;
                delay(210);
            }

            // RIGHT
            if (digitalRead(BTN_RIGHT) == LOW) {
                sel_j++;
                if (sel_j >= SIZE) sel_j = 0;
                delay(210);
            }

            // SELECT
            if (digitalRead(BTN_SELECT) == LOW) {
                Action move = {sel_i, sel_j};
                ActionSet actions = board.possible_actions();
                if (actions.contains(move)) {
                    board.make_move(move);
                }
                delay(210);
            }
        }

        // ===== CONDITIONAL RENDERIZATION =====
        bool needs_redraw = false;

        // Verifica mudanças no tabuleiro
        if (prev_board != board) {
            needs_redraw = true;
            prev_board = board;
        }

        // Verifica mudanças de contexto
        if (current_context != prev_context || current_winner != prev_winner) {
            needs_redraw = true;
            prev_context = current_context;
            prev_winner = current_winner;
        }

        // Verifica mudanças de seleção ou pisca
        if (sel_i != prev_sel_i || sel_j != prev_sel_j || blink_on != prev_blink_on) {
            needs_redraw = true;
            prev_sel_i = sel_i;
            prev_sel_j = sel_j;
            prev_blink_on = blink_on;
        }

        // Só redesenha se houver mudanças
        if (needs_redraw) {
            display.clearDisplay();
            draw_game_board();
            print_context(current_context);

            if (user == current_player && !game_over) {
                draw_selection(sel_i, sel_j, blink_on);
            }

            display.display();
        }

        if (game_over) {
            if (digitalRead(BTN_SELECT) == LOW) {
                user = ' ';
                AI_turn = false;
                winner = VOID;
                sel_i = 0;
                sel_j = 0;
                board.clear();
                print_context(CTX_SELECT);
                delay(1500);
                return;
            }
        }
    }
}

char select_player() {
    char choice = 'X';
    bool blink = true;
    unsigned long lastBlink = 0;
    const int yText = 8;
    const int xX = 32;
    const int xO = 80;
    const int boxSize = 20;

    while (true) {
        if (millis() - lastBlink > 400) {
            blink = !blink;
            lastBlink = millis();
        }

        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);

        display.setCursor(xX, yText);
        display.print("X");

        display.setCursor(xO, yText);
        display.print("O");

        if (blink) {
            int boxX = (choice == 'X') ? xX - 4 : xO - 4;
            int boxY = yText - 4;
            display.drawRect(boxX, boxY, boxSize, boxSize, SSD1306_WHITE);
        }

        display.display();

        if (digitalRead(BTN_DOWN) == LOW) {
            choice = 'X';
            delay(200);
        }

        if (digitalRead(BTN_RIGHT) == LOW) {
            choice = 'O';
            delay(200);
        }

        if (digitalRead(BTN_SELECT) == LOW) {
            delay(200);
            return choice;
        }
    }
}

void draw_game_board() {
    const int xinit = 0;
    const int xend = 31;
    const int vert1 = 10;
    const int vert2 = 21;
    const int horz1 = 10;
    const int horz2 = 21;

    const int cell_w = vert2 - vert1;
    const int cell_h = horz2 - horz1;

    display.drawLine(vert1, 0, vert1, SCREEN_HEIGHT - 1, SSD1306_WHITE);
    display.drawLine(vert2, 0, vert2, SCREEN_HEIGHT - 1, SSD1306_WHITE);
    display.drawLine(xinit, horz1, xend, horz1, SSD1306_WHITE);
    display.drawLine(xinit, horz2, xend, horz2, SSD1306_WHITE);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int x = xinit + j * cell_w;
            int y = i * cell_h;

            if (board(i, j) == X) {
                display.drawBitmap(x, y, X_bmp, 10, 10, SSD1306_WHITE);
            } else if (board(i, j) == O){
                display.drawBitmap(x, y, O_bmp, 10, 10, SSD1306_WHITE);
            }
        }
    }
}

void print_context(int ctx) {
    display.fillRect(
        SCREEN_HEIGHT,
        0,
        SCREEN_WIDTH - SCREEN_HEIGHT,
        SCREEN_HEIGHT,
        SSD1306_BLACK
    );

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(SCREEN_HEIGHT + 15, 0);

    switch (ctx) {
        case CTX_SELECT:
            display.clearDisplay();
            display.setCursor(20, 0);
            display.print("CHOOSE A PLAYER");
            break;
        case CTX_YOUR_TURN:
            display.print("Your Turn");
            break;

        case CTX_AI_THINKING:
            display.print("AI Thinking");
            display.display();
            delay(990);
            break;

        case CTX_TIE:
            display.print("GAME TIE");
            break;

        case CTX_WIN:
            display.print("WINNER:");
            display.setTextSize(2);
            display.setCursor(SCREEN_HEIGHT + 15, 16);

            if (winner == X) {
                display.print("X");
            } else if (winner == O) {
                display.print("O");
            }
            break;

        default:
            break;
    }
    display.display();
}

void draw_selection(int i, int j, bool visible) {
    int x = j * 11;
    int y = i * 11;

    if (visible) {
        display.drawRect(x, y, 10, 10, SSD1306_WHITE);
    }
    else {
        display.drawRect(x, y, 10, 10, SSD1306_BLACK);
    }
}
