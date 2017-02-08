#ifndef _TEAM_H
#define _TEAM_H

#include <string>
#include "piece.h"

// indicies for a Team's pieces array
enum PIECE_INDICIES {
    PAWN1,
    PAWN2,
    PAWN3,
    PAWN4,
    PAWN5,
    PAWN6,
    PAWN7,
    PAWN8,
    ROOK1,
    ROOK2,
    KNIGHT1,
    KNIGHT2,
    BISHOP1,
    BISHOP2,
    QUEEN1,
    KING1
};

class Chess;
class Audio;
class Input;

class Team {
private:
    Chess* game;
    Audio* audio;
    Input* input;

    Team* opp;

    Piece pieces[NUM_PIECES];
    int cursorX, cursorY;
    std::string message;
    float messageBaseY;
    float messageX, messageY;
    bool inCheck, inCheckMate, inStaleMate;
    bool holding;
    PIECE_INDICIES held;
    int pawnDirection;
    bool god;

public:
    Piece* getPieces() { return pieces; }
    int getCursorX() { return cursorX; }
    int getCursorY() { return cursorY; }
    std::string getMessage() { return message; }
    float getMessageBaseY() { return messageBaseY; }
    float getMessageX() { return messageX; }
    float getMessageY() { return messageY; }
    bool getInCheckMate() { return inCheckMate; }
    bool getInStaleMate() { return inStaleMate; }
    int getPawnDirection() { return pawnDirection; }
    bool getHolding() { return holding; }
    PIECE_INDICIES getHeld() { return held; }
    bool getGod() { return god; }

    void setOpp(Team* o) { opp = o; }
    void setCursorX(int x) { cursorX = x; }
    void setCursorY(int y) { cursorY = y; }
    void setMessageBaseY(float y) { messageBaseY = y; }
    void setMessageX(float x) { messageX = x; }
    void setMessageY(float y) { messageY = y; }
    void setPawnDirection(int p) { pawnDirection = p; }
    void setGod(bool g) { god = g; }

    Team(Chess* g);

    bool update();
    bool TakeTurn();
    void updateMessage(float ft);
    void CheckForCheck();
    bool IsInCheck();
    bool IsInCheckMate();
    bool IsInStaleMate();
    bool CanMove(int piece);
    bool CanMoveTo(int piece, int x, int y);
    bool WillCauseCheck(int piece, int x, int y);
};

#endif
