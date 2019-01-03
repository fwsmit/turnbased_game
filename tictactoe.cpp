#include "tictactoe.h"
#include "Bot.h"
#include <iostream>
#include <string>
#include <sstream>

tictactoe::tictactoe(bool _isFirstPlayerAI, bool _isSecondPlayerAI, bool _commandLineMode) : Game(_isFirstPlayerAI, _isSecondPlayerAI, _commandLineMode){
	board.setSize(3, 3);
	//board.set(2, 0, pieceType::player1);
}

bool tictactoe::isLegalMove(Move move) const{
	if(move.x < 0 || move.y < 0 || move.x >= getXSize() || move.y >= getYSize()){ //check if move is in bounds
		return false;
	}
	if(board.get(move.x, move.y) != pieceType::empty){ // check if place is occupied
		return false;
	}
	return true;
}

bool tictactoe::hasEnded(){
	return isBoardFull() || hasWon();
}

void tictactoe::playMove(Move move){
	if(isLegalMove(move)){
		//std::cout << "Playing move " << move.x << ", " << move.y << std::endl;
		board.set(move.x, move.y, getCurrentPlayerPiece());
		//std::cout << "Has current player won? " << std::boolalpha << hasWon() << std::endl;
	}
	else{ // illegal move
		std::cout << "Move " << move.x << ", " << move.y << " is illegal" << std::endl;
	}
}

bool tictactoe::hasWon() {
    int count_cross_forward = 0; /*       "/"     (top right to left bottom)*/
    int count_cross_backward = 0; /*      "\"     (top left to right bottom)*/
    for(int i = 0; i < 3; i++) {
		int count_vertical = 0;
		int count_horizontal = 0;
        for(int j = 0; j < 3; j++) {
            if(board.get(i, j) == getCurrentPlayerPiece().getValue()) {
                count_vertical++;
            }
            if(board.get(j, i)  == getCurrentPlayerPiece().getValue()) {
                count_horizontal++;
            }
        }

        if(board.get(i, i) == getCurrentPlayerPiece().getValue()) {
            count_cross_backward++;
        }
        if(board.get(2 - i, i) == getCurrentPlayerPiece().getValue()) {
            count_cross_forward++;
        }

        if (count_vertical == 3 || count_horizontal == 3 ) { return true; }
    }
    if(count_cross_backward == 3 || count_cross_forward == 3) { return true; }
    return false;

}

std::vector<Move> tictactoe::getAvailableMoves() const{
	std::vector<Move> result;
	for(int x = 0; x < getXSize(); x++){
		for(int y = 0; y < getYSize(); y++){
			Move actualMove = Move(x, y);
			//std::cout << "checking move " << x << std::endl;
			if(isLegalMove(actualMove)){
				result.push_back(actualMove);
				//std::cout << "Possible move added" << std::endl;
			}
		}
	}
	return result;
}

void tictactoe::printBoard(){
    std::string spacing = "     ";
    std::string horizontalLine = "  --  --  --";
	std::cout << spacing << horizontalLine << std::endl;
	for(int y = 0; y < 3; y++) {
		std::cout << spacing << "| ";
		for(int x = 0; x < 3; x++) {
			std::cout << board.get(x, y) << " | ";
		}
		std::cout << std::endl << spacing << horizontalLine << std::endl;
	}
}

Score tictactoe::getBoardScore(unsigned int depth){
	return Score(Result::t_score, depth, 0);
}

Move tictactoe::getBotMove(){
	return bot.getBestMove(*this, botDepth);
}

Move tictactoe::getUserMove(){
	if(commandLineMode){
		std::cout << "type X_POSITION, Y_POSITION and press enter" << std::endl;
		int x, y;
		std::string input;

		while(1) {
			std::getline (std::cin, input);
			std::size_t found = input.find(",");
			if(found != std::string::npos) {
				input.replace(found, 1, " ");
			}
			std::cout << "input " << input << std::endl;
			std::stringstream ss(input);
			if(ss >> x >> y && isLegalMove(Move(x-1, y-1))) {
				break;
			}
			std::cout << "input not correct" << std::endl;
			std::cin.clear();
		}
		return Move(x-1, y-1);
	}
	else{
		std::cout << "This game only works in command line mode" << std::endl;
		assert(0);
		return Move(0, 0);
	}
}

void tictactoe::startGame(){
	if(commandLineMode){
		printBoard();
		while(!hasEnded()){
			nextPlayer();
			Move move;
			if(isAITurn()){
				std::cout << "it's player " << getCurrentPlayerPiece().getValue() << "'s turn" << std::endl;
				std::cout << "(bot's turn)" << std::endl;
				move = getBotMove();
				std::cout << "Bot's move: ";
				printMove(move);
			}
			else{
				std::cout << "it's player " << getCurrentPlayerPiece().getValue() << "'s turn" << std::endl;
				move = getUserMove();
			}
			playMove(move);
			std::cout << "Possible move size " << getAvailableMoves().size() << std::endl;
			printBoard();
			if(hasWon()){
				std::cout << "Game has ended, player " << getCurrentPlayerPiece().getValue() << " has won" << std::endl;
			}
			else{
				std::cout << "This game has ended in a tie" << std::endl;
			}
		}
	}
	else{
		std::cout << "This game only works in command line mode" << std::endl;
		assert(0);
	}
}

void tictactoe::testGame(){
	printBoard();
	std::cout << "Current player: " << getCurrentPlayerPiece().getValue() << std::endl; 
	bot.testBot(*this, botDepth);
}
