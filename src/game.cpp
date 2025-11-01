#include "game.h"

#include <iostream>

namespace go {

Game::Game(Player *bp, Player *wp, Board *board):
	bp_(bp), wp_(wp), board_(board) {}

const Player *Game::GetBlackPlayer() const { return bp_; }
const Player *Game::GetWhitePlayer() const { return wp_; }
const Board *Game::GetBoard() const { return board_; }

void Game::SetBlackPlayer(Player *p) {
    if (!bp_->IsInGame())
	bp_ = p;
}
void Game::SetWhitePlayer(Player *p) {
    if (!wp_->IsInGame())
	wp_ = p;
}
void Game::SetBoard(Board *b) {
    if (!bp_->IsInGame() && !wp_->IsInGame())
	board_ = b;
}

void Game::Start() const {
    std::cout << "Game Start!" << std::endl;
    bool is_black_turn = true;
    int consecutive_passes = 0;

    while (bp_->IsInGame()) {
		board_->Draw();

		bp_->ResetPass();
		wp_->ResetPass();

		if (is_black_turn) {
		    bp_->Play(board_);
	 	   	if (bp_->IsLost()) { // check forfeit
				wp_->Win();
				break;
	 	   	}
		} else {
	   		wp_->Play(board_);
	  	 	if (wp_->IsLost()) {
				bp_->Win();
				break;
	  	  	}
		}

		if ((is_black_turn && bp_->HasPassed()) || 
            (!is_black_turn && wp_->HasPassed())) {
            consecutive_passes++;
            std::cout << "Pass count: " << consecutive_passes << std::endl;
        } else {
            consecutive_passes = 0; // Reset the consecutive pass count.
        }

		if (consecutive_passes >= 2) {
            std::cout << "Both players passed.";
            break;
        }

  		is_black_turn = !is_black_turn;

    }
    if (consecutive_passes < 2)board_->Draw();
    if (bp_->IsWin()){
	std::cout << bp_->GetName() << " (Black) Win!" << std::endl;
    } else if (wp_->IsWin()){
	std::cout << wp_->GetName() << " (White) Win!" << std::endl;
	} else {
		std::cout << " Game ended by mutual agreement." << std::endl;

	}
}


} // namespace go
