#include "Game.h"


void Game::run()
{
	while (m_window.isOpen())
	{
		processInput();
		update();
		display();
	}
}

void Game::processInput()
{
	sf::Event e;

	if (m_window.waitEven(e))
	{
		if (m_window.checkIfWasClosed(e))
			return;
		
		m_gameState->processInput(e, m_window);
	}
}

void Game::update()
{
	m_gameState->update();
}

void Game::display()
{
	m_window.clear();
	m_gameState->display(m_window);
	m_window.display();
}

Game::~Game()
{
	delete m_gameState;
}