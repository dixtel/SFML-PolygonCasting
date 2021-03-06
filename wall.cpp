#include "wall.h"

Wall::Wall(sf::Vector2f position, sf::Vector2f size, float height, float angle, std::string texturePath) {
    this->position = position;
    this->size     = size;
    this->height   = height;
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

    pos_A = sf::Vector2f(wall.getPosition().x, wall.getPosition().y);
    pos_B = sf::Vector2f(wall.getPosition().x + wall.getSize().x, wall.getPosition().y);
    pos_C = sf::Vector2f(wall.getPosition().x + wall.getSize().x, wall.getPosition().y + wall.getSize().y);
    pos_D = sf::Vector2f(wall.getPosition().x, wall.getPosition().y + wall.getSize().y);

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

sf::RectangleShape &Wall::GetRect() {
    return wall;
}

sf::Vector2f Wall::GetPosition() {
    return position;
}

sf::Vector2f Wall::GetSize() {
    return size;
}

float Wall::GetHeight() {
    return height;
}

std::string Wall::GetTexturePath() {
    return texturePath;
}

