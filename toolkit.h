#ifndef TOOLKIT_H
#define TOOLKIT_H

#include "SFML/System.hpp"

#define PI 3.14159265

namespace Toolkit {
    struct intersect_propeties {
        bool is_intersection;
        sf::Vector2f position; // position intersect
        sf::Vector2f A1;       //positions of line intersection;
        sf::Vector2f A2;
        sf::Vector2f B1;       //positions of line to intersection;
        sf::Vector2f B2;
    };

    struct Vector2f_pair {
        sf::Vector2f A;
        sf::Vector2f B;

        bool operator==(Vector2f_pair &pair) {
            if((pair.A == A) && (pair.B == B)) return true;
            return false;
        }
    };


    float GetAngle(sf::Vector2f start, sf::Vector2f end);
    float GetDistance(sf::Vector2f A, sf::Vector2f B);
    float Cosine(float degree);
    float Sine(float degree);
    intersect_propeties GetIntersection(sf::Vector2f A1, sf::Vector2f A2, sf::Vector2f B1, sf::Vector2f B2);
}

#endif // TOOLKIT_H
