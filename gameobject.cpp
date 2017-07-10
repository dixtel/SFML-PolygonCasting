#include "gameobject.h"

GameObject::GameObject() {

}

GameObject::~GameObject() {

}

void GameObject::CreatePlayer(sf::Vector2f position, sf::Vector2f size, std::string texturePath) {
    player = Player(position, size, texturePath);
}

void GameObject::CreateWall(sf::Vector2f position, sf::Vector2f size, float height, float angle, std::string texturePath) {
    walls.push_back(Wall(position, size, height, angle, texturePath));
}


Player &GameObject::GetPlayer() {
    return player;
}

std::vector <Wall> &GameObject::GetWalls() {
    return walls;
}

