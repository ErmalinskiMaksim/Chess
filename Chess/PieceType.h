#pragma once
#ifndef PIECETYPE_H
#define PIECETYPE_H

#include "SFML/System/Vector2.hpp"
#include "Constants.h"
#include <cmath>
#include <map>
#include <vector>
#include <array>

extern const float SQUARE_WIDTH;
extern const float SQUARE_HEIGHT;

enum class TeamColour{ BLACK = 0, WHITE = 1};

class PieceType
{
public:
	PieceType() : m_movesFirstTime(true), m_possibleMoves(0){}
	virtual ~PieceType(){}

	virtual TeamColour getColour() const;
	virtual const char* getDirectory() const;
	virtual const char* getName()const;
	virtual bool isFirstTime() const;

	virtual void setFirstTime(bool b);

	virtual void checkTheKing(); 
	virtual void checkIfMyKingIsChecked(); // I adore the name of this function
	virtual void checkIfIsPinned();// if it's pinned it removes the illegal moves
	virtual void calculatePossibleMoves() = 0; // calculates all moves (including illegal)
	
public:
	struct Coordinates
	{
		Coordinates(){}
		Coordinates(int x, int y) : x(x), y(y){}
		void convertToBoard(sf::Vector2f position);
		sf::Vector2f convertToPosition(sf::Vector2i coordinates);
		bool operator<(const Coordinates c)const;
		bool operator==(const Coordinates c)const;
		int x, y;
	}m_coordinates; // coordinates on the board
	std::vector<PieceType::Coordinates> m_possibleMoves; // piece's avilable moves

	static std::map<PieceType::Coordinates, PieceType*> s_positionInfo; // a map which contains all pieces on the board
	static std::vector<PieceType::Coordinates> s_dangerousPieces; // pieces that check the king
	static std::pair<bool, TeamColour> s_checkedKing; // the currently checked king

protected:
	virtual void eraseDangerousCells(PieceType::Coordinates defendor, PieceType::Coordinates kingPos) = 0; // erases illegal moves 
	virtual PieceType::Coordinates findMyKingPosition(); // finds the king position
protected:
	const char* m_name;
	const char* m_textureDirectory;
	TeamColour m_teamColour;
	bool m_movesFirstTime;
};



class Pawn : public PieceType
{
public:
	Pawn(TeamColour tc);
	void calculatePossibleMoves()override;
	void eraseDangerousCells(PieceType::Coordinates defendor, PieceType::Coordinates kingPos)override;
};

class Rook : public PieceType
{
public:
	Rook(TeamColour tc);
	void calculatePossibleMoves()override;
	void eraseDangerousCells(PieceType::Coordinates defendor, PieceType::Coordinates kingPos)override;
};

class Knight : public PieceType
{
public:
	Knight(TeamColour tc);
	void calculatePossibleMoves()override;
	void eraseDangerousCells(PieceType::Coordinates defendor, PieceType::Coordinates kingPos)override;
};

class Bishop : public PieceType
{
public:
	Bishop(TeamColour tc);
	void calculatePossibleMoves()override;
	void eraseDangerousCells(PieceType::Coordinates defendor, PieceType::Coordinates kingPos)override;
};

class Queen : public PieceType
{
public:
	Queen(TeamColour tc);
	void calculatePossibleMoves()override;
	void eraseDangerousCells(PieceType::Coordinates defendor, PieceType::Coordinates kingPos)override;
};

class King : public PieceType
{
public:
	King(TeamColour tc);
	void calculatePossibleMoves()override;
	void eraseDangerousCells(PieceType::Coordinates defendor, PieceType::Coordinates kingPos)override;
};

#endif