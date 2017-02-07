// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Game Engine constants.h v4.0
// Last modification: Mar-1-2015

#pragma once
#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//=============================================================================
//                  Constants
//=============================================================================

// window
const char CLASS_NAME[] = "Chess";
const char GAME_TITLE[] = "ChessDX";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  800;               // width of game in pixels
const UINT GAME_HEIGHT = 800;               // height of game in pixels
 
// game
const bool VSYNC = false;                   // true locks display to vertical sync rate
const float FRAME_RATE = 240.0f;            // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const double PI = 3.14159265;
const int NUM_PIECES = 16;
const int BOARD_WIDTH = 8;
const int BOARD_HEIGHT = 8;
const int MESSAGE_SPEED = 120; // pixels per second
const int MESSAGE_CHECK_X = 350;
const int MESSAGE_CHECKMATE_X = 315;
const int MESSAGE1_Y = -30;
const int MESSAGE2_Y = GAME_HEIGHT;

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

// graphic images
const char TITLE_IMAGE[] = "pictures\\titlescreen.png";
const char LOGO_IMAGE[] = "pictures\\logo.png";
const char BOARD_IMAGE[] = "pictures\\board.png"; // background
const char CHESS_PIECES[] = "pictures\\pieces.png"; // chess pieces
const char CURSOR_IMAGE[] = "pictures\\cursor.png";
const char HIGHLIGHT_IMAGE[] = "pictures\\highlight.png";

const int PIECES_COLS = 6;
const int PIECES_WIDTH = 128;
const int PIECES_HEIGHT = 342;
const int SQUARE_WIDTH = 80;
const float PIECES_SCALE = 0.35f;
const int TITLE_WIDTH = 1024;
const int LOGO_WIDTH = 392;

// frame indicies for pieces.png
enum PIECE_FRAMES {
    WHITE_PAWN,
    WHITE_ROOK,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_ROOK,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_QUEEN,
    BLACK_KING
};

// audio files required by audio.cpp
// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[] = "audio\\Win\\Wave Bank.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "audio\\Win\\Sound Bank.xsb";
// XGS_FILE must be location of .xgs file.
const char XGS_FILE[] = "audio\\Win\\ChessDX.xgs";

// audio cues
const char ACQUISITION[] = "Acquisition";
const char MORATORIUM[] = "Moratorium";
const char FEROCITY[] = "Ferocity";
const char KNOCK[] = "knock";
const char CAPTURE[] = "capture";

const char* const AUDIO_CUES[] = { ACQUISITION, MORATORIUM, FEROCITY, KNOCK, CAPTURE };
const int NUM_AUDIO_CUES = 5;

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR CONSOLE_KEY  = '`';         // ` key
const UCHAR ESC_KEY      = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY      = VK_MENU;     // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;   // Enter key
const UCHAR LEFT_KEY     = VK_LEFT;     // left arrow
const UCHAR RIGHT_KEY    = VK_RIGHT;    // right arrow
const UCHAR UP_KEY       = VK_UP;       // up arrow
const UCHAR DOWN_KEY     = VK_DOWN;     // down arrow

//=============================================================================
// Function templates for safely dealing with pointer referenced items.
// The functions defined by these templates may be called using a normal
// function call syntax. The compiler will create a function that replaces T
// with the type of the calling parameter.
//=============================================================================
// Safely release pointer referenced item
template <typename T>
inline void safeRelease(T& ptr)
{
    if (ptr)
    { 
        ptr->Release(); 
        ptr = NULL;
    }
}
#define SAFE_RELEASE safeRelease            // for backward compatiblility

// Safely delete pointer referenced item
template <typename T>
inline void safeDelete(T& ptr)
{
    if (ptr)
    { 
        delete ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE safeDelete              // for backward compatiblility

// Safely delete pointer referenced array
template <typename T>
inline void safeDeleteArray(T& ptr)
{
    if (ptr)
    { 
        delete[] ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE_ARRAY safeDeleteArray   // for backward compatiblility

// Safely call onLostDevice
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
    if (ptr)
        ptr->onLostDevice(); 
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice    // for backward compatiblility

// Safely call onResetDevice
template <typename T>
inline void safeOnResetDevice(T& ptr)
{
    if (ptr)
        ptr->onResetDevice(); 
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice  // for backward compatiblility

#endif
