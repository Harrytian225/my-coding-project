#include <iostream>

#include "board.h"
#include "game.h"
#include "player.h"

int main() {
	go::Board board;
	go::Player player1(true, "cat"), player2(false, "dog");
	go::Game game(&player1, &player2, &board);
	game.Start();
}