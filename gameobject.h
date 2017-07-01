#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "player.h"
#include "wall.h"
#include <string>

class GameObject
{
    Player             player;
    std::vector <Wall> walls;
public:
    GameObject();
    ~GameObject();

    //init
    void CreatePlayer(sf::Vector2f position, sf::Vector2f size, std::string texturePath);
    void CreateWall(sf::Vector2f position, sf::Vector2f size, float angle, std::string texturePath);

    //get
    Player &GetPlayer();
    std::vector <Wall> &GetWalls();

};

#endif // GAMEOBJECT_H
