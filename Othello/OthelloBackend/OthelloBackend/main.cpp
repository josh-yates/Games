//Test area for othello backend functionality

#include <iostream>
#include <stdexcept>
#include "OthelloNS.h"
#include "GameBoard.h"

int main() {
	try {
		//Make a board
		othello::GameBoard MyBoard;
		MyBoard.SetTile(2, 2, othello::BlackTile);
		MyBoard.SetTile(4, 4, othello::WhiteTile);
		for (int row{ 0 }; row < othello::BoardSize; ++row) {
			for (int col{ 0 }; col < othello::BoardSize; ++col) {
				switch (MyBoard.GetTile(row, col)) {
				case(othello::EmptyTile):
					std::cout << "#";
					break;
				case(othello::BlackTile):
					std::cout << "X";
					break;
				case(othello::WhiteTile):
					std::cout << "O";
					break;
				}
				std::cout << " ";
			}
			std::cout << std::endl;
		}
		int BlackTileCount{ 10 };
		int WhiteTileCount{ 42 };
		int EmptyTileCount{ 15 };
		MyBoard.GetTileCount(BlackTileCount, WhiteTileCount, EmptyTileCount);
		std::cout << "Black: " << BlackTileCount << std::endl;
		std::cout << "White: " << WhiteTileCount << std::endl;
		std::cout << "Empty: " << EmptyTileCount << std::endl;
	}
	catch (std::invalid_argument& ErrMsg) {
		std::cout << ErrMsg.what() << std::endl;
	}
	getchar();
	return 0;
}