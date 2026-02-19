# Typing-Speed-Console-Game
A typing speed console game in C made using ncurses. It is an interactive game which gives real time feedback regarding the correct and incorrect keystrokes. Test your typing speed, accuracy and reflexes across multiple difficulty levels. 

## Features 
- Three difficullty modes:
  - Easy: Short Words (1-4 letters)
  - Medium: Medium Words (5-7 letters)
  - Hard: Hard Words (8+ letters)
- Real-Time Feedback:
  - Correct letters are highlighted in green
  - Incorrect letters are highlighted in red
- Typing Metrics Tracked:
  - Accuracy (%)
  - Words Per Minute (WPM)
  - Characters Per Second (CPS)
  - Correct words and letters counted
  - Missed and incorrect letters counted
  - Total letters, words and time taken counted
- Game Controls:
  - Start: Press Enter
  - Restart: Press "r"
  - Quit anytime after the game starts: Press ESC
  - Time limit of 60 seconds
- Randomized word selection from a word list of a txt file for replayability. It selects the appropriate words for the difficulty modes from the file.

## How to Run
- Open a terminal in the folder where typing_game.c is located
- Compile it with gcc and link ncurses:
  ``` bash
  # gcc typing_game.c -o typinggame -lncurses
- Run it using a word list (for example wordpool.txt in the same folder):
  ``` bash
  # ./typinggame < wordpool.txt

## Gameplay
- Select a difficulty mode (Easy/ Medium/ Hard)
- Words will appear on the screen
- Type the words as fast and accurately as possible
- Backspace is allowed but pressing space automatically starts the next word
- Metrics update in real-time, showing correct/incorrect letters
- After 60 seconds or when all the words are typed, stats are displayed

## Technical Highlights
