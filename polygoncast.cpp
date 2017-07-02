#include "polygoncast.h"


PolygonCast::PolygonCast() {
}

void PolygonCast::ClearSurfaces() {
    surfaces.clear();
}

std::vector <Wall> PolygonCast::GetWallsOnPlayerView(std::vector <Wall> *walls) {
    std::vector <sf::Vector2f> viewTriangle;
    viewTriangle.push_back(player_center_pos);
    viewTriangle.push_back(sf::Vector2f(player_center_pos.x + (ToolKit::cosine(fmod((player_dir + (angleView/2)), 360)) * distanceView), player_center_pos.y + (-ToolKit::sine(fmod((player_dir + (angleView/2)), 360)) * distanceView)));
    viewTriangle.push_back(sf::Vector2f(player_center_pos.x + (ToolKit::cosine(fmod((player_dir - (angleView/2)), 360)) * distanceView), player_center_pos.y + (-ToolKit::sine(fmod((player_dir - (angleView/2)), 360)) * distanceView)));

    sf::Vector2f centerTriangle;
    centerTriangle = sf::Vector2f(player_center_pos.x + ToolKit::cosine(player_dir) * (distanceView/2), player_center_pos.y + (-ToolKit::sine(player_dir)) * (distanceView/2));

    std::vector <Wall> viewWalls;

    for(int i = 0; i < walls->size(); ++i) {
        std::vector <sf::Vector2f> wallPoints;
        wallPoints.push_back(walls->at(i).GetPointPosition('A'));
        wallPoints.push_back(walls->at(i).GetPointPosition('B'));
        wallPoints.push_back(walls->at(i).GetPointPosition('C'));
        wallPoints.push_back(walls->at(i).GetPointPosition('D'));

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
            viewWalls.push_back(walls->at(i));
        }

    }

    return viewWalls;
}

void PolygonCast::CreateSurfaces(std::vector <Wall> *walls) {
    ClearSurfaces();

    std::vector <Wall> walls_on_views = GetWallsOnPlayerView(walls);

    const int number_ray = angleView * 2; // 2 to more detalid
    const int angle_space = 1 / number_ray;
    float angle = player_dir - (angleView / 2);







    for(int i = 0; i < number_ray; ++i) {        

        angle += angle_space;
    }
}

void PolygonCast::SetAngleView(float angle) {
    angleView = angle;
}

void PolygonCast::SetDistanceView(float distance) {
    distanceView = distance;
}

void PolygonCast::CreateView(Player *player, std::vector<Wall> *walls) {
    player_dir = player->GetDirection();
    player_pos = player->GetPosition();
    player_center_pos = sf::Vector2f(player_pos.x + (player->GetSize().x/2), player_pos.y + (player->GetSize().y/2));

    CreateSurfaces(walls);
}

