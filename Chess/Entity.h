#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "Window.h"

class Entity
{
public:
	Entity(){}
	Entity(const char* dir);
	virtual ~Entity(){}

	virtual sf::Sprite getSprite();
	virtual void setTexture(const char* directory);
	virtual void setPosition(sf::Vector2f position);

	virtual void processEvents(sf::Event& event, Window& window) = 0;
	virtual void display(Window& window) = 0;
protected:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
};

#endif