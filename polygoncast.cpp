#include "polygoncast.h"


PolygonCast::PolygonCast(const int widthhWindow, const int heightWindow) {
    this->widthWindow  = widthhWindow;
    this->heightWindow = heightWindow;
}

std::vector <Surface> &PolygonCast::GetSurfaces() {
    return surfaces;
}

Surface PolygonCast::CalculateSurface(ToolKit::Vector2f_pair line, float distanceA, float distanceB, float height, std::string texturePath) {
    std::vector <sf::Vector2f> playerView;
    playerView.push_back(player_center_pos);
    playerView.push_back(sf::Vector2f(player_center_pos.x + (ToolKit::cosine(fmod((player_dir + (angleView/2)), 360)) * distanceView), player_center_pos.y - ToolKit::sine(fmod((player_dir + (angleView/2)), 360)) * distanceView));
    playerView.push_back(sf::Vector2f(player_center_pos.x + (ToolKit::cosine(fmod((player_dir - (angleView/2)), 360)) * distanceView), player_center_pos.y - ToolKit::sine(fmod((player_dir - (angleView/2)), 360)) * distanceView));

    float viewSpace = distanceView*2;
    float rayExtendSize = distanceView/3;

    sf::Vector2f leftPointView  = sf::Vector2f(playerView[1].x + (ToolKit::cosine(ToolKit::GetAngle(playerView[2], playerView[1]))) * viewSpace, playerView[1].y - (ToolKit::sine(ToolKit::GetAngle(playerView[2], playerView[1]))) * viewSpace);
    sf::Vector2f rightPointView = sf::Vector2f(playerView[2].x + (ToolKit::cosine(ToolKit::GetAngle(playerView[1], playerView[2]))) * viewSpace, playerView[2].y - (ToolKit::sine(ToolKit::GetAngle(playerView[1], playerView[2]))) * viewSpace);

    float angleToLineASize = ToolKit::GetAngle(player_center_pos, line.A);
    float angleToLineBSize = ToolKit::GetAngle(player_center_pos, line.B);

    sf::Vector2f endRayToLineAPoint = sf::Vector2f(player_center_pos.x + ToolKit::cosine(angleToLineASize) * (distanceView + rayExtendSize), player_center_pos.y - ToolKit::sine(angleToLineASize) * (distanceView + rayExtendSize));
    sf::Vector2f endRayToLineBPoint = sf::Vector2f(player_center_pos.x + ToolKit::cosine(angleToLineBSize) * (distanceView + rayExtendSize), player_center_pos.y - ToolKit::sine(angleToLineBSize) * (distanceView + rayExtendSize));

    float distanceFromLeftPointViewA;
    float distanceFromLeftPointViewB;
    //Calc A pos x
    sf::Vector2f positionIntersectionRayToLineA = ToolKit::GetIntersectPosition(player_center_pos, endRayToLineAPoint, leftPointView, rightPointView).position;
    distanceFromLeftPointViewA = ToolKit::GetDistance(leftPointView, positionIntersectionRayToLineA);

    //Calc B pos x
    sf::Vector2f positionIntersectionRayToLineB = ToolKit::GetIntersectPosition(player_center_pos, endRayToLineBPoint, leftPointView, rightPointView).position;
    distanceFromLeftPointViewB = ToolKit::GetDistance(leftPointView, positionIntersectionRayToLineB);

    float posXLineA = ((distanceFromLeftPointViewA / ToolKit::GetDistance(leftPointView, rightPointView)) * (widthWindow + viewSpace * 2)) - viewSpace;
    float posXLineB = ((distanceFromLeftPointViewB / ToolKit::GetDistance(leftPointView, rightPointView)) * (widthWindow + viewSpace * 2)) - viewSpace;


    float heightRatioForLineA = (ToolKit::GetDistance(line.A, positionIntersectionRayToLineA) / ToolKit::GetDistance(player_center_pos, positionIntersectionRayToLineA));
    float posYLineAUp = (heightWindow/2) - heightRatioForLineA * (height/2);
    float posYLineADown = (heightWindow/2) + heightRatioForLineA * (height/2);


    float heightRatioForLineB = (ToolKit::GetDistance(line.B, positionIntersectionRayToLineB) / ToolKit::GetDistance(player_center_pos, positionIntersectionRayToLineB));
    float posYLineBUp = (heightWindow/2) - heightRatioForLineB * (height/2);
    float posYLineBDown = (heightWindow/2) + heightRatioForLineB * (height/2);

    sf::Vector2f pos1 = sf::Vector2f(posXLineA, posYLineAUp);
    sf::Vector2f pos2 = sf::Vector2f(posXLineB, posYLineBUp);
    sf::Vector2f pos3 = sf::Vector2f(posXLineB, posYLineBDown);
    sf::Vector2f pos4 = sf::Vector2f(posXLineA, posYLineADown);

    int shadowValueLineA = 255 * heightRatioForLineA;
    int shadowValueLineB = 255 * heightRatioForLineB;

    Surface surface(pos1, pos2, pos3, pos4, texturePath, sf::Color(0, shadowValueLineA, 0, 255), sf::Color(0, shadowValueLineB, 0, 255));

    return surface;
}


void PolygonCast::ClearSurfaces() {
    surfaces.clear();
}

std::vector <Wall> PolygonCast::GetWallsOnPlayerView(std::vector <Wall> *walls) {
    std::vector <sf::Vector2f> playerView;
    playerView.push_back(player_center_pos);
    playerView.push_back(sf::Vector2f(player_center_pos.x + (ToolKit::cosine(fmod((player_dir + (angleView/2)), 360)) * distanceView), player_center_pos.y - ToolKit::sine(fmod((player_dir + (angleView/2)), 360)) * distanceView));
    playerView.push_back(sf::Vector2f(player_center_pos.x + (ToolKit::cosine(fmod((player_dir - (angleView/2)), 360)) * distanceView), player_center_pos.y - ToolKit::sine(fmod((player_dir - (angleView/2)), 360)) * distanceView));

    sf::Vector2f centerPlayerView;
    centerPlayerView = sf::Vector2f(player_center_pos.x + ToolKit::cosine(player_dir) * (distanceView/2), player_center_pos.y + (-ToolKit::sine(player_dir)) * (distanceView/2));

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

           if(!ToolKit::GetIntersectPosition(playerView[0], playerView[1], centerPlayerView, wallPoint).is_intersection &&
              !ToolKit::GetIntersectPosition(playerView[1], playerView[2], centerPlayerView, wallPoint).is_intersection &&
              !ToolKit::GetIntersectPosition(playerView[2], playerView[0], centerPlayerView, wallPoint).is_intersection) {
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
    std::vector <Wall> wallsOnPlayerView = GetWallsOnPlayerView(walls);
    std::vector <lineSegment> allLinesSegments;
    std::vector <lineSegment> linesSegmentsToDraw;

    // create lines
    for(int i = 0; i < wallsOnPlayerView.size(); i++) {
        sf::Vector2f wallAPos = wallsOnPlayerView[i].GetPointPosition('A');
        sf::Vector2f wallBPos = wallsOnPlayerView[i].GetPointPosition('B');
        sf::Vector2f wallCPos = wallsOnPlayerView[i].GetPointPosition('C');
        sf::Vector2f wallDPos = wallsOnPlayerView[i].GetPointPosition('D');
        sf::Vector2f wallSize = wallsOnPlayerView[i].GetSize();
        std::string texturePath = wallsOnPlayerView[i].GetTexturePath();

        std::vector <sf::Vector2f> wallPoints = {wallAPos, wallBPos, wallCPos, wallDPos};

        for(int j = 0; j < 4; ++j) {
            lineSegment line(wallPoints[j], wallPoints[(j+1)%4], ToolKit::GetDistance(wallPoints[j], player_center_pos), ToolKit::GetDistance(wallPoints[(j+1)%4], player_center_pos), wallSize.y, texturePath);
            allLinesSegments.push_back(line);
        }
    }

    int number_ray = angleView * 2;
    double angle_space = angleView / float(number_ray);
    double angle = player_dir - (angleView / 2);

    //set lines seen by player
    for(int i = 0; i < number_ray; ++i) {
        if(angle > 360) angle = fmod(angle, 360);
        else if(angle < 0) angle = 360 - fmod(fabs(angle), 360);

        sf::Vector2f endRay = sf::Vector2f(player_center_pos.x + ToolKit::cosine(angle) * distanceView, player_center_pos.y - ToolKit::sine(angle) * distanceView);

        lineSegment lineToDraw;
        bool lineInit = false;
        float nearestDistanceToIntersect = distanceView;

        for(int j = 0; j < allLinesSegments.size(); ++j) {
            sf::Vector2f linePosA = allLinesSegments[j].line.A;
            sf::Vector2f linePosB = allLinesSegments[j].line.B;

            if(ToolKit::GetIntersectPosition(player_center_pos, endRay, linePosA, linePosB).is_intersection) {
                sf::Vector2f intersectPos = ToolKit::GetIntersectPosition(player_center_pos, endRay, linePosA, linePosB).position;
                float x = fabs(player_center_pos.x - intersectPos.x);
                float y = fabs(player_center_pos.y - intersectPos.y);
                float distanceToIntersect = sqrt(x*x + y*y);

                if(nearestDistanceToIntersect > distanceToIntersect) {
                   nearestDistanceToIntersect = distanceToIntersect;
                   lineToDraw = allLinesSegments[j];
                   lineInit = true;
                }
            }
        }

        if(lineInit) {
            bool linesSame = false;
            for(int j = 0; j < linesSegmentsToDraw.size(); ++j) {
               if(lineToDraw == linesSegmentsToDraw[j]) {
                   linesSame = true;
                   break;
               }
            }

            if(!linesSame) linesSegmentsToDraw.push_back(lineToDraw);
        }

        angle += angle_space;
    }

    //set surfaces from lines
    std::cout << linesSegmentsToDraw.size() << std::endl;

    ClearSurfaces();

    for(int i = 0; i < linesSegmentsToDraw.size(); ++i) {
        ToolKit::Vector2f_pair line = linesSegmentsToDraw[i].line;
        float distanceToA = linesSegmentsToDraw[i].distance_A;
        float distanceToB = linesSegmentsToDraw[i].distance_B;
        float height = linesSegmentsToDraw[i].height;
        std::string texturePath = linesSegmentsToDraw[i].texturePath;

        surfaces.push_back(CalculateSurface(line, distanceToA, distanceToB, height, texturePath));
    }

    //sort surfaces from the fareast to the nearest

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

