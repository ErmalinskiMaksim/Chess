#include "Entity.h"

Entity::Entity(const char* dir)
{
    setTexture(dir);
}

sf::Sprite Entity::getSprite()
{
    return m_sprite;
}

void Entity::setTexture(const char* directory)
{
    m_texture.loadFromFile(directory);
    m_sprite.setTexture(m_texture);
}

void Entity::setPosition(sf::Vector2f position)
{
    m_sprite.setPosition(position);
}
