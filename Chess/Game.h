#pragma once
#ifndef GAME_H
#define GAME_H
 
#include "NewGameState.h"

class Game
{
public:
	Game(): m_gameState(new NewGameState){}
	~Game();
	void run();
private:
	void processInput();
	void update();
	void display();

	GameState* m_gameState;
	Window m_window;
};

#endif