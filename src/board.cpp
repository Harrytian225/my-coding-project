#include "board.h"

#include <iostream>
#include <iomanip>

namespace go {

Board::Board(): size_(DEFAULT_SIZE),prevx(-1), prevy(-1), last_move_pass_(false) { Init(); }
Board::Board(int size): size_(size),prevx(-1), prevy(-1), last_move_pass_(false) { Init(); }

void Board::Init() {
	state_ = new int[size_ * size_];
	for(int i = 0; i < size_ * size_; i++){
		state_[i] = EMPTY;
	}
}

Board::~Board() {
	delete[] state_;
}

int Board::GetSize() const { return size_; }

int Board::GetState(int x, int y) const {
	if (x < 0 || x >= size_ || y < 0 || y >= size_) {
        return EMPTY;
    }
	return state_[x * size_ + y];
}

int Board::GetPrevX() const { 
    return prevx; 
}

int Board::GetPrevY() const { 
    return prevy; 
}

bool Board::SetStateBlack(int x, int y) {
	return SetState(x, y, BLACK);
}

bool Board::SetStateWhite(int x, int y) {
	return SetState(x, y, WHITE);
}

bool Board::IsLastMovePass() const { 
    return last_move_pass_; 
}

void Board::SetLastMovePass(bool pass) { 
    last_move_pass_ = pass; 
}

bool Board::SetState(int x, int y, int state) {
    if(x < 0 || x >= size_ || y < 0 || y >= size_) return false;
    if (state_[x * size_ + y] != EMPTY) return false;
    
    // 检查打劫：不能立即在上一手被提子的位置落子
    if (x == prevx && y == prevy) return false;
    
    if (!IsValidMove(x, y, state)) return false;

    int tempState = state_[x * size_ + y];
    state_[x * size_ + y] = state;

    // 保存提子前的状态，用于检测提子数量
    int *before_capture_state = new int[size_ * size_];
    for (int i = 0; i < size_ * size_; i++) {
        before_capture_state[i] = state_[i];
    }

    // 提掉对方的死子
    bool captured = RemoveDeadStones(-state);

    if (!HasLiberty(x, y, new bool[size_ * size_])) {
        state_[x * size_ + y] = tempState; // 回退
        delete[] before_capture_state;
        return false;
    }

    // 检查是否形成了打劫（只提了一个子）
    if (captured) {
        int capture_count = 0;
        int captured_x = -1, captured_y = -1;
        
        // 计算提子数量并记录被提子的位置
        for (int i = 0; i < size_ * size_; i++) {
            if (before_capture_state[i] != EMPTY && state_[i] == EMPTY) {
                capture_count++;
                if (capture_count == 1) {  // 只记录第一个被提子的位置
                    captured_x = i / size_;
                    captured_y = i % size_;
                }
            }
        }
        
        // 如果只提了一个子，设置打劫位置
        if (capture_count == 1) {
            prevx = captured_x;
            prevy = captured_y;
        } else {
            // 提了多个子，不是打劫，重置打劫位置
            prevx = -1;
            prevy = -1;
        }
    } else {
        // 没有提子，重置打劫位置
        prevx = -1;
        prevy = -1;
    }
    
    delete[] before_capture_state;
    return true;
}

bool Board::IsValidMove(int x, int y, int color) {
    // 移除了原来的打劫检查，因为已经在 SetState 中处理了
    
    // 临时落子检查自杀规则
    int original_state = state_[x * size_ + y];
    state_[x * size_ + y] = color;
    
    bool *visited = new bool[size_ * size_]();
    bool has_liberty = HasLiberty(x, y, visited);
    
    // 如果自己没有气，检查是否能提对方的子
    if (!has_liberty) {
        int dx[] = {0, 1, 0, -1};
        int dy[] = {1, 0, -1, 0};
        bool can_capture = false;
        int opponent = -color;
        
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            if (nx >= 0 && nx < size_ && ny >= 0 && ny < size_ && 
                state_[nx * size_ + ny] == opponent) {
                
                bool *opp_visited = new bool[size_ * size_]();
                if (!HasLiberty(nx, ny, opp_visited)) {
                    can_capture = true;
                    delete[] opp_visited;
                    break;
                }
                delete[] opp_visited;
            }
        }
        
        // 回退临时落子
        state_[x * size_ + y] = original_state;
        delete[] visited;
        
        return can_capture;
    }
    
    // 回退临时落子
    state_[x * size_ + y] = original_state;
    delete[] visited;
    return true;
}

bool Board::HasLiberty(int x, int y, bool visited[], int ignore_color) {
    if (x < 0 || x >= size_ || y < 0 || y >= size_) {
        return false;
    }
    if (visited[x * size_ + y]) {
        return false;
    }
    
    int color = state_[x * size_ + y];
    if (color == EMPTY) {
        return true; // 空位置就是气
    }
    if (ignore_color != -1 && color == ignore_color) {
        return false;
    }
    
    visited[x * size_ + y] = true;
    
    // 检查四个方向
    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};
    
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        if (nx >= 0 && nx < size_ && ny >= 0 && ny < size_) {
            if (state_[nx * size_ + ny] == EMPTY) {
                return true; // 找到气
            }
            if (state_[nx * size_ + ny] == color) {
                if (HasLiberty(nx, ny, visited, ignore_color)) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool Board::RemoveDeadStones(int color) {
    bool removed = false;
    bool *visited = new bool[size_ * size_]();
    
    for (int i = 0; i < size_; i++) {
        for (int j = 0; j < size_; j++) {
            if (!visited[i * size_ + j] && state_[i * size_ + j] == color) {
                std::vector<std::pair<int, int> > group;
                FindGroup(i, j, color, visited, group);
                
                // check whether this group has liberty
                bool group_has_liberty = false;
                for (auto& stone : group) {
                    bool *stone_visited = new bool[size_ * size_]();
                    if (HasLiberty(stone.first, stone.second, stone_visited)) {
                        group_has_liberty = true;
                        delete[] stone_visited;
                        break;
                    }
                    delete[] stone_visited;
                }
                
                // if this group doesn't have liberty, then remove all of them
                if (!group_has_liberty) {
                    for (auto& stone : group) {
                        state_[stone.first * size_ + stone.second] = EMPTY;
                    }
                    removed = true;
                }
            }
        }
    }
    
    delete[] visited;
    return removed;
}

void Board::FindGroup(int x, int y, int color, bool visited[], 
                      std::vector<std::pair<int, int> >& group) {
    if (x < 0 || x >= size_ || y < 0 || y >= size_) {
        return;
    }
    if (visited[x * size_ + y]) {
        return;
    }
    if (state_[x * size_ + y] != color) {
        return;
    }
    
    visited[x * size_ + y] = true;
    group.push_back(std::make_pair(x, y));
    
    // 递归检查四个方向的相邻棋子
    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};
    
    for (int i = 0; i < 4; i++) {
        FindGroup(x + dx[i], y + dy[i], color, visited, group);
    }
}

void Board::Draw() const {
    std::cout << "   ";
    for (int j = 0; j < size_; j++) {
	std::cout << std::setw(3) << (j + 1);
    }
    std::cout << std::endl << std::endl;
    for (int i = 0; i < size_; i++) {
	std::cout << std::setw(3) << (i + 1);
	for (int j = 0; j < size_; j++) {
	    int state = state_[i * size_ + j];
	    if (state == EMPTY)
		std::cout << "\x1B[34m" << std::setw(3) << "+" << "\033[0m";// "\x1B[34m": set color to blue
	    else if (state == BLACK)
		std::cout << "  " << "\u25CF" << "\033[0m";
	    else
		std::cout << "  " << "\u25CB" << "\033[0m";;
	}
	std::cout << std::endl << std::endl;
    }
}

} // namespace go



