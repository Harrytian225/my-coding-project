#include "player.h"

#include <cstring>
#include <iostream>

namespace go {

Player::Player(bool is_black, const char *name) :
	is_black_(is_black), state_(IN_GAME), passed_(false) {
	strcpy(name_, name);
}


bool Player::IsBlack() const { return is_black_; }
void Player::SetRole(bool is_black) { is_black_ = is_black; }
const char *Player::GetName() { return name_; }

bool Player::IsWin() const { return (state_ == WIN); }
bool Player::IsLost() const { return (state_ == LOST); }
bool Player::IsInGame() const { return (state_ == IN_GAME); }
void Player::Win() { state_ = WIN; }
void Player::Lost() { state_ = LOST; }

bool Player::HasPassed() const { return passed_; }
void Player::ResetPass() { passed_ = false; }

void Player::Play(Board *board) {
	passed_ = false;

    std::cout << name_;
    if (is_black_)
	std::cout << " (Black)'s turn, ";
    else
	std::cout << " (White)'s turn, ";
    bool play_success = false;
    while (!play_success) {
		std::cout << "enter coordinates (e.g., 1 2), -1 -1 to pass, 0 0 to forfeit:" << std::endl;
		while (!play_success) {
			int x = 0, y = 0;
			std::cin >> x >> y;

			if (!x && !y) {
			    state_ = LOST; // forfeit
			    play_success = true;
			} else if (x == -1 && y == -1) {
      	      	// Pass
      	    	passed_ = true;
            	std::cout << name_ << " chooses to pass." << std::endl;
            	play_success = true;
        	} else {
		    	if (is_black_)
				play_success = board->SetStateBlack(x - 1, y - 1);
		    	else
				play_success = board->SetStateWhite(x - 1, y - 1);

				if (!play_success) {
             	   std::cout << "Invalid move! Please try again." << std::endl;
            	}
			}
    	}
	}
}


}