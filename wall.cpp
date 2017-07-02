#include "wall.h"

Wall::Wall(sf::Vector2f position, sf::Vector2f size, float angle, std::string texturePath) {
    this->position = position;
    this->size     = size;
    this->angle    = angle;
    this->texturePath = texturePath;
    color = sf::Color::Red;

    Update();
}

void Wall::Update() {
    wall.setPosition(position);
    wall.setSize(size);
    wall.setFillColor(color);
    wall.setRotation(angle);

    float space = 0;
    pos_A = sf::Vector2f(wall.getPosition().x+space, wall.getPosition().y+space);
    pos_B = sf::Vector2f(wall.getPosition().x + wall.getSize().x-space, wall.getPosition().y+space);
    pos_C = sf::Vector2f(wall.getPosition().x+space, wall.getPosition().y + wall.getSize().y-space);
    pos_D = sf::Vector2f(wall.getPosition().x + wall.getSize().x-space, wall.getPosition().y + wall.getSize().y-space);
}

sf::RectangleShape &Wall::GetRect() {
    return wall;
}

sf::Vector2f Wall::GetPosition() {
    return position;
}

sf::Vector2f Wall::GetSize() {
    return size;
}

void Wall::SetPosition(sf::Vector2f position) {
    this->position = position;
    Update();
}

sf::Vector2f Wall::GetPointPosition(const char point) {
    switch (point) {
    case 'A':
        return pos_A;
        break;
    case 'B':
        return pos_B;
        break;
    case 'C':
        return pos_C;
        break;
    case 'D':
        return pos_D;
        break;
    }

    return sf::Vector2f(0, 0);
}
