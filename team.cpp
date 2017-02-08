#include "chess.h"
#include "team.h"
#include "piece.h"

Team::Team(Chess* g) {
    game = g;
    audio = game->getAudio();
    input = game->getInput();
    inCheck = false;
    inCheckMate = false;
    inStaleMate = false;
    message = "";
    messageX = 0;
    messageY = 0;
    holding = false;
}

bool Team::update() {
    if (TakeTurn()) {
        inCheck = false;
        return true;
    }
    return false;
}

bool Team::TakeTurn() {
    if (input->anyKeyPressed()) {
        if (input->wasKeyPressed('A') || input->wasKeyPressed(LEFT_KEY)) {
            if (cursorX > 0) {
                --cursorX;
            }
        }
        else if (input->wasKeyPressed('D') || input->wasKeyPressed(RIGHT_KEY)) {
            if (cursorX < BOARD_WIDTH - 1) {
                ++cursorX;
            }
        }
        else if (input->wasKeyPressed('W') || input->wasKeyPressed(UP_KEY)) {
            if (cursorY > 0) {
                --cursorY;
            }
        }
        else if (input->wasKeyPressed('S') || input->wasKeyPressed(DOWN_KEY)) {
            if (cursorY < BOARD_HEIGHT - 1) {
                ++cursorY;
            }
        }
        else if (input->wasKeyPressed(ENTER_KEY)) {
            if (holding) {
                if (CanMoveTo(held, cursorX, cursorY) &&
                    !WillCauseCheck(held, cursorX, cursorY)) {
                    if (game->IsPieceAt(cursorX, cursorY, opp->getPieces())) {
                        int oppPiece = game->PieceAt(cursorX, cursorY, opp->getPieces());
                        opp->getPieces()[oppPiece].setGridX(-1);
                        opp->getPieces()[oppPiece].setGridY(-1);
                        audio->playCue(CAPTURE);
                    }
                    holding = false;
                    pieces[held].setGridX(cursorX);
                    pieces[held].setGridY(cursorY);
                    if (pieces[held].getRank() == PAWN && (cursorY == 0 || cursorY == BOARD_HEIGHT - 1)) {
                        pieces[held].setRank(QUEEN);
                        if (pieces[held].getCurrentFrame() == WHITE_PAWN) {
                            pieces[held].setCurrentFrame(WHITE_QUEEN);
                        }
                        else {
                            pieces[held].setCurrentFrame(BLACK_QUEEN);
                        }
                    }
                    audio->playCue(KNOCK);
                    return true;
                }
            }
            else {
                if (game->IsPieceAt(cursorX, cursorY, pieces)) {
                    held = (PIECE_INDICIES)game->PieceAt(cursorX, cursorY, pieces);
                    if (CanMove(held)) {
                        holding = true;
                    }
                }
            }
        }
        else if (input->wasKeyPressed('X')) {
            holding = false;
        }
    }
    return false;
}

void Team::updateMessage(float ft) {
    float diff;
    if (inCheck || inStaleMate) {
        diff = messageY - (messageBaseY + 50 * (messageBaseY < 0 ? 1 : -1));
    }
    else {
        diff = messageY - (messageBaseY);
    }
    if (diff > 1) {
        messageY = messageY - ft * MESSAGE_SPEED;
    }
    else if (diff < -1) {
        messageY = messageY + ft * MESSAGE_SPEED;
    }
}

void Team::CheckForCheck() {
    if (IsInCheck()) {
        inCheck = true;
        message = "CHECK";
        if (IsInCheckMate()) {
            message = "CHECKMATE";
            messageX = MESSAGE_CHECKMATE_X;
            inCheckMate = true;
        }
    }
    else if (IsInStaleMate()) {
        message = "STALEMATE";
        messageX = MESSAGE_CHECKMATE_X;
        inStaleMate = true;
    }
}

bool Team::IsInCheck() {
    int kingX = pieces[KING1].getGridX();
    int kingY = pieces[KING1].getGridY();
    for (int i = 0; i < NUM_PIECES; ++i) {
        if (opp->CanMoveTo(i, kingX, kingY)) {
            return true;
        }
    }
    return false;
}

bool Team::IsInCheckMate() {
    if (IsInCheck()) {
        for (int i = 0; i < NUM_PIECES; ++i) {
            for (int x = 0; x < BOARD_WIDTH; ++x) {
                for (int y = 0; y < BOARD_HEIGHT; ++y) {
                    if (CanMoveTo(i, x, y) && !WillCauseCheck(i, x, y)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

bool Team::IsInStaleMate() {
    if (!IsInCheck()) {
        for (int i = 0; i < NUM_PIECES; ++i) {
            for (int x = 0; x < BOARD_WIDTH; ++x) {
                for (int y = 0; y < BOARD_HEIGHT; ++y) {
                    if (CanMoveTo(i, x, y) && !WillCauseCheck(i, x, y)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

bool Team::CanMove(int piece) {
    for (int x = 0; x < BOARD_WIDTH; ++x) {
        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            if (CanMoveTo(piece, x, y) && !WillCauseCheck(piece, x, y)) {
                return true;
            }
        }
    }
    return false;
}

bool Team::CanMoveTo(int piece, int x, int y) {
    bool canMove = false;
    if (pieces[piece].getGridX() == -1 && pieces[piece].getGridY() == -1) {
        return false;
    }
    if (god && !game->IsPieceAt(x, y)) {
        return true;
    }
    switch (pieces[piece].getRank()) {
    case PAWN:
        if (x == pieces[piece].getGridX() &&
            y == pieces[piece].getGridY() + pawnDirection &&
            !game->IsPieceAt(x, y)) {
            canMove = true;
        }
        if ((x == pieces[piece].getGridX() + 1 || x == pieces[piece].getGridX() - 1) &&
            y == pieces[piece].getGridY() + pawnDirection &&
            game->IsPieceAt(x, y, opp->getPieces())) {
            canMove = true;
        }
        if (x == pieces[piece].getGridX() &&
            abs(y - pieces[piece].getGridY()) == 2 &&
            ((pieces[piece].getGridY() == 1 && pawnDirection == 1) || (pieces[piece].getGridY() == 6) && pawnDirection == -1) &&
            !game->IsPieceAt(x, y) &&
            !game->IsPieceAt(x, y - pawnDirection)) {
            canMove = true;
        }
        break;
    case ROOK:
        if ((x == pieces[piece].getGridX() || y == pieces[piece].getGridY()) &&
            !game->IsPieceBetween(x, y, pieces[piece].getGridX(), pieces[piece].getGridY()) &&
            !game->IsPieceAt(x, y, pieces)) {
            canMove = true;
        }
        break;
    case KNIGHT:
        if (((abs(x - pieces[piece].getGridX()) == 1 && abs(y - pieces[piece].getGridY()) == 2) ||
            (abs(x - pieces[piece].getGridX()) == 2 && abs(y - pieces[piece].getGridY()) == 1)) &&
            !game->IsPieceAt(x, y, pieces)) {
            canMove = true;
        }
        break;
    case BISHOP:
        if (abs(x - pieces[piece].getGridX()) == abs(y - pieces[piece].getGridY()) &&
            !game->IsPieceBetween(x, y, pieces[piece].getGridX(), pieces[piece].getGridY()) &&
            !game->IsPieceAt(x, y, pieces)) {
            canMove = true;
        }
        break;
    case QUEEN:
        if ((x == pieces[piece].getGridX() || y == pieces[piece].getGridY()) &&
            !game->IsPieceBetween(x, y, pieces[piece].getGridX(), pieces[piece].getGridY()) &&
            !game->IsPieceAt(x, y, pieces)) {
            canMove = true;
        }
        if (abs(x - pieces[piece].getGridX()) == abs(y - pieces[piece].getGridY()) &&
            !game->IsPieceBetween(x, y, pieces[piece].getGridX(), pieces[piece].getGridY()) &&
            !game->IsPieceAt(x, y, pieces)) {
            canMove = true;
        }
        break;
    case KING:
        if (abs(x - pieces[piece].getGridX()) <= 1 &&
            abs(y - pieces[piece].getGridY()) <= 1 &&
            !game->IsPieceAt(x, y, pieces)) {
            canMove = true;
        }
        break;
    default:
       break;
    }
    return canMove;
}

bool Team::WillCauseCheck(int piece, int x, int y) {
    bool check;
    int tempPlayerX = pieces[piece].getGridX();
    int tempPlayerY = pieces[piece].getGridY();
    int oppPiece = -1;
    int tempOppX;
    int tempOppY;
    if (game->IsPieceAt(x, y, opp->getPieces())) {
        oppPiece = game->PieceAt(x, y, opp->getPieces());
        tempOppX = opp->getPieces()[oppPiece].getGridX();
        tempOppY = opp->getPieces()[oppPiece].getGridY();
        opp->getPieces()[oppPiece].setGridX(-1);
        opp->getPieces()[oppPiece].setGridY(-1);
    }
    pieces[piece].setGridX(x);
    pieces[piece].setGridY(y);
    check = IsInCheck();
    pieces[piece].setGridX(tempPlayerX);
    pieces[piece].setGridY(tempPlayerY);
    if (oppPiece >= 0) {
        opp->getPieces()[oppPiece].setGridX(tempOppX);
        opp->getPieces()[oppPiece].setGridY(tempOppY);
    }
    return check;
}
