#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <utility>

namespace go {

class Board {
	static const int DEFAULT_SIZE = 19;

public:
	static const int BLACK = 1;
	static const int WHITE = -1;
	static const int EMPTY = 0;
	Board();
	Board(int size);
	~Board();

	int GetSize() const;
	int GetState(int x,int y) const;
	int GetPrevX() const;
    int GetPrevY() const;
	bool SetStateBlack(int x, int y);
	bool SetStateWhite(int x, int y);
	bool IsLastMovePass() const;
	void SetLastMovePass(bool pass);

	void Draw() const;

private:
    void Init();
	//Here, placing SetState in private is to prevent malicious attacks (since the game rules dictate that black and white take turns, the state can be controlled by the system).
	bool SetState(int x, int y, int state);
	bool IsValidMove(int x, int y, int color);
    bool RemoveDeadStones(int color);
    bool HasLiberty(int x, int y, bool visited[], int ignore_color = -1);
    void FindGroup(int x, int y, int color, bool visited[], 
                   std::vector<std::pair<int, int> >& group);
    bool CheckSuicide(int x, int y, int color);

	int size_;
	int *state_;
	int prevx, prevy;//The last move's position is used to determine if a ko situation exists.
	bool last_move_pass_; // Records whether the last move was a pass.
};

}

#endif
