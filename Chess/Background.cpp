#include "Background.h"

void Background::display(Window& window)
{
	sf::RectangleShape square({ SQUARE_WIDTH, SQUARE_HEIGHT });

	for (int raw = 0; raw < 8; ++raw)
		for (int column = 0; column < 8; ++column)
		{
			square.setPosition(column * SQUARE_WIDTH, raw * SQUARE_HEIGHT);

			if ((raw + column) % 2)
				square.setFillColor({ 112, 112, 112, 255 });
			else
				square.setFillColor({181, 181, 181, 255});

			window.draw(square);
		}
}