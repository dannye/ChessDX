// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// createThisClass.cpp v1.1
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

    if (!board.initialize(graphics, &board1Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing board"));

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
                for (int i = 0; i <= Team::PAWN8; ++i) {
                    pieces[i].setRank(QUEEN);
                    pieces[i].setCurrentFrame(WHITE_QUEEN);
                }
            }
            else {
                Piece* pieces = black->getPieces();
                for (int i = 0; i <= Team::PAWN8; ++i) {
                    pieces[i].setRank(QUEEN);
                    pieces[i].setCurrentFrame(BLACK_QUEEN);
                }
            }
        }

        if (command == "downgrade") {
            if (whitesTurn) {
                Piece* pieces = white->getPieces();
                for (int i = 0; i <= Team::PAWN8; ++i) {
                    pieces[i].setRank(PAWN);
                    pieces[i].setCurrentFrame(WHITE_PAWN);
                }
            }
            else {
                Piece* pieces = black->getPieces();
                for (int i = 0; i <= Team::PAWN8; ++i) {
                    pieces[i].setRank(PAWN);
                    pieces[i].setCurrentFrame(BLACK_PAWN);
                }
            }
        }

        if (command.substr(0, 4) == "move") {
            if (std::regex_match(command, std::regex("move 0*([0-9]|1[0-5]) 0*[0-7] 0*[0-7]"))) {
                int piece, x, y;
                std::smatch match;
                std::regex_match(command, match, std::regex("move (.+) .+ .+"));
                piece = std::stoi(match[1].str());
                std::regex_match(command, match, std::regex("move .+ (.+) .+"));
                x = std::stoi(match[1].str());
                std::regex_match(command, match, std::regex("move .+ .+ (.+)"));
                y = std::stoi(match[1].str());
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
                int b = std::stoi(match[1].str());
                switch (b) {
                case 0:
                    if (!board.initialize(graphics, &board1Texture))
                        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing board"));
                    break;
                case 1:
                    if (!board.initialize(graphics, &board2Texture))
                        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing board"));
                    break;
                default:
                    break;
                }
            }
        }

        if (command.substr(0, 3) == "set") {
            std::smatch match;
            if (std::regex_match(command, match, std::regex("set 0*([0-2])"))) {
                int s = std::stoi(match[1].str());
                Piece* wPieces = white->getPieces();
                Piece* bPieces = black->getPieces();
                switch (s) {
                case 0:
                    for (int i = 0; i < NUM_PIECES; ++i) {
                        if (!wPieces[i].initialize(graphics, PIECES1_WIDTH, PIECES1_HEIGHT, PIECES_COLS, &piece1Textures))
                            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing piece"));
                        if (!bPieces[i].initialize(graphics, PIECES1_WIDTH, PIECES1_HEIGHT, PIECES_COLS, &piece1Textures))
                            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing piece"));
                        wPieces[i].setScale(PIECES1_SCALE);
                        bPieces[i].setScale(PIECES1_SCALE);
                        wPieces[i].setGridX(wPieces[i].getGridX());
                        wPieces[i].setGridY(wPieces[i].getGridY());
                        bPieces[i].setGridX(bPieces[i].getGridX());
                        bPieces[i].setGridY(bPieces[i].getGridY());
                    }
                    break;
                case 1:
                    for (int i = 0; i < NUM_PIECES; ++i) {
                        if (!wPieces[i].initialize(graphics, PIECES2_WIDTH, PIECES2_HEIGHT, PIECES_COLS, &piece2Textures))
                            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing piece"));
                        if (!bPieces[i].initialize(graphics, PIECES2_WIDTH, PIECES2_HEIGHT, PIECES_COLS, &piece2Textures))
                            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing piece"));
                        wPieces[i].setScale(PIECES2_SCALE);
                        bPieces[i].setScale(PIECES2_SCALE);
                        wPieces[i].setGridX(wPieces[i].getGridX());
                        wPieces[i].setGridY(wPieces[i].getGridY());
                        bPieces[i].setGridX(bPieces[i].getGridX());
                        bPieces[i].setGridY(bPieces[i].getGridY());
                    }
                    break;
                case 2:
                    for (int i = 0; i < NUM_PIECES; ++i) {
                        if (!wPieces[i].initialize(graphics, PIECES3_WIDTH, PIECES3_HEIGHT, PIECES_COLS, &piece3Textures))
                            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing piece"));
                        if (!bPieces[i].initialize(graphics, PIECES3_WIDTH, PIECES3_HEIGHT, PIECES_COLS, &piece3Textures))
                            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing piece"));
                        wPieces[i].setScale(PIECES3_SCALE);
                        bPieces[i].setScale(PIECES3_SCALE);
                        wPieces[i].setGridX(wPieces[i].getGridX());
                        wPieces[i].setGridY(wPieces[i].getGridY());
                        bPieces[i].setGridX(bPieces[i].getGridX());
                        bPieces[i].setGridY(bPieces[i].getGridY());
                    }
                    break;
                default:
                    break;
                }
            }
        }
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

    InitBoard();

    audio->stopAllCues();
    audio->playCue(MORATORIUM);
}

void Chess::InitBoard() {
    Piece* wPieces = white->getPieces();
    Piece* bPieces = black->getPieces();

    for (int i = 0; i < NUM_PIECES; ++i) {
        if (!wPieces[i].initialize(graphics, PIECES1_WIDTH, PIECES1_HEIGHT, PIECES_COLS, &piece1Textures)) {
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing piece"));
        }
        if (!bPieces[i].initialize(graphics, PIECES1_WIDTH, PIECES1_HEIGHT, PIECES_COLS, &piece1Textures)) {
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing piece"));
        }
    }
    for (int pawn = Team::PAWN1; pawn <= Team::PAWN8; ++pawn) {
        wPieces[pawn].setRank(PAWN);
        wPieces[pawn].setGridX(pawn);
        wPieces[pawn].setGridY(6);
        wPieces[pawn].setCurrentFrame(WHITE_PAWN);
        bPieces[pawn].setRank(PAWN);
        bPieces[pawn].setGridX(pawn);
        bPieces[pawn].setGridY(1);
        bPieces[pawn].setCurrentFrame(BLACK_PAWN);
    }
    wPieces[Team::ROOK1].setRank(ROOK);
    wPieces[Team::ROOK1].setGridX(0);
    wPieces[Team::ROOK1].setGridY(7);
    wPieces[Team::ROOK1].setCurrentFrame(WHITE_ROOK);
    wPieces[Team::ROOK2].setRank(ROOK);
    wPieces[Team::ROOK2].setGridX(7);
    wPieces[Team::ROOK2].setGridY(7);
    wPieces[Team::ROOK2].setCurrentFrame(WHITE_ROOK);
    bPieces[Team::ROOK1].setRank(ROOK);
    bPieces[Team::ROOK1].setGridX(0);
    bPieces[Team::ROOK1].setGridY(0);
    bPieces[Team::ROOK1].setCurrentFrame(BLACK_ROOK);
    bPieces[Team::ROOK2].setRank(ROOK);
    bPieces[Team::ROOK2].setGridX(7);
    bPieces[Team::ROOK2].setGridY(0);
    bPieces[Team::ROOK2].setCurrentFrame(BLACK_ROOK);
    wPieces[Team::KNIGHT1].setRank(KNIGHT);
    wPieces[Team::KNIGHT1].setGridX(1);
    wPieces[Team::KNIGHT1].setGridY(7);
    wPieces[Team::KNIGHT1].setCurrentFrame(WHITE_KNIGHT);
    wPieces[Team::KNIGHT2].setRank(KNIGHT);
    wPieces[Team::KNIGHT2].setGridX(6);
    wPieces[Team::KNIGHT2].setGridY(7);
    wPieces[Team::KNIGHT2].setCurrentFrame(WHITE_KNIGHT);
    bPieces[Team::KNIGHT1].setRank(KNIGHT);
    bPieces[Team::KNIGHT1].setGridX(1);
    bPieces[Team::KNIGHT1].setGridY(0);
    bPieces[Team::KNIGHT1].setCurrentFrame(BLACK_KNIGHT);
    bPieces[Team::KNIGHT2].setRank(KNIGHT);
    bPieces[Team::KNIGHT2].setGridX(6);
    bPieces[Team::KNIGHT2].setGridY(0);
    bPieces[Team::KNIGHT2].setCurrentFrame(BLACK_KNIGHT);
    wPieces[Team::BISHOP1].setRank(BISHOP);
    wPieces[Team::BISHOP1].setGridX(2);
    wPieces[Team::BISHOP1].setGridY(7);
    wPieces[Team::BISHOP1].setCurrentFrame(WHITE_BISHOP);
    wPieces[Team::BISHOP2].setRank(BISHOP);
    wPieces[Team::BISHOP2].setGridX(5);
    wPieces[Team::BISHOP2].setGridY(7);
    wPieces[Team::BISHOP2].setCurrentFrame(WHITE_BISHOP);
    bPieces[Team::BISHOP1].setRank(BISHOP);
    bPieces[Team::BISHOP1].setGridX(2);
    bPieces[Team::BISHOP1].setGridY(0);
    bPieces[Team::BISHOP1].setCurrentFrame(BLACK_BISHOP);
    bPieces[Team::BISHOP2].setRank(BISHOP);
    bPieces[Team::BISHOP2].setGridX(5);
    bPieces[Team::BISHOP2].setGridY(0);
    bPieces[Team::BISHOP2].setCurrentFrame(BLACK_BISHOP);
    wPieces[Team::QUEEN1].setRank(QUEEN);
    wPieces[Team::QUEEN1].setGridX(3);
    wPieces[Team::QUEEN1].setGridY(7);
    wPieces[Team::QUEEN1].setCurrentFrame(WHITE_QUEEN);
    bPieces[Team::QUEEN1].setRank(QUEEN);
    bPieces[Team::QUEEN1].setGridX(3);
    bPieces[Team::QUEEN1].setGridY(0);
    bPieces[Team::QUEEN1].setCurrentFrame(BLACK_QUEEN);
    wPieces[Team::KING1].setRank(KING);
    wPieces[Team::KING1].setGridX(4);
    wPieces[Team::KING1].setGridY(7);
    wPieces[Team::KING1].setCurrentFrame(WHITE_KING);
    bPieces[Team::KING1].setRank(KING);
    bPieces[Team::KING1].setGridX(4);
    bPieces[Team::KING1].setGridY(0);
    bPieces[Team::KING1].setCurrentFrame(BLACK_KING);
    
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
