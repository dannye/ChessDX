// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// createThisClass.h

#ifndef _CREATETHIS_H           // Prevent multiple definitions if this 
#define _CREATETHIS_H           // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <sstream>
#include <regex>
#include "game.h"
#include "scene.h"
#include "textureManager.h"
#include "image.h"
#include "textDX.h"
#include "team.h"
#include "piece.h"

//=============================================================================
// This class is the core of the game
//=============================================================================
class Chess : public Game
{
private:
    Scene* scene;

    // game items
    TextureManager backgroundTexture; // textures
    Image   background;         // background image

    TextDX  *dxFont;            // DirectX font

    TextureManager pieceTextures;
    Team* white;
    Team* black;

    TextureManager cursorTexture;
    Image cursor;

    TextureManager highlightTexture;
    Image highlight;

    bool whitesTurn;
    bool gameOver;

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
