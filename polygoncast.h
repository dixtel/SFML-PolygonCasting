#ifndef POLYGONCAST_H
#define POLYGONCAST_H

#include <SFML/Graphics.hpp>
#include <gameobject.h>
#include <vector>

class PolygonCast
{
public:
    PolygonCast();

    std::vector <sf::VertexArray> GetView(Player *player, std::vector <Wall*>);
};

#endif // POLYGONCAST_H
