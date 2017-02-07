#ifndef _TEAM_H
#define _TEAM_H

#include <string>
#include "piece.h"

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
    float messageX, messageY;
    bool inCheck, inCheckMate, inStaleMate;
    bool holding;
    int held;
    int pawnDirection;
    bool god;

public:
    Piece* getPieces() { return pieces; }
    int getCursorX() { return cursorX; }
    int getCursorY() { return cursorY; }
    std::string getMessage() { return message; }
    float getMessageX() { return messageX; }
    float getMessageY() { return messageY; }
    bool getInCheck() { return inCheck; }
    bool getInCheckMate() { return inCheckMate; }
    bool getInStaleMate() { return inStaleMate; }
    int getPawnDirection() { return pawnDirection; }
    bool getHolding() { return holding; }
    int getHeld() { return held; }
    bool getGod() { return god; }

    void setOpp(Team* o) { opp = o; }
    void setCursorX(int x) { cursorX = x; }
    void setCursorY(int y) { cursorY = y; }
    void setMessage(std::string m) { message = m; }
    void setMessageX(float x) { messageX = x; }
    void setMessageY(float y) { messageY = y; }
    void setInCheck(bool c) { inCheck = c; }
    void setInCheckMate(bool c) { inCheckMate = c; }
    void setPawnDirection(int p) { pawnDirection = p; }
    void setGod(bool g) { god = g; }

    Team(Chess* g);

    bool update();
    bool TakeTurn();
    void CheckForCheck();
    bool IsInCheck();
    bool IsInCheckMate();
    bool IsInStaleMate();
    bool CanMove(int piece);
    bool CanMoveTo(int piece, int x, int y);
    bool WillCauseCheck(int piece, int x, int y);
};

#endif
