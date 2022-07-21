#pragma once
#ifndef PIECE_H
#define PIECE_H

#include "Entity.h"
#include "PieceType.h"

class Piece : public Entity
{
public:
	Piece() : m_hiden(false), m_selected(false), m_type(nullptr), m_availableCells(0){}
	~Piece() { delete m_type; }

	void processEvents(sf::Event& event, Window& window) override;
	void update();
	void display(Window& window) override;

	PieceType* getType() const;
	void setType(PieceType* type);

	static bool s_isTurnOver;
	
private:

	std::vector<sf::RectangleShape> m_availableCells;
	PieceType* m_type;
	bool m_selected;
	bool m_hiden;

	static bool s_anyPiecesSelected;
};

#endif