#ifndef _PIECE_H
#define _PIECE_H

#include "image.h"

enum RANK {
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    KING,
    QUEEN
};

class Piece : public Image {
public:
    RANK rank;
    int gridX, gridY;

public:
    RANK getRank() { return rank; }
    int getGridX() { return gridX; }
    int getGridY() { return gridY; }

    void setRank(RANK r) { rank = r; }
    void setGridX(int x) { gridX = x; spriteData.x = ((float)x + 1) * SQUARE_WIDTH + (SQUARE_WIDTH - PIECES_WIDTH) / 2; }
    void setGridY(int y) { gridY = y; spriteData.y = ((float)y + 1) * SQUARE_WIDTH; }
};

#endif
