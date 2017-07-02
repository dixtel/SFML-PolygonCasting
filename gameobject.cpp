#include "gameobject.h"

GameObject::GameObject() {

}

GameObject::~GameObject() {

}

void GameObject::CreatePlayer(sf::Vector2f position, sf::Vector2f size, std::string texturePath) {
    Player plr(position, size, texturePath);
    player = plr;
}

void GameObject::CreateWall(sf::Vector2f position, sf::Vector2f size, float angle, std::string texturePath) {
    Wall wall(position, size, angle, texturePath);
    walls.push_back(wall);
}


Player &GameObject::GetPlayer() {
    return player;
}

std::vector <Wall> &GameObject::GetWalls() {
    return walls;
}

