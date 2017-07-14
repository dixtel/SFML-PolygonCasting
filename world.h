#ifndef WORLD_H
#define WORLD_H

#include "gameobject.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>


/* txt file of object exmaple
    type:player position:50.3200,50.0540 size:10.23,10.001,0.5 height:0 angle:240023 texturepath:texturepath/path.png /n

    Description:
    Types: player.
    Position and size to 4 digits after comma.
    Angle 0 is on the right and moving clockwise, Max 6 digits
    Spacebar is separation.

 */

struct Object {
    std::string       type;
    std::string       texturePath;
    sf::Vector2f      position;
    sf::Vector2f      size;
    float             height;
    float             angle;
};

class World
{
    std::vector <Object> objects;
    bool                 is_load = false;

    sf::VertexArray floor;
    sf::VertexArray ceiling;
    sf::Texture floorTexture;
    sf::Texture ceilingTexture;
    sf::RenderStates floorRenderState;
    sf::RenderStates ceilingRenderState;
public:
    World();

    void LoadMap(const std::string path);
    void InitGameObjects(GameObject *gameObject);

    sf::VertexArray GetFloor();
    sf::VertexArray GetCeiling();
    sf::RenderStates &GetFloorRenderState();
    sf::RenderStates &GetCeilingRenderState();

    bool isLoad();
};

#endif // WORLD_H
