// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Daniel Harding
// chess.h

#ifndef _CHESS_H           // Prevent multiple definitions if this 
#define _CHESS_H           // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <sstream>
#include <regex>
#include "game.h"
#include "scene.h"
#include "textureManager.h"
#include "image.h"
#include "textDX.h"
#include "piece.h"
#include "team.h"

//=============================================================================
// This class is the core of the game
//=============================================================================
class Chess : public Game
{
private:
    // game items
    TextDX  *dxFont;            // DirectX font

    TextureManager titlescreenTexture;
    Image titlescreen;

    TextureManager logoTexture;
    Image logo;

    TextureManager board1Texture;
    TextureManager board2Texture;
    Image   board;

    TextureManager piece1Textures;
    TextureManager piece2Textures;
    TextureManager piece3Textures;
    Team* white = NULL;
    Team* black = NULL;

    TextureManager cursorTexture;
    Image cursor;

    TextureManager highlightTexture;
    Image highlight;

    bool onTitlescreen;
    bool whitesTurn;
    bool gameOver;

    int curBoard;
    int curSet;

public:
    // Constructor
    Chess();
    // Destructor
    virtual ~Chess();
    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void consoleCommand(); // process console command
    void releaseAll();
    void resetAll();

    void StartRound();
    void InitPieces();
    void InitBoard();
    void PrintPieces();
    void PrintCursorAt(int x, int y, COLOR_ARGB color);
    void PrintHighlightAt(int x, int y, COLOR_ARGB color);
    void HighlightMoves(Team* player, Team* opp);
    bool IsPieceAt(int x, int y);
    bool IsPieceAt(int x, int y, Piece pieces[]);
    bool IsPieceBetween(int x1, int y1, int x2, int y2);
    int PieceAt(int x, int y, Piece pieces[]);
};

#endif
