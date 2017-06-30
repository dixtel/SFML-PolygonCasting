#include "polygoncast.h"


PolygonCast::PolygonCast() {
}

std::vector <Wall*> PolygonCast::GetWallsOnPlayerView(sf::Vector2f player_center_pos, float player_dir, std::vector <Wall*> walls) {
    std::vector <sf::Vector2f> viewTriangle;
    viewTriangle.push_back(player_center_pos);
    viewTriangle.push_back(sf::Vector2f(player_center_pos.x + (ToolKit::cosine(fmod((player_dir + (angleView/2)), 360)) * distanceView), player_center_pos.y + (-ToolKit::sine(fmod((player_dir + (angleView/2)), 360)) * distanceView)));
    viewTriangle.push_back(sf::Vector2f(player_center_pos.x + (ToolKit::cosine(fmod((player_dir - (angleView/2)), 360)) * distanceView), player_center_pos.y + (-ToolKit::sine(fmod((player_dir - (angleView/2)), 360)) * distanceView)));

    sf::Vector2f centerTriangle;
    centerTriangle = sf::Vector2f(player_center_pos.x + ToolKit::cosine(player_dir) * (distanceView/2), player_center_pos.y + (-ToolKit::sine(player_dir)) * (distanceView/2));

    std::vector <Wall*> viewWalls;

    for(int i = 0; i < walls.size(); ++i) {
        std::vector <sf::Vector2f> wallPoints;
        wallPoints.push_back(walls[i]->GetPointPosition('A'));
        wallPoints.push_back(walls[i]->GetPointPosition('B'));
        wallPoints.push_back(walls[i]->GetPointPosition('C'));
        wallPoints.push_back(walls[i]->GetPointPosition('D'));

        bool wallPointIntersection = true;

        for(int j = 0; j < 4; ++j) {
           sf::Vector2f wallPoint = wallPoints[j];

           if(!ToolKit::GetIntersectPosition(viewTriangle[0], viewTriangle[1], centerTriangle, wallPoint).is_intersection &&
              !ToolKit::GetIntersectPosition(viewTriangle[1], viewTriangle[2], centerTriangle, wallPoint).is_intersection &&
              !ToolKit::GetIntersectPosition(viewTriangle[2], viewTriangle[0], centerTriangle, wallPoint).is_intersection) {
               wallPointIntersection = false;
               break;
           }
        }

        if(!wallPointIntersection) {
            viewWalls.push_back(walls[i]);
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

std::vector <PolygonCast::Surface*> PolygonCast::GetView(Player *player, std::vector <Wall*> walls) {
    if((angleView == 0) && (distanceView == 0)) {
        std::cout << "error: angle and distance for view are not init (PolygonCast)" << std::endl;
        return std::vector <PolygonCast::Surface*>  {};
    }


    float player_dir = player->GetDirection();
    sf::Vector2f player_pos = player->GetPosition();
    sf::Vector2f player_center_pos = sf::Vector2f(player_pos.x + (player->GetSize().x/2), player_pos.y + (player->GetSize().y/2));


    std::vector <Wall*> viewWalls = GetWallsOnPlayerView(player_center_pos, player_dir, walls);
    std::cout << "size view: " << viewWalls.size() << std::endl;


    return std::vector <PolygonCast::Surface*>  {};
}

