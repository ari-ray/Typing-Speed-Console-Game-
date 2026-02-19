#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define WORD_MAX_LEN 20
#define WORD_DISPLAY_LIMIT 40
#define TIME_LIMIT 60

void endGame(char all_word_list[][WORD_MAX_LEN], int count, float accuracy,
             double timeUsed, int wordcount, float wpm, float cps,
             int correct_words, int correct_letter, int wrong_letter,
             int character_count);

int main() {
  int ch; 

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  start_color();

  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);

  int count = 0;
  char all_word_list[329][WORD_MAX_LEN];
  while (count < 329 && fscanf(stdin, "%19s", all_word_list[count]) == 1) {
    count++;
  }

  //this allows restarting the game easily.
  while (1) {
    freopen("/dev/tty", "r", stdin); 

    printw("Press Enter to start\n");
    refresh();
    while (getch() != '\n')
      ;

    clear();
    printw("Select Mode:\n1.Easy\n2.Medium\n3.Hard\n(Once you start playing the game, you can quit anytime by pressing ESCAPE)");
    refresh();
    char mode;
    while ((mode = getch()) != '1' && mode != '2' && mode != '3')
      ;

    clear();
    refresh();

    int rand_word_count, min_length, max_length;
    if (mode == '1') {
      rand_word_count = 20;
      min_length = 1;
      max_length = 4;
    } else if (mode == '2') {
      rand_word_count = 40;
      min_length = 5;
      max_length = 7;
    } else {
      rand_word_count = 60;
      min_length = 8;
      max_length = WORD_MAX_LEN;
    }

    char random_word_list[rand_word_count][WORD_MAX_LEN];
    srand(time(NULL));
    int selected_count = 0;
    while (selected_count < rand_word_count) {
      int random_index = rand() % count;
      int word_length = strlen(all_word_list[random_index]);
      if (word_length >= min_length && word_length <= max_length) {
        strcpy(random_word_list[selected_count], all_word_list[random_index]);
        selected_count++;
      }
    }

    clear();
    int y = 0, x = 0;
    for (int i = 0; i < WORD_DISPLAY_LIMIT && i < rand_word_count; i++) {
      mvprintw(y, x, "%s ", random_word_list[i]);
      x += strlen(random_word_list[i]) + 1;
      if (x > COLS - WORD_MAX_LEN) {
        y++;
        x = 0;
      }
    }
    printw("\n\n");
    refresh();

    char wordBuffer[WORD_MAX_LEN];
    int wordcount = 0, char_index = 0, character_count = 0;
    time_t start_time = time(NULL);
    int correct_letter = 0, wrong_letter = 0, correct_words = 0;

    move(y + 2, 0);
    int c;
    while (wordcount < rand_word_count) {
      if (difftime(time(NULL), start_time) >= TIME_LIMIT) break;
      c = getch();
      if (c == 27) break;

      char *target = random_word_list[wordcount];
      int target_len = strlen(target);

      if (c == ' ' || c == '\n') {
        if (char_index > 0) {
          wordBuffer[char_index] = '\0';
          if (strcmp(wordBuffer, target) == 0) correct_words++;
          else wrong_letter += target_len;
          wordcount++;
          char_index = 0;
          printw(" ");
        }
      } else if (c == KEY_BACKSPACE || c == 127 || c == 8) {
        if (char_index > 0) {
          char_index--;
          int cur_x = getcurx(stdscr);
          int cur_y = getcury(stdscr);
          if (cur_x == 0 && cur_y > y + 2) {
            move(cur_y - 1, COLS - 1);
          } else {
            move(cur_y, cur_x - 1);
          }
          delch();
          refresh();
        }
      } else {
        character_count++;
        if (char_index < target_len) {
          if (c == target[char_index]) {
            attron(COLOR_PAIR(1));
            correct_letter++;
          } else {
            attron(COLOR_PAIR(2));
            wrong_letter++;
          }
        } else {
          attron(COLOR_PAIR(2));
        }
        addch(c);
        attroff(COLOR_PAIR(1) | COLOR_PAIR(2));
        wordBuffer[char_index++] = c;
      }
      refresh();
    }

    float accuracy = (correct_letter + wrong_letter)
                         ? ((float)correct_letter / (correct_letter + wrong_letter)) *
                               100
                         : 0;
    double timeUsed = difftime(time(NULL), start_time);
    float wpm = (timeUsed > 0) ? ((float)correct_words / (timeUsed / 60.0)) : 0;
    float cps = timeUsed ? ((float)character_count / timeUsed) : 0;

    //calls the endGame function to handle end-of-game actions 
    endGame(all_word_list, count, accuracy, timeUsed, wordcount, wpm, cps,
            correct_words, correct_letter, wrong_letter, character_count);

    
    printw("Press 'r' to restart, or Escape to quit.\n");
    refresh();

    while (1) { 
      ch = getch();
      if (ch == 'r') {
        clear();
        break; // Restarts the game loop
      } else if (ch == 27) { 
        clear();
        endwin(); 
        refresh(); 
        system("reset");  
        exit(0);    
      }
    }
  }

  endwin();   
  return 0;
}

void endGame(char all_word_list[][WORD_MAX_LEN], int count, float accuracy,
             double timeUsed, int wordcount, float wpm, float cps,
             int correct_words, int correct_letter, int wrong_letter,
             int character_count) {
  clear();
  printw("Time's up!\nAccuracy: %.2f%%\nTime: %.1fs \nWords: %d \nWPM: %.1f\n",
         accuracy, timeUsed, wordcount, wpm);
  printw(
      "Correct Words: %d \nCorrect Letters: %d \nMissed and Incorrect Letters: %d \nTotal Letters: %d\n",
      correct_words, correct_letter, wrong_letter, character_count);
  printw("Characters per second (CPS): %.1f\n", cps);
  refresh();
}