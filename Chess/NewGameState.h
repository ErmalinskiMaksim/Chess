#pragma once

#ifndef NGS
#define NGS
#include "GameState.h"
#include "Piece.h"
#include <iostream>

class NewGameState : public GameState
{
public:
	NewGameState();

	void processInput(sf::Event& event, Window& window) override;
	void update() override;
	void display(Window& window) override;
private:
	void initializeTeams();
	void processTeamInput(sf::Event& event, Window& window);
	void updateTeams();
	void displayTeams(Window& window);

	void displayBackground(Window& window);

	std::array<Piece, 16> m_teamBlack;
	std::array<Piece, 16> m_teamWhite;

	TeamColour m_teamThatMoves;
	bool m_isGameOver;
};

#endif 