// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Daniel Harding
// chess.cpp v1.1
// This class is the core of the game

#include "chess.h"

//=============================================================================
// Constructor
//=============================================================================
Chess::Chess() : Game()
{
    dxFont = new TextDX();  // DirectX font
}

//=============================================================================
// Destructor
//=============================================================================
Chess::~Chess()
{
    releaseAll();           // call onLostDevice() for every graphics item
    safeDelete(dxFont);
    safeDelete(white);
    safeDelete(black);
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Chess::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

    // initialize DirectX font
    // 32 pixel high Arial
    if (dxFont->initialize(graphics, 32, false, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    if (!titlescreenTexture.initialize(graphics, TITLE_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing title screen texture"));

    if (!titlescreen.initialize(graphics, &titlescreenTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing title screen"));

    if (!logoTexture.initialize(graphics, LOGO_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing logo texture"));

    if (!logo.initialize(graphics, &logoTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing logo"));

    if (!board1Texture.initialize(graphics, BOARD1_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing board texture"));

    if (!board2Texture.initialize(graphics, BOARD2_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing board texture"));

    if (!piece1Textures.initialize(graphics, CHESS_PIECES1))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing piece textrues"));

    if (!piece2Textures.initialize(graphics, CHESS_PIECES2))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing piece textrues"));

    if (!piece3Textures.initialize(graphics, CHESS_PIECES3))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing piece textrues"));

    if (!cursorTexture.initialize(graphics, CURSOR_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing cursor texture"));

    if (!cursor.initialize(graphics, &cursorTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing cursor"));

    if (!highlightTexture.initialize(graphics, HIGHLIGHT_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing highlight texture"));

    if (!highlight.initialize(graphics, &highlightTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing highlight"));

    curBoard = 1;
    curSet = 2;

    InitBoard();

    titlescreen.setScale((float)GAME_WIDTH / TITLE_WIDTH);

    logo.setX((GAME_WIDTH - LOGO_WIDTH) / 2.0f);
    logo.setY(GAME_HEIGHT * 0.15f);

    onTitlescreen = true;
    audio->playCue(ACQUISITION);

    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Chess::update()
{
    if (onTitlescreen) {
        if (input->wasKeyPressed(ENTER_KEY)) {
            onTitlescreen = false;
            StartRound();
        }
    }
    else {
        if (!gameOver) {
            if (input->anyKeyPressed()) {
                if (whitesTurn) {
                    if (white->update()) {
                        black->CheckForCheck();
                        if (black->getInCheckMate() || black->getInStaleMate()) {
                            gameOver = true;
                            audio->stopAllCues();
                            audio->playCue(ACQUISITION);
                            return;
                        }
                        whitesTurn = !whitesTurn;
                    }
                }
                else {
                    if (black->update()) {
                        white->CheckForCheck();
                        if (white->getInCheckMate() || white->getInStaleMate()) {
                            gameOver = true;
                            audio->stopAllCues();
                            audio->playCue(ACQUISITION);
                            return;
                        }
                        whitesTurn = !whitesTurn;
                    }
                }
            }
            if (black->getInCheck()) {
                if (black->getMessageY() < MESSAGE1_Y + 50) {
                    black->setMessageY(black->getMessageY() + frameTime * MESSAGE_SPEED);
                }
            }
            else if (black->getMessageY() > MESSAGE1_Y) {
                black->setMessageY(black->getMessageY() - frameTime * MESSAGE_SPEED);
            }
            if (white->getInCheck()) {
                if (white->getMessageY() > MESSAGE2_Y - 50) {
                    white->setMessageY(white->getMessageY() - frameTime * MESSAGE_SPEED);
                }
            }
            else if (white->getMessageY() < MESSAGE2_Y) {
                white->setMessageY(white->getMessageY() + frameTime * MESSAGE_SPEED);
            }
        }
        else {
            if ((black->getInCheckMate() || black->getInStaleMate()) && black->getMessageY() < MESSAGE1_Y + 50) {
                black->setMessageY(black->getMessageY() + frameTime * MESSAGE_SPEED);
            }
            if ((white->getInCheckMate() || white->getInStaleMate()) && white->getMessageY() > MESSAGE2_Y - 50) {
                white->setMessageY(white->getMessageY() - frameTime * MESSAGE_SPEED);
            }
        }
    }
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Chess::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Chess::collisions()
{}

//=============================================================================
// Render game items
//=============================================================================
void Chess::render()
{
    graphics->spriteBegin();                // begin drawing sprites

    if (onTitlescreen) {
        titlescreen.draw();
        logo.draw();
    }
    else {
        board.draw();
        if (!gameOver) {
            if (whitesTurn && white->getHolding()) {
                HighlightMoves(white, black);
            }
            else if (black->getHolding()) {
                HighlightMoves(black, white);
            }
            if (whitesTurn) {
                PrintCursorAt(white->getCursorX(), white->getCursorY(), graphicsNS::WHITE);
                PrintCursorAt(black->getCursorX(), black->getCursorY(), graphicsNS::GRAY & graphicsNS::ALPHA50);
            }
            else {
                PrintCursorAt(black->getCursorX(), black->getCursorY(), graphicsNS::BLACK);
                PrintCursorAt(white->getCursorX(), white->getCursorY(), graphicsNS::GRAY & graphicsNS::ALPHA50);
            }
        }
        PrintPieces();
        dxFont->setFontColor(graphicsNS::WHITE);
        dxFont->print(white->getMessage(), (int)white->getMessageX(), (int)white->getMessageY());
        dxFont->setFontColor(graphicsNS::BLACK);
        dxFont->print(black->getMessage(), (int)black->getMessageX(), (int)black->getMessageY());
    }

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// Process console commands
//=============================================================================
void Chess::consoleCommand()
{
    command = console->getCommand();    // get command from console
    if (command == "")                   // if no command
        return;

    if (command == "help")              // if "help" command
    {
        console->print("Console Commands:");
        console->print("fps - toggle display of frames per second");
        console->print("end turn - switch active player");
        console->print("upgrade - all active player's pawns are now queens");
        console->print("move n x y - move active player's piece, n, to x, y");
        console->print("reset - reset board");
        return;
    }

    if (command == "fps")
    {
        fpsOn = !fpsOn;                 // toggle display of fps
        if (fpsOn)
            console->print("fps On");
        else
            console->print("fps Off");
    }

    if (!onTitlescreen && !gameOver) {
        if (command == "end turn") {
            whitesTurn = !whitesTurn;
        }

        if (command == "upgrade") {
            if (whitesTurn) {
                Piece* pieces = white->getPieces();
                for (int i = 0; i <= PAWN8; ++i) {
                    pieces[i].setRank(QUEEN);
                    pieces[i].setCurrentFrame(WHITE_QUEEN);
                }
            }
            else {
                Piece* pieces = black->getPieces();
                for (int i = 0; i <= PAWN8; ++i) {
                    pieces[i].setRank(QUEEN);
                    pieces[i].setCurrentFrame(BLACK_QUEEN);
                }
            }
        }

        if (command == "downgrade") {
            if (whitesTurn) {
                Piece* pieces = white->getPieces();
                for (int i = 0; i <= PAWN8; ++i) {
                    pieces[i].setRank(PAWN);
                    pieces[i].setCurrentFrame(WHITE_PAWN);
                }
            }
            else {
                Piece* pieces = black->getPieces();
                for (int i = 0; i <= PAWN8; ++i) {
                    pieces[i].setRank(PAWN);
                    pieces[i].setCurrentFrame(BLACK_PAWN);
                }
            }
        }

        if (command.substr(0, 4) == "move") {
            std::smatch match;
            if (std::regex_match(command, match, std::regex("move 0*([0-9]|1[0-5]) 0*([0-7]) 0*([0-7])"))) {
                int piece, x, y;
                piece = std::stoi(match[1].str());
                x = std::stoi(match[2].str());
                y = std::stoi(match[3].str());
                Piece* pieces;
                if (whitesTurn) {
                    pieces = white->getPieces();
                }
                else {
                    pieces = black->getPieces();
                }
                if (!IsPieceAt(x, y)) {
                    pieces[piece].setGridX(x);
                    pieces[piece].setGridY(y);
                }
                else {
                    console->print("Unable to move piece to x, y - space not empty.");
                }
                white->CheckForCheck();
                black->CheckForCheck();
            }
            else {
                console->print("Invalid move.");
            }
        }

        if (command == "rotate") {
            Piece* wPieces = white->getPieces();
            Piece* bPieces = black->getPieces();
            for (int i = 0; i < NUM_PIECES; ++i) {
                if (!(wPieces[i].getGridX() == -1 && wPieces[i].getGridY() == -1)) {
                    wPieces[i].setGridX(7 - wPieces[i].getGridX());
                    wPieces[i].setGridY(7 - wPieces[i].getGridY());
                }
                if (!(bPieces[i].getGridX() == -1 && bPieces[i].getGridY() == -1)) {
                    bPieces[i].setGridX(7 - bPieces[i].getGridX());
                    bPieces[i].setGridY(7 - bPieces[i].getGridY());
                }
            }
            white->setPawnDirection(white->getPawnDirection() * -1);
            black->setPawnDirection(black->getPawnDirection() * -1);
            white->setCursorX(7 - white->getCursorX());
            white->setCursorY(7 - white->getCursorY());
            black->setCursorX(7 - black->getCursorX());
            black->setCursorY(7 - black->getCursorY());
            board.setRadians(fmod(board.getRadians() + (float)PI, 2.0f * (float)PI));
        }

        if (command == "reset") {
            safeDelete(white);
            safeDelete(black);
            StartRound();
        }

        if (command == "god") {
            if (whitesTurn) {
                white->setGod(!white->getGod());
            }
            else {
                black->setGod(!black->getGod());
            }
        }

        if (command.substr(0, 5) == "board") {
            std::smatch match;
            if (std::regex_match(command, match, std::regex("board 0*([0-1])"))) {
                curBoard = std::stoi(match[1].str());
                InitBoard();
            }
        }

        if (command.substr(0, 3) == "set") {
            std::smatch match;
            if (std::regex_match(command, match, std::regex("set 0*([0-2])"))) {
                curSet = std::stoi(match[1].str());
                InitPieces();
            }
        }
    }
    if (command == "sfx off") {
        audio->turnSfxOff();
    }
    if (command == "sfx on") {
        audio->turnSfxOn();
    }
    if (command == "music off") {
        audio->turnMusicOff();
    }
    if (command == "music on") {
        audio->turnMusicOn();
    }
    if (command == "quit") {
        safeDelete(white);
        safeDelete(black);
        onTitlescreen = true;
        audio->stopAllCues();
        audio->playCue(ACQUISITION);
    }
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Chess::releaseAll()
{
    dxFont->onLostDevice();
    titlescreenTexture.onLostDevice();
    logoTexture.onLostDevice();
    board1Texture.onLostDevice();
    board2Texture.onLostDevice();
    piece1Textures.onLostDevice();
    piece2Textures.onLostDevice();
    piece3Textures.onLostDevice();
    cursorTexture.onLostDevice();
    highlightTexture.onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Chess::resetAll()
{
    dxFont->onResetDevice();
    titlescreenTexture.onResetDevice();
    logoTexture.onResetDevice();
    board1Texture.onResetDevice();
    board2Texture.onResetDevice();
    piece1Textures.onResetDevice();
    piece2Textures.onResetDevice();
    piece3Textures.onResetDevice();
    cursorTexture.onResetDevice();
    highlightTexture.onResetDevice();
    Game::resetAll();
    return;
}

void Chess::StartRound() {
    whitesTurn = true;
    gameOver = false;

    white = new Team(this);
    black = new Team(this);

    white->setCursorX(4);
    white->setCursorY(7);
    black->setCursorX(4);
    black->setCursorY(0);

    white->setPawnDirection(-1);
    black->setPawnDirection(1);

    white->setOpp(black);
    black->setOpp(white);

    white->setGod(false);
    black->setGod(false);

    white->setMessageY((float)MESSAGE2_Y);
    black->setMessageY((float)MESSAGE1_Y);

    InitPieces();

    Piece* wPieces = white->getPieces();
    Piece* bPieces = black->getPieces();
    for (int pawn = PAWN1; pawn <= PAWN8; ++pawn) {
        wPieces[pawn].setRank(PAWN);
        wPieces[pawn].setGridX(pawn);
        wPieces[pawn].setGridY(6);
        wPieces[pawn].setCurrentFrame(WHITE_PAWN);
        bPieces[pawn].setRank(PAWN);
        bPieces[pawn].setGridX(pawn);
        bPieces[pawn].setGridY(1);
        bPieces[pawn].setCurrentFrame(BLACK_PAWN);
    }
    wPieces[ROOK1].setRank(ROOK);
    wPieces[ROOK1].setGridX(0);
    wPieces[ROOK1].setGridY(7);
    wPieces[ROOK1].setCurrentFrame(WHITE_ROOK);
    wPieces[ROOK2].setRank(ROOK);
    wPieces[ROOK2].setGridX(7);
    wPieces[ROOK2].setGridY(7);
    wPieces[ROOK2].setCurrentFrame(WHITE_ROOK);
    bPieces[ROOK1].setRank(ROOK);
    bPieces[ROOK1].setGridX(0);
    bPieces[ROOK1].setGridY(0);
    bPieces[ROOK1].setCurrentFrame(BLACK_ROOK);
    bPieces[ROOK2].setRank(ROOK);
    bPieces[ROOK2].setGridX(7);
    bPieces[ROOK2].setGridY(0);
    bPieces[ROOK2].setCurrentFrame(BLACK_ROOK);
    wPieces[KNIGHT1].setRank(KNIGHT);
    wPieces[KNIGHT1].setGridX(1);
    wPieces[KNIGHT1].setGridY(7);
    wPieces[KNIGHT1].setCurrentFrame(WHITE_KNIGHT);
    wPieces[KNIGHT2].setRank(KNIGHT);
    wPieces[KNIGHT2].setGridX(6);
    wPieces[KNIGHT2].setGridY(7);
    wPieces[KNIGHT2].setCurrentFrame(WHITE_KNIGHT);
    bPieces[KNIGHT1].setRank(KNIGHT);
    bPieces[KNIGHT1].setGridX(1);
    bPieces[KNIGHT1].setGridY(0);
    bPieces[KNIGHT1].setCurrentFrame(BLACK_KNIGHT);
    bPieces[KNIGHT2].setRank(KNIGHT);
    bPieces[KNIGHT2].setGridX(6);
    bPieces[KNIGHT2].setGridY(0);
    bPieces[KNIGHT2].setCurrentFrame(BLACK_KNIGHT);
    wPieces[BISHOP1].setRank(BISHOP);
    wPieces[BISHOP1].setGridX(2);
    wPieces[BISHOP1].setGridY(7);
    wPieces[BISHOP1].setCurrentFrame(WHITE_BISHOP);
    wPieces[BISHOP2].setRank(BISHOP);
    wPieces[BISHOP2].setGridX(5);
    wPieces[BISHOP2].setGridY(7);
    wPieces[BISHOP2].setCurrentFrame(WHITE_BISHOP);
    bPieces[BISHOP1].setRank(BISHOP);
    bPieces[BISHOP1].setGridX(2);
    bPieces[BISHOP1].setGridY(0);
    bPieces[BISHOP1].setCurrentFrame(BLACK_BISHOP);
    bPieces[BISHOP2].setRank(BISHOP);
    bPieces[BISHOP2].setGridX(5);
    bPieces[BISHOP2].setGridY(0);
    bPieces[BISHOP2].setCurrentFrame(BLACK_BISHOP);
    wPieces[QUEEN1].setRank(QUEEN);
    wPieces[QUEEN1].setGridX(3);
    wPieces[QUEEN1].setGridY(7);
    wPieces[QUEEN1].setCurrentFrame(WHITE_QUEEN);
    bPieces[QUEEN1].setRank(QUEEN);
    bPieces[QUEEN1].setGridX(3);
    bPieces[QUEEN1].setGridY(0);
    bPieces[QUEEN1].setCurrentFrame(BLACK_QUEEN);
    wPieces[KING1].setRank(KING);
    wPieces[KING1].setGridX(4);
    wPieces[KING1].setGridY(7);
    wPieces[KING1].setCurrentFrame(WHITE_KING);
    bPieces[KING1].setRank(KING);
    bPieces[KING1].setGridX(4);
    bPieces[KING1].setGridY(0);
    bPieces[KING1].setCurrentFrame(BLACK_KING);

    audio->stopAllCues();
    audio->playCue(MORATORIUM);
}

void Chess::InitPieces() {
    Piece* wPieces = white->getPieces();
    Piece* bPieces = black->getPieces();
    int w, h;
    float s;
    TextureManager* p;

    switch (curSet) {
    case 0:
        w = PIECES1_WIDTH;
        h = PIECES1_HEIGHT;
        s = PIECES1_SCALE;
        p = &piece1Textures;
        break;
    case 1:
        w = PIECES2_WIDTH;
        h = PIECES2_HEIGHT;
        s = PIECES2_SCALE;
        p = &piece2Textures;
        break;
    case 2:
        w = PIECES3_WIDTH;
        h = PIECES3_HEIGHT;
        s = PIECES3_SCALE;
        p = &piece3Textures;
        break;
    default:
        break;
    }

    for (int i = 0; i < NUM_PIECES; ++i) {
        if (!wPieces[i].initialize(graphics, w, h, PIECES_COLS, s, p)) {
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing piece"));
        }
        if (!bPieces[i].initialize(graphics, w, h, PIECES_COLS, s, p)) {
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing piece"));
        }
    }
}

void Chess::InitBoard() {
    TextureManager* b;
    switch (curBoard) {
    case 0:
        b = &board1Texture;
        break;
    case 1:
        b = &board2Texture;
        break;
    default:
        break;
    }
    if (!board.initialize(graphics, b))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing board"));
}

void Chess::PrintPieces() {
    Piece* wPieces = white->getPieces();
    Piece* bPieces = black->getPieces();
    for (int j = 0; j < BOARD_HEIGHT; ++j) {
        for (int i = 0; i < BOARD_WIDTH; ++i) {
            int piece = PieceAt(i, j, wPieces);
            if (piece != -1) {
                wPieces[piece].draw();
                continue;
            }
            piece = PieceAt(i, j, bPieces);
            if (piece != -1) {
                bPieces[piece].draw();
            }
        }
    }
}

void Chess::PrintCursorAt(int x, int y, COLOR_ARGB color) {
    cursor.setX(((float)x + 1) * SQUARE_WIDTH);
    cursor.setY(((float)y + 1) * SQUARE_WIDTH);
    cursor.draw(color);
}

void Chess::PrintHighlightAt(int x, int y, COLOR_ARGB color) {
    highlight.setX(((float)x + 1) * SQUARE_WIDTH);
    highlight.setY(((float)y + 1) * SQUARE_WIDTH);
    highlight.draw(color);
}

void Chess::HighlightMoves(Team* player, Team* opp) {
    int held = player->getHeld();
    for (int x = 0; x < BOARD_WIDTH; ++x) {
        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            if (player->CanMoveTo(held, x, y) && !player->WillCauseCheck(held, x, y)) {
                if (IsPieceAt(x, y, opp->getPieces())) {
                    PrintHighlightAt(x, y, graphicsNS::RED & graphicsNS::ALPHA50);
                }
                else {
                    PrintHighlightAt(x, y, graphicsNS::GREEN & graphicsNS::ALPHA50);
                }
            }
        }
    }
}

bool Chess::IsPieceAt(int x, int y) {
    Piece* wPieces = white->getPieces();
    Piece* bPieces = black->getPieces();
    for (int i = 0; i < NUM_PIECES; ++i) {
        if ((wPieces[i].getGridX() == x && wPieces[i].getGridY() == y) ||
            (bPieces[i].getGridX() == x && bPieces[i].getGridY() == y)) {
            return true;
        }
    }
    return false;
}

bool Chess::IsPieceAt(int x, int y, Piece pieces[]) {
    for (int i = 0; i < NUM_PIECES; ++i) {
        if (pieces[i].getGridX() == x && pieces[i].getGridY() == y) {
            return true;
        }
    }
    return false;
}


bool Chess::IsPieceBetween(int x1, int y1, int x2, int y2) {
    if (x1 == x2) {
        if (y2 < y1) {
            int temp = y1;
            y1 = y2;
            y2 = temp;
        }
        for (int i = y1 + 1; i < y2; ++i) {
            if (IsPieceAt(x1, i)) {
                return true;
            }
        }
    }
    else if (y1 == y2) {
        if (x2 < x1) {
            int temp = x1;
            x1 = x2;
            x2 = temp;
        }
        for (int i = x1 + 1; i < x2; ++i) {
            if (IsPieceAt(i, y1)) {
                return true;
            }
        }
    }
    else if (abs(x1 - x2) == abs(y1 - y2)) {
        if (x1 < x2 && y1 < y2) {
            for (int xi = x1 + 1, yi = y1 + 1; xi < x2; ++xi, ++yi) {
                if (IsPieceAt(xi, yi)) {
                    return true;
                }
            }
        }
        else if (x1 < x2 && y1 > y2) {
            for (int xi = x1 + 1, yi = y1 - 1; xi < x2; ++xi, --yi) {
                if (IsPieceAt(xi, yi)) {
                    return true;
                }
            }
        }
        else if (x1 > x2 && y1 < y2) {
            for (int xi = x1 - 1, yi = y1 + 1; xi > x2; --xi, ++yi) {
                if (IsPieceAt(xi, yi)) {
                    return true;
                }
            }
        }
        else { // if (x1 > x2 && y1 > y2) {
            for (int xi = x1 - 1, yi = y1 - 1; xi > x2; --xi, --yi) {
                if (IsPieceAt(xi, yi)) {
                    return true;
                }
            }
        }
    }
    else {
        // invalid call
    }
    return false;
}

int Chess::PieceAt(int x, int y, Piece pieces[]) {
    for (int i = 0; i < NUM_PIECES; ++i) {
        if (pieces[i].getGridX() == x && pieces[i].getGridY() == y) {
            return i;
        }
    }
    return -1;
}
