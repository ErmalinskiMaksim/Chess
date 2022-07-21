#pragma once
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Entity.h"

class Background : public Entity
{
public:
	void processEvents(sf::Event& event, Window& window) override{}
	void display(Window& window) override;
};

#endif