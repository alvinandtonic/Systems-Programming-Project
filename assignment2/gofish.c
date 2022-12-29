#include <stdio.h>
#include <time.h>
#include "gofish.h"

int main(int args, char* argv[]) {
    do {
        game_start();
        do {
            // Play the game. If there is a winner, go to game_end
            if(game_loop())
                break; 
            current = next_player;
        } while(1);
    } while(game_end());
    fprintf(stdout, "Exiting\n");
    return 0;
}

/*
 * Contributor: Jeffrey Song
 * Function: game_start
 * --------------------
 * Starts a new game
 * resets players, shuffles deck, and deals cards
 * 
 * 
 */
void game_start() {
    reset_player(&user);
    reset_player(&computer);

    fprintf(stdout, "Shuffling deck...\n");
    shuffle();
    deal_player_cards(&user);
    deal_player_cards(&computer);

    // Checks if first hand has a book
    check_add_book(&user);
    check_add_book(&computer);
    if(user.book[0] != 0)
        fprintf(stdout, "Player 1 books %s", pR(user.book[0]));
    if(computer.book[0] != 0)
        fprintf(stdout, "Player 2 books %s", pR(computer.book[0]));

    current = &user;
    next_player = &computer;
}

/*
 * Contributor: Byung Woong Ko
 * Function: game_loop
 * -------------------
 * Method to handle each round of game play.
 * Implmented right after game starts.
 * 
 * Return: 1 if there is a winner, 0 otherwise
 */

int game_loop() {
    fprintf(stdout, "\n");
    print_status();
    struct player* other_player;
    // Set who the other player is.
   if (current == &computer) {
      other_player = &user;
   }
   else{
      other_player = &computer;
   }
    // Vheck if current user has 7 rank in book, where game is over
    if(game_over(current)) {
        return 1;
    }

    char r;
    // If not empty hand, play through a turn. if empty handed, set r to '0' to be used go by handle case (gofish).
    if(current->hand_size > 0) { 
        if(current == &computer) { 
            r = computer_play(current);
        } else { 
            r = user_play(current);
        }
    } 
    else 
        r = '0'; 

    // If the other player does not have the cards player calls out, its gofish, and current player draws a card.
    if(search(other_player, r) == 0) { 
        // As long as current user's hand is not empty. Print below.
        if(r != '0'){ 
            if(current == &computer){
                fprintf(stdout, "   - Player %d has no %s's\n", 1, pR(r));    
            }
            else{
                fprintf(stdout, "   - Player %d has no %s's\n", 2, pR(r));
            }
        }

        struct card* fishedC = next_card();
        int next_book_i = 0;

        // As long as there are cards on deck, current player will draw a card, and it's content is printed if it was the user, hidden if it was the computer.
        if (fishedC != NULL) {
            if(current == &user)
                fprintf(stdout, "   - Go Fish, Player 1 draws %s%c\n", pR(fishedC->rank), fishedC->suit);
            else {
                fprintf(stdout, "   - Go Fish, Player 2 draws a card\n");
            }

           while(current->book[next_book_i] != 0) {
                next_book_i++;
            }
            add_card(current, fishedC);

            // If after adding a card, a rank match was found, print updated book.
            if(current->book[next_book_i] != 0){
                if(current == &user){
                    fprintf(stdout, "   - Player %d books %s\n", 1, pR(fishedC->rank));    
                }
                else{
                    fprintf(stdout, "   - Player %d books %s\n", 1, pR(fishedC->rank));
                }
            }
        }

        // Play again if book was added or card transfer occured. If not, swtich turns.
        if(fishedC != NULL && (current->book[next_book_i] != 0 || fishedC->rank == r)) {
            next_player = current;
            if(current == &user){
                fprintf(stdout, "   - Player %d gets another turn\n", 1);
            }
            else{
                fprintf(stdout, "   - Player %d gets another turn\n", 2);
            }
        } 
        else { 
            next_player = other_player;
            if(next_player == &user){
                fprintf(stdout, "   - Player %d's turn\n", 1);
            }
            else{
                fprintf(stdout, "   - Player %d's turn\n", 2);
            }
        }
    } 

    else { 
        // Card match(s) were found. Print rank matched cards other player had.
        struct player* print_player = current;
        for(int i = 0; i < 2; i++) {
            //Switch to other player
            if (current == &computer && print_player == &computer) {
                if(current == &computer){
                    print_player = &user;
                }
                else{
                    print_player = &computer;
                } 
                continue;
            }

            fprintf(stdout, "   - Player %d has ", ((print_player == &user) ? 1 : 2));
            struct hand* curH = print_player->card_list;
            int rank_count = 0;
            for(int j = 0; j < print_player->hand_size && curH != NULL; j++) {
                if(curH->top.rank == r) {
                    if(rank_count++ > 0)
                        fprintf(stdout, ", ");
                    fprintf(stdout, "%s%c", pR(r), curH->top.suit);
                }

                curH = curH->next;
            }
            fprintf(stdout, "\n");
            // Switch to other player
            if(current == &computer){
                print_player = &user;
            }
            else{
                print_player = &computer;
            }
        }

        int next_book_i = 0;
        while(current->book[next_book_i] != 0) {
            next_book_i++;
        }
        transfer_cards(other_player, current, r);
        if(current->book[next_book_i] != 0){
            if(current == &user){
                fprintf(stdout, "   - Player %d books %s\n", 1, pR(r));
            }
            else{
                fprintf(stdout, "   - Player %d books %s\n", 2, pR(r));
            }
        }
        // Play again if book was added. If not, switch turns.
        if(current->book[next_book_i] != 0) {
            next_player = current;
            if(current == &user){
                fprintf(stdout, "   - Player %d gets another turn\n", 1);
            }
            else{
                fprintf(stdout, "   - Player %d gets another turn\n", 2);
            }
        }
        else {
            next_player = other_player;
            if(current == &user){
                fprintf(stdout, "   - Player %d's turn\n", 1);
            }
            else{
                fprintf(stdout, "   - Player %d's turn\n", 2);
            }
        }
    }
    return 0;
}

/*
 * Contributor: Byung Woong Ko
 * Function: game_end
 * ------------------
 * Declares the winner and prompts the user 
 * to play again.
 * Implemented right after someone wins the game. 
 * (when gameloop is finished)
 * 
 * Return: 1 to play again, 0 to exit
 */

int game_end() {
    struct player* other_player;
   if (current == &computer) {
      other_player = &user;
   }
   else{
      other_player = &computer;
   }
    
    // Only count book of loser since winner has 7 by default.
    int count = 0;

    while(other_player->book[count] != 0) { 
        count++;
    }

    // Check which user won and print respective comments.
    if(current == &user) { 
        fprintf(stdout, "Player 1 Wins! 7-%d\n", count);
    } 
    else {
        fprintf(stdout, "Player 2 Wins! 7-%d\n", count);
    }


    // Replay portion of function. Only takes in "Y" or "N" to signify if user wants to replay. Error if any other character or characters are passed.
    int replay = 0;
    int or = 0;
    char YorN[3] = "";
    
    while(replay == 0) {
        if(or) {
            fprintf(stdout, "Error - must enter \"Y\" or \"N\"");
        }
        fprintf(stdout, "\nDo you want to play again [Y/N]: ");
        scanf("%2s", YorN);

        while(getchar() != '\n');

        if(YorN[1] != '\0'){
            continue;
        }
        if(YorN[0] == 'N'){
            replay = 1;
            return 0;
        }
        else if(YorN[0] == 'Y'){
            replay = 1;
            return 1;
        }
        else{
            continue;
        }
    }
    return 0;
}

/* Contributor: Byung Woong Ko
 * Function: print_status
 * ------------
 * A simple function to print
 * current status of user's hand
 * and book. Also prints current
 * status of computer's book.
 */
void print_status(){
    fprintf(stdout, "Player 1's Hand - ");
    print_hand(&user);
    fprintf(stdout, "Player 1's Book - ");
    print_book(&user);
    fprintf(stdout, "Player 2's Book - ");
    print_book(&computer);
}

/*
 * Contributor: Jeffrey Song
 * Function: pR
 * ------------
 * stands for Print_Rank
 * if the rank is 'T' return "10"
 * other input is same but in
 * string format
 * 
 * r: the rank to format
 * 
 * Return: String representing rank r
 */
const char* pR(char r) {
    if(r == 'T') { // if T change to 10
        static char ten[] = "10";
        return ten;
    }
    static char rS[2];
    rS[0] = r;
    rS[1] = '\0';
    return rS;
}

/*
 * Contributor: Jeffrey Song
 * Function: print_hand
 * --------------------
 * Prints the hand of the
 * target player in the format
 * of rank and suit seperated by
 * a space
 * 
 * 
 * 
 * target: the player to print the hand of
 */
void print_hand(struct player* target) {
    if(target->hand_size == 0) { // empty hand print new line
        fprintf(stdout, "\n");
        return;
    }

    struct hand* h = target->card_list; // h is current hand

    while(h != NULL) { // while h is not null iterate and print each card
      fprintf(stdout, " %s%c", pR(h->top.rank), h->top.suit);
      h = h->next;
    }

    fprintf(stdout, "\n");
}

/*
 * Contributor: Jeffrey Song
 * Function: print_book
 * --------------------
 * Prints the book of the target player 
 * in the format of the rank
 * representaion
 * 
 * 
 * target: the player to print the book of
 */
void print_book(struct player* target) {
    if(target == NULL || target->book == NULL || target->book[0] == '\0' || target->book[0] == 0) { // newline if no book
        fprintf(stdout, "\n");
        return;
    }

    int i = 0;
    while(i < 7 && target->book[i] != '\0' && target->book[i] != 0) {
        fprintf(stdout, " %s", pR(target->book[i++]));
    }

    fprintf(stdout, "\n");
}

