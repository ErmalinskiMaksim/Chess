#pragma once
#ifndef PIECETYPE_H
#define PIECETYPE_H

#include "SFML/System/Vector2.hpp"
#include "Constants.h"
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

	// getters
	virtual TeamColour getColour() const;
	virtual const char* getDirectory() const;
	virtual const char* getName()const;
	virtual bool isFirstTime() const;

	//setters
	virtual void setFirstTime(bool b);

	// the name says it all
	virtual void checkTheKing(); 

	// checks if can prevent a check
	virtual void tryToPreventCheck(); 

	// if it's pinned it removes the illegal moves
	virtual void checkIfIsPinned();

	// calculates all moves (including illegal)
	virtual void calculatePossibleMoves() = 0;
	
public:
	struct Coordinates
	{
		Coordinates(){}
		Coordinates(int x, int y) : x(x), y(y){}
		void convertToBoard(sf::Vector2f position);
		sf::Vector2f convertToPosition(int x, int y);
		bool operator<(const Coordinates c)const;
		bool operator==(const Coordinates c)const;
		int x, y;
	}m_coordinates; // coordinates on the board

	// piece's avilable moves
	std::vector<PieceType::Coordinates> m_possibleMoves; 

	// a map which contains all pieces on the board
	static std::map<PieceType::Coordinates, PieceType*> s_positionInfo; 

	// pieces that check the king
	static std::vector<PieceType::Coordinates> s_dangerousPieces; 

	// the currently checked king
	static std::pair<bool, TeamColour> s_checkedKing; 

protected:

	// erases illegal moves (both when the king is checked and when safe)
	virtual void eraseDangerousCells(std::vector<PieceType::Coordinates>& possibleMoves, const PieceType::Coordinates& kingPos, bool isKing) = 0;

	// finds the king position
	virtual PieceType::Coordinates findMyKingPosition(); 

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
	void eraseDangerousCells(std::vector<PieceType::Coordinates>& possibleMoves, const PieceType::Coordinates& kingPos, bool isKing)override;
};

class Rook : public PieceType
{
public:
	Rook(TeamColour tc);
	void calculatePossibleMoves()override;
	void eraseDangerousCells(std::vector<PieceType::Coordinates>& possibleMoves, const PieceType::Coordinates& kingPos, bool isKing)override;
};

class Knight : public PieceType
{
public:
	Knight(TeamColour tc);
	void calculatePossibleMoves()override;
	void eraseDangerousCells(std::vector<PieceType::Coordinates>& possibleMoves, const PieceType::Coordinates& kingPos, bool isKing)override;
};

class Bishop : public PieceType
{
public:
	Bishop(TeamColour tc);
	void calculatePossibleMoves()override;
	void eraseDangerousCells(std::vector<PieceType::Coordinates>& possibleMoves, const PieceType::Coordinates& kingPos, bool isKing)override;
};

class Queen : public PieceType
{
public:
	Queen(TeamColour tc);
	void calculatePossibleMoves()override;
	void eraseDangerousCells(std::vector<PieceType::Coordinates>& possibleMoves, const PieceType::Coordinates& kingPos, bool isKing)override;
};

class King : public PieceType
{
public:
	King(TeamColour tc);
	void calculatePossibleMoves()override;
	void eraseDangerousCells(std::vector<PieceType::Coordinates>& possibleMoves, const PieceType::Coordinates& kingPos, bool isKing)override;
};

#endif