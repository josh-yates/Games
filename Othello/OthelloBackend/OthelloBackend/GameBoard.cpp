#include "GameBoard.h"

namespace othello {
	GameBoard::GameBoard() {
		BoardArray = std::move(array2D<TileType>(BoardSize, BoardSize, EmptyTile));
	}
	GameBoard::~GameBoard() {}
	TileType GameBoard::GetTile(const int i, const int j)const {
		return BoardArray(i, j);
	}
	void GameBoard::SetTile(const int i, const int j, const TileType TileIn) {
		BoardArray.set_element(i, j, TileIn);
	}
	void GameBoard::GetTileCount(int& BlackCount, int& WhiteCount, int& EmptyCount)const {
		BlackCount = 0;
		WhiteCount = 0;
		EmptyCount = 0;
		for (int i{ 0 }; i < BoardArray.get_m_rows(); ++i) {
			for (int j{ 0 }; j < BoardArray.get_n_cols(); ++j) {
				switch (BoardArray(i, j)) {
				case(BlackTile):
					BlackCount += 1;
					break;
				case(WhiteTile):
					WhiteCount += 1;
					break;
				case(EmptyTile):
					EmptyCount += 1;
					break;
				}
			}
		}
	}
}