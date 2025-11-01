#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "board.h"

namespace go {

class Player{
	static const int WIN = 1;
    static const int LOST = -1;
    static const int IN_GAME = 0;

public:
	Player(bool is_black, const char *name);

	bool IsBlack() const;
	void SetRole(bool is_black);
	const char *GetName();

	bool IsWin() const;
	bool IsLost() const;
	bool IsInGame() const;
	void Win();
	void Lost();

	bool HasPassed() const; 
    void ResetPass();

	void Play(Board *board);

private:
	bool is_black_;
	int state_;
	char name_[20];
	bool passed_;
};

}

#endif
