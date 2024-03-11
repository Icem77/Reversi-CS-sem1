# Reversi-CS-sem1
The program enables two players to play Reversi.

Program does the following in a loop:
 🎲 It displays a prompt indicating which player's turn it is and what legal moves they can make.
 🎲 It reads the player's command for whom the turn belongs to.
 🎲 If it reads a command to end the game, it terminates.
 🎲 If it reads a command to resign from the move, it returns to the beginning of the loop and passes the turn to the opponent.
 🎲 If it reads a command to make a move, it executes that move and returns to the beginning of the loop.

Exceptions to the original rules:
🎲 A player can resign from making a move even if they can make a legal move.
🎲 The game does not end automatically when neither player has a legal move available.

🎲 INPUT:
Players' commands occupying one line each. All commands have a correct form and end with the representation of the end of the line '\n'. Move execution commands indicate a legal move. After the game interruption command, there can be any text, ignored by the program.
The game interruption command is represented by a line with the content '='.
The resignation from move command is represented by a line with the content '-'.
The move execution command is represented by a line with the name of the field.

🎲 OUTPUT:
The program output is a sequence of prompt texts. Each of them occupies one line and ends with the representation of the end of the line '\n'.
The prompt text starts with an indication of the player to whom the move belongs, the 'C' character indicates the Black player, and the 'B' character indicates the White player.
The rest of the prompt text consists of a sequence of field names on which the player can place their pawn.
The names are sorted lexicographically. Each name is preceded by a space.
The field name is as in the input case, a digit from '1' to '8' preceded by a lowercase letter from 'a' to 'h'.

❗️In the code, both variable names and comments are in Polish because this program was one of the projects for the first semester of computer science studies. Sorry for the inconvenience.❗️
