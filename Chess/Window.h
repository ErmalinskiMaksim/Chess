#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include "SFML/Graphics.hpp"
#include "Constants.h"

class Window
{
public:
	Window() : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chess"){}
	void draw(const sf::Sprite& sprite);
	void draw(const sf::RectangleShape& shape);

	sf::RenderWindow& getWindow();
	void close();
	void clear();
	void display();
	bool isOpen() const;
	bool checkIfWasClosed(sf::Event& e);
	bool waitEven(sf::Event& e);
private:
	sf::RenderWindow m_window;
};

#endif