#ifndef TOOLKIT_H
#define TOOLKIT_H

#include "SFML/System.hpp"

#define PI 3.14159265

namespace ToolKit {
    typedef struct {
        bool is_intersection;
        sf::Vector2f position; // position intersect
        sf::Vector2f A1;       //positions of line intersection;
        sf::Vector2f A2;
        sf::Vector2f B1;       //positions of line to intersection;
        sf::Vector2f B2;
    } intersect_propeties;

    typedef struct {
        sf::Vector2f A;
        sf::Vector2f B;
    } Vector2f_pair;


    float GetAngle(sf::Vector2f start, sf::Vector2f end);

    float cosine(float degree);

    float sine(float degree);

    intersect_propeties GetIntersectPosition(sf::Vector2f A1, sf::Vector2f A2, sf::Vector2f B1, sf::Vector2f B2);
}

#endif // TOOLKIT_H
