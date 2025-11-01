#ifndef __GAME_H__
#define __GAME_H__

#include "board.h"
#include "player.h"

namespace go {

class Game {
    
public:
    Game(Player *bp, Player *wp, Board *board);

    const Player *GetBlackPlayer() const;
    const Player *GetWhitePlayer() const;
    const Board *GetBoard() const;

    void SetBlackPlayer(Player *p);
    void SetWhitePlayer(Player *p);
    void SetBoard(Board *b);
    
    void Start() const;
    
private:
    void Judge() const;
    
    //bp:blackplayer
    Player *bp_;
    Player *wp_;
    Board *board_;
};

} // namespace go

#endif