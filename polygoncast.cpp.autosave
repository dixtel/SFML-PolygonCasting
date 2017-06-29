#include "polygoncast.h"


PolygonCast::PolygonCast() {
}

std::vector <Wall*> PolygonCast::GetWallsOnPlayerView(sf::Vector2f player_pos, float player_dir, std::vector <Wall*> walls) {
    std::vector <sf::Vector2f> viewTriangle;
    sf::Vector2f centerTriangle;
    viewTriangle.push_back(player_pos);
    viewTriangle.push_back(sf::Vector2f(ToolKit::cosine(fmod((player_dir + (angleView/2)), 360)) * distanceView, ToolKit::sine(fmod((player_dir + (angleView/2)), 360)) * distanceView));
    viewTriangle.push_back(sf::Vector2f(ToolKit::cosine(fmod((player_dir - (angleView/2)), 360)) * distanceView, ToolKit::sine(fmod((player_dir - (angleView/2)), 360)) * distanceView));
    centerTriangle = sf::Vector2f(ToolKit::cosine(player_dir) * (distanceView/2), ToolKit::sine(player_dir) * (distanceView/2));

    std::vector <Wall*> viewWalls;

    for(int i = 0; i < walls.size(); ++i) {
        std::vector <sf::Vector2f> wallPoints;
        wallPoints.push_back(walls[i]->GetPointPosition('A'));
        wallPoints.push_back(walls[i]->GetPointPosition('B'));
        wallPoints.push_back(walls[i]->GetPointPosition('C'));
        wallPoints.push_back(walls[i]->GetPointPosition('D'));

        for (int j = 0; j < 5; ++j) {
           sf::Vector2f wallPoint = wallPoints[i%4];

           for (int k = 0; k < 4; ++k) {
               sf::Vector2f trianglePoint1 = viewTriangle[i];
               sf::Vector2f trianglePoint2 = viewTriangle[i%3];

               if(ToolKit::GetIntersectPosition(trianglePoint1, trianglePoint2, centerTriangle, wallPoint).is_intersection) {
                   viewWalls.push_back(walls[i]);
               }
           }
        }
    }

    return viewWalls;
}

void PolygonCast::SetAngleView(float angle) {
    angleView = angle;
}

void PolygonCast::SetDistanceView(float distance) {
    distanceView = distance;
}

std::vector <sf::VertexArray> PolygonCast::GetView(Player *player, std::vector <Wall*> walls) {
    float player_dir = player->GetDirection();
    sf::Vector2f player_pos = player->GetPosition();

    std::vector <Wall*> viewWalls = GetWallsOnPlayerView(player_pos ,player_dir, walls);
    std::cout << "size view: " << viewWalls.size() << std::endl;
    return std::vector <sf::VertexArray> {};
}

