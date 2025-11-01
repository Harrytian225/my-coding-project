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
	//此处把SetState写在private里是避免恶意攻击（因为游戏设定是黑一步白一步，所以state可以由系统控制）
	bool SetState(int x, int y, int state);
	bool IsValidMove(int x, int y, int color);
    bool RemoveDeadStones(int color);
    bool HasLiberty(int x, int y, bool visited[], int ignore_color = -1);
    void FindGroup(int x, int y, int color, bool visited[], 
                   std::vector<std::pair<int, int> >& group);
    bool CheckSuicide(int x, int y, int color);

	int size_;
	int *state_;
	int prevx, prevy;//上一次落子位置，用于判断是否存在打劫
	bool last_move_pass_; // 记录上一次是否pass
};

}

#endif