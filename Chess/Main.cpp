#include "Game.h"

//			file map
// 
// Main				SFML/Graphics.hpp	
//  |				  |
//  V				  V
// Game.h			Window.h ----> Entity.h
// 	^							 /   |
//  |							/    V
//GameState.h <------Background.h	Piece.h
//  |								|	^
//  V								|	|
//NewGameState.h <------------------|	|----PieceType.h <--- other crucial libs
// 


///TO DO
/// 1) Castling, En Passant, Promotion
/// 2) Move list and navigation through the list
/// 3) Menu


int main()
{
	Game game;
	game.run();

	return 0;
}