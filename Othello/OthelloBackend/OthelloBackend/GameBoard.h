#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <iostream>

#include "array2D.h"
#include "OthelloNS.h"

namespace othello {
	class GameBoard {
	private:
		array2D<TileType> BoardArray;
	public:
		GameBoard();
		~GameBoard();
		TileType GetTile(const int i, const int j)const;
		void SetTile(const int i, const int j, const TileType TileIn);
		void GetTileCount(int& BlackCount, int& WhiteCount, int& EmptyCount)const;
	};
}
#endif