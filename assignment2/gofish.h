#ifndef GOFISH_H
#define GOFISH_H

#include <stdlib.h>
#include "player.h"
#include "deck.h"

/*
   Define any prototype functions
   for gofish.h here.
*/


/*
 * Contributor: Jeffrey Song
 * Function: game_start
 * --------------------
 * Starts a new game
 * resets players, shuffles deck, and deals cards
 * 
 * 
 */
void game_start();


/* Contributor: Byung Woong Ko
 * Function: game_loop
 * -------------------
 * Method to handle each round of game play.
 * Implmented right after game starts.
 * 
 * Return: 1 if there is a winner, 0 otherwise
 */
int game_loop();

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
int game_end();


/* Contributor: Byung Woong Ko
 * Function: print_status
 * ------------
 * A simple function to print
 * current status of user's hand
 * and book. Also prints current
 * status of computer's book.
 */
void print_status();
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
const char* pR(char r);

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
void print_hand(struct player* target);

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
void print_book(struct player* target);

/*
 * Instance Variables: current, next_player
 * 
 * Points to the current player and the next player's turn
 */
struct player* current;
struct player* next_player;

#endif