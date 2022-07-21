#pragma once
#ifndef GS
#define GS

#include "Background.h"

class GameState
{
public:
	GameState(){}
	virtual ~GameState() {}

	virtual void processInput(sf::Event& event, Window& window) = 0;
	virtual void update() = 0;
	virtual void display(Window& window) = 0;
protected:
	Background m_background;
};

#endif 