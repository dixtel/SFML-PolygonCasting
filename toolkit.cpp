#include "toolkit.h"

float ToolKit::GetAngle(sf::Vector2f start, sf::Vector2f end) {
    sf::Vector2f direction = start - end;

    float angleRad = atan2(direction.x, direction.y);
    float angleDeg = (angleRad / 3.14159265359) * 180.0;

    if(angleDeg < 0) angleDeg = 360 + angleDeg;
    angleDeg += 90;
    if(angleDeg >= 360) angleDeg = angleDeg - 360;

    return angleDeg;
}

float ToolKit::cosine(float degree) {
    if(degree == 90.0) {
        return 0;
    }
    else if(degree == 270.0) {
        return 0;
    }

    return std::cos(degree * PI / 180);
}

float ToolKit::sine(float degree) {
    if(degree == 0.0) {
        return 0;
    }
    else if(degree == 360.0) {
        return 0;
    }
    else if(degree == 180.0) {
        return 0;
    }

    return std::sin(degree * PI / 180);
}

//TODO change function GetIntersectPosition to GetIntersect

ToolKit::intersect_propeties ToolKit::GetIntersectPosition(sf::Vector2f A1, sf::Vector2f A2, sf::Vector2f B1, sf::Vector2f B2) {
    intersect_propeties result;
    sf::Vector2f s1, s2;
    s1.x = A2.x - A1.x;
    s1.y = A2.y - A1.y;
    s2.x = B2.x - B1.x;
    s2.y = B2.y - B1.y;

    float s, t;
    s = (-s1.y * (A1.x - B1.x) + s1.x * (A1.y - B1.y)) / (-s2.x * s1.y + s1.x * s2.y);
    t = ( s2.x * (A1.y - B1.y) - s2.y * (A1.x - B1.x)) / (-s2.x * s1.y + s1.x * s2.y);

    if(s >= 0 && s <= 1 && t >= 0 && t <= 1) {
        // Collision detected

        float x = A1.x + (t * s1.x);
        float y = A1.y + (t * s1.y);

        result.is_intersection = true;
        result.position = sf::Vector2f(x, y);
        result.A1 = A1;
        result.A2 = A2;
        result.B1 = B1;
        result.B2 = B2;

        return result;
    }

    result.is_intersection = false;

    return result; // No collision
}
