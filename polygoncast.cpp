#include "polygoncast.h"

void ChangePosition(std::vector <lineSegment> *object, unsigned were, unsigned int what) {
    if((what > object->size() - 1) || (were > object->size() - 1)) {
        std::cout << "error: function ChangePosition: valid variables (PolygonCast)" << std::endl;
        return;
    }
    \
    object->insert(object->begin() + were, *(object->begin() + what));
    object->erase(object->begin() + what + 1);

}

PolygonCast::PolygonCast(const int widthhWindow, const int heightWindow) {
    this->widthWindow  = widthhWindow;
    this->heightWindow = heightWindow;
}

std::vector <Surface> &PolygonCast::GetSurfaces() {
    return surfaces;
}

Surface PolygonCast::CalculateSurface(Toolkit::Vector2f_pair line, float distanceA, float distanceB, float height, std::string texturePath) {
    //height = float(widthWindow) / float(heightWindow) * height;

    std::vector <sf::Vector2f> viewTriangle;
    viewTriangle.push_back(cameraPosition);
    viewTriangle.push_back(sf::Vector2f(cameraPosition.x + (Toolkit::Cosine(fmod((directionView + (angleView/2)), 360)) * distanceView), cameraPosition.y - Toolkit::Sine(fmod((directionView + (angleView/2)), 360)) * distanceView));
    viewTriangle.push_back(sf::Vector2f(cameraPosition.x + (Toolkit::Cosine(fmod((directionView - (angleView/2)), 360)) * distanceView), cameraPosition.y - Toolkit::Sine(fmod((directionView - (angleView/2)), 360)) * distanceView));


    float viewSpace = distanceView*10;
    float rayExtendSize = distanceView/3;

    sf::Vector2f leftPointView  = sf::Vector2f(viewTriangle[1].x + (Toolkit::Cosine(Toolkit::GetAngle(viewTriangle[2], viewTriangle[1]))) * viewSpace, viewTriangle[1].y - (Toolkit::Sine(Toolkit::GetAngle(viewTriangle[2], viewTriangle[1]))) * viewSpace);
    sf::Vector2f rightPointView = sf::Vector2f(viewTriangle[2].x + (Toolkit::Cosine(Toolkit::GetAngle(viewTriangle[1], viewTriangle[2]))) * viewSpace, viewTriangle[2].y - (Toolkit::Sine(Toolkit::GetAngle(viewTriangle[1], viewTriangle[2]))) * viewSpace);

    float angleToLineASize = Toolkit::GetAngle(cameraPosition, line.A);
    float angleToLineBSize = Toolkit::GetAngle(cameraPosition, line.B);

    sf::Vector2f endRayToLineAPoint = sf::Vector2f(cameraPosition.x + Toolkit::Cosine(angleToLineASize) * (distanceView + rayExtendSize), cameraPosition.y - Toolkit::Sine(angleToLineASize) * (distanceView + rayExtendSize));
    sf::Vector2f endRayToLineBPoint = sf::Vector2f(cameraPosition.x + Toolkit::Cosine(angleToLineBSize) * (distanceView + rayExtendSize), cameraPosition.y - Toolkit::Sine(angleToLineBSize) * (distanceView + rayExtendSize));

    float distanceFromLeftPointViewA;
    float distanceFromLeftPointViewB;
    //Calc A pos x
    sf::Vector2f positionIntersectionRayToLineA = Toolkit::GetIntersection(cameraPosition, endRayToLineAPoint, leftPointView, rightPointView).position;
    distanceFromLeftPointViewA = Toolkit::GetDistance(leftPointView, positionIntersectionRayToLineA);

    //Calc B pos x
    sf::Vector2f positionIntersectionRayToLineB = Toolkit::GetIntersection(cameraPosition, endRayToLineBPoint, leftPointView, rightPointView).position;
    distanceFromLeftPointViewB = Toolkit::GetDistance(leftPointView, positionIntersectionRayToLineB);

    float posXLineA = ((distanceFromLeftPointViewA / Toolkit::GetDistance(leftPointView, rightPointView)) * (widthWindow + viewSpace * 2)) - viewSpace;
    float posXLineB = ((distanceFromLeftPointViewB / Toolkit::GetDistance(leftPointView, rightPointView)) * (widthWindow + viewSpace * 2)) - viewSpace;


    float heightRatioForLineA = (Toolkit::GetDistance(line.A, positionIntersectionRayToLineA) / Toolkit::GetDistance(cameraPosition, positionIntersectionRayToLineA));
    float posYLineAUp = (heightWindow/2) - heightRatioForLineA * (height/2);
    float posYLineADown = (heightWindow/2) + heightRatioForLineA * (height/2);


    float heightRatioForLineB = (Toolkit::GetDistance(line.B, positionIntersectionRayToLineB) / Toolkit::GetDistance(cameraPosition, positionIntersectionRayToLineB));
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

void PolygonCast::SortLineSegments(std::vector <lineSegment> *lines) {
    const float linesDivision = 10;

    for(int i = 0; i < lines->size(); ++i) {
        sf::Vector2f A = lines->at(i).line.A;
        sf::Vector2f B = lines->at(i).line.B;

        std::vector <sf::Vector2f> linesPointsPos;
        std::vector <sf::Vector2f> maxViewsPos;

        float widthLine = Toolkit::GetDistance(A.x , B.x);
        float heightLine = Toolkit::GetDistance(A.y , B.y);

        float spaceX = widthLine / linesDivision;
        float spaceY = heightLine / linesDivision;

        //set linesPointsPos
        if(spaceX) {
            //spaceX
            if(A.x > B.x) {
                for(int j = 0; j < linesDivision + 1; ++j) {
                    float x = A.x - (j * spaceX);
                    float y = 0;

                    if(A.y > B.y) {
                        y = A.y - (Toolkit::GetDistance(A.y, B.y) * (j * spaceX / Toolkit::GetDistance(A.y, B.y)));
                    }
                    else {
                        y = A.y + (Toolkit::GetDistance(A.y, B.y) * (j * spaceX / Toolkit::GetDistance(A.y, B.y)));
                    }

                    linesPointsPos.push_back(sf::Vector2f(x, y));
                }
            }
            else {
                for(int j = 0; j < linesDivision + 1; ++j) {
                    float x = A.x + (j * spaceX);
                    float y = 0;

                    if(A.y > B.y) {
                        y = A.y - (Toolkit::GetDistance(A.y, B.y) * (j * spaceX / Toolkit::GetDistance(A.y, B.y)));
                    }
                    else {
                        y = A.y + (Toolkit::GetDistance(A.y, B.y) * (j * spaceX / Toolkit::GetDistance(A.y, B.y)));
                    }

                    linesPointsPos.push_back(sf::Vector2f(x, y));
                }
            }
        }
        else {
            //spaceY
            for(int j = 0; j < linesDivision + 1; ++j) {
                float x = A.x;
                float y = 0;

                if(A.y > B.y) {
                    y = A.y - (Toolkit::GetDistance(A.y, B.y) * (j * spaceY / Toolkit::GetDistance(A.y, B.y)));
                }
                else {
                    y = A.y + (Toolkit::GetDistance(A.y, B.y) * (j * spaceY / Toolkit::GetDistance(A.y, B.y)));
                }

                linesPointsPos.push_back(sf::Vector2f(x, y));
            }
        }

        //set maxVievsPos
        for(int j = 0; j < linesPointsPos.size(); ++j) {
            float angle = Toolkit::GetAngle(cameraPosition, linesPointsPos[j]);
            float cos = Toolkit::Cosine(angle);
            float sin = -Toolkit::Sine(angle);
            float distanceToMaxViev = distanceView - (Toolkit::GetDistance(cameraPosition, linesPointsPos[j]));
            sf::Vector2f endView = sf::Vector2f(linesPointsPos[j].x + (cos * distanceView), linesPointsPos[j].y + (sin * distanceView));

            maxViewsPos.push_back(endView);
        }


        for(int j = 0; j < lines->size(); ++j) {
            if(j == i) continue;

            sf::Vector2f intersectToA = lines->at(j).line.A;
            sf::Vector2f intersectToB = lines->at(j).line.B;

            for(int k = 0; k < maxViewsPos.size(); ++k) {
                float distanceToLinePoint = Toolkit::GetDistance(cameraPosition, linesPointsPos[k]);
                if(Toolkit::GetIntersection(cameraPosition, maxViewsPos[k], intersectToA, intersectToB).is_intersection) {
                    float distance = Toolkit::GetDistance(cameraPosition, Toolkit::GetIntersection(cameraPosition, maxViewsPos[k], intersectToA, intersectToB).position);

                    if(distance > distanceToLinePoint) {
                        if(j > i) {
                            ChangePosition(lines, i, j);
                        }
                    }
                }
            }
        }
    }
}

void PolygonCast::ClearSurfaces() {
    surfaces.clear();
}

std::vector <Wall> PolygonCast::GetWallsOnPlayerView(std::vector <Wall> *walls) {
    // A-----B
    // |     |
    // D--C--C
    // C - cameraPosition

    const float widthViewRectangle = distanceView + distanceView / 3;
    const float heightViewRectangle = distanceView;

    float leftUpPositionY = (cameraPosition.y + ((-Toolkit::Sine(fmod(directionView + 90, 360)) * (widthViewRectangle/2))) + (-Toolkit::Sine(directionView) * (heightViewRectangle)));
    float leftUpPositionX = cameraPosition.x + ((Toolkit::Cosine(fmod(directionView + 90, 360)) * (widthViewRectangle/2)) + (Toolkit::Cosine(directionView) * (heightViewRectangle)));
    float rightUpPositionY = (cameraPosition.y + ((-Toolkit::Sine(fmod(directionView - 90, 360)) * (widthViewRectangle/2))) + (-Toolkit::Sine(directionView) * (heightViewRectangle)));
    float rightUpPositionX = cameraPosition.x + ((Toolkit::Cosine(fmod(directionView - 90, 360)) * (widthViewRectangle/2)) + (Toolkit::Cosine(directionView) * (heightViewRectangle)));

    sf::Vector2f viewRectangleAPoint = sf::Vector2f(leftUpPositionX, leftUpPositionY);
    sf::Vector2f viewRectangleBPoint = sf::Vector2f(rightUpPositionX, rightUpPositionY);
    sf::Vector2f viewRectangleCPoint = sf::Vector2f(cameraPosition.x + (Toolkit::Cosine(fmod(directionView - 90, 360)) * (widthViewRectangle/2)), cameraPosition.y + (-Toolkit::Sine(fmod(directionView - 90, 360)) * (widthViewRectangle/2)));
    sf::Vector2f viewRectangleDPoint = sf::Vector2f(cameraPosition.x + (Toolkit::Cosine(fmod(directionView + 90, 360)) * (widthViewRectangle/2)), cameraPosition.y + (-Toolkit::Sine(fmod(directionView + 90, 360)) * (widthViewRectangle/2)));
    sf::Vector2f centerViewRectangle = sf::Vector2f(cameraPosition.x, cameraPosition.y + (-Toolkit::Sine(directionView) * (distanceView/2)));

    std::vector <Wall> wallsOnViews;

    for(int i = 0; i < walls->size(); ++i) {
        std::vector <sf::Vector2f> wallPointsPosistions;
        wallPointsPosistions.push_back(walls->at(i).GetPointPosition('A'));
        wallPointsPosistions.push_back(walls->at(i).GetPointPosition('B'));
        wallPointsPosistions.push_back(walls->at(i).GetPointPosition('C'));
        wallPointsPosistions.push_back(walls->at(i).GetPointPosition('D'));

        bool wallPointIntersection = true;

        for(int j = 0; j < 4; ++j) {
           sf::Vector2f wallPointPosition = wallPointsPosistions[j];

           if(!Toolkit::GetIntersection(viewRectangleAPoint, viewRectangleBPoint, centerViewRectangle, wallPointPosition).is_intersection &&
              !Toolkit::GetIntersection(viewRectangleBPoint, viewRectangleCPoint, centerViewRectangle, wallPointPosition).is_intersection &&
              !Toolkit::GetIntersection(viewRectangleCPoint, viewRectangleDPoint, centerViewRectangle, wallPointPosition).is_intersection &&
              !Toolkit::GetIntersection(viewRectangleDPoint, viewRectangleAPoint, centerViewRectangle, wallPointPosition).is_intersection) {
               wallPointIntersection = false;
               break;
           }
        }

        if(!wallPointIntersection) {
            wallsOnViews.push_back(walls->at(i));
        }
    }

    return wallsOnViews;
}

void PolygonCast::CreateSurfaces(std::vector <Wall> *walls) {
    std::vector <Wall> wallsOnPlayerView = GetWallsOnPlayerView(walls);
    std::vector <lineSegment> allLinesSegments;
    std::vector <lineSegment> linesSegmentsToDraw;

    if(wallsOnPlayerView.size() == 0) return;

    // create lines
    for(int i = 0; i < wallsOnPlayerView.size(); i++) {
        sf::Vector2f wallAPos = wallsOnPlayerView[i].GetPointPosition('A');
        sf::Vector2f wallBPos = wallsOnPlayerView[i].GetPointPosition('B');
        sf::Vector2f wallCPos = wallsOnPlayerView[i].GetPointPosition('C');
        sf::Vector2f wallDPos = wallsOnPlayerView[i].GetPointPosition('D');
        sf::Vector2f wallSize = wallsOnPlayerView[i].GetSize();
        float wallHeight = wallsOnPlayerView[i].GetHeight();
        std::string texturePath = wallsOnPlayerView[i].GetTexturePath();

        std::vector <sf::Vector2f> wallPoints = {wallAPos, wallBPos, wallCPos, wallDPos};

        for(int j = 0; j < 4; ++j) {
            lineSegment line(wallPoints[j], wallPoints[(j+1)%4], Toolkit::GetDistance(wallPoints[j], cameraPosition), Toolkit::GetDistance(wallPoints[(j+1)%4], cameraPosition), wallHeight, texturePath);
            allLinesSegments.push_back(line);
        }
    }

    int number_ray = angleView * 2;
    double angle_space = angleView / float(number_ray);
    double angle = directionView - (angleView / 2);

    //set lines seen by player
    for(int i = 0; i < number_ray; ++i) {
        if(angle > 360) angle = fmod(angle, 360);
        else if(angle < 0) angle = 360 - fmod(fabs(angle), 360);

        sf::Vector2f endRay = sf::Vector2f(cameraPosition.x + Toolkit::Cosine(angle) * distanceView, cameraPosition.y - Toolkit::Sine(angle) * distanceView);

        lineSegment lineToDraw;
        bool lineInit = false;
        float nearestDistanceToIntersect = distanceView;

        for(int j = 0; j < allLinesSegments.size(); ++j) {
            sf::Vector2f linePosA = allLinesSegments[j].line.A;
            sf::Vector2f linePosB = allLinesSegments[j].line.B;

            if(Toolkit::GetIntersection(cameraPosition, endRay, linePosA, linePosB).is_intersection) {
                sf::Vector2f intersectPos = Toolkit::GetIntersection(cameraPosition, endRay, linePosA, linePosB).position;
                float x = fabs(cameraPosition.x - intersectPos.x);
                float y = fabs(cameraPosition.y - intersectPos.y);
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


    //sort surfaces from the fareast to the nearest

    SortLineSegments(&linesSegmentsToDraw);

    //set surfaces from lines
    std::cout << linesSegmentsToDraw.size() << std::endl;

    ClearSurfaces();

    for(int i = 0; i < linesSegmentsToDraw.size(); ++i) {
        Toolkit::Vector2f_pair line = linesSegmentsToDraw[i].line;
        float distanceToA = linesSegmentsToDraw[i].distance_A;
        float distanceToB = linesSegmentsToDraw[i].distance_B;
        float height = linesSegmentsToDraw[i].height;
        std::string texturePath = linesSegmentsToDraw[i].texturePath;

        surfaces.push_back(CalculateSurface(line, distanceToA, distanceToB, height, texturePath));
    }
}

void PolygonCast::CreateView(Player *player, std::vector<Wall> *walls) {
    angleView      = player->GetAngleView();
    distanceView   = player->GetDistanceView();
    directionView  = player->GetDirection();

    cameraPosition = sf::Vector2f((player->GetPosition().x + (player->GetSize().x/2)) + (Toolkit::Cosine(fmod(directionView + 180 ,360)) * (player->GetSize().y/2)), player->GetPosition().y + (player->GetSize().y/2) + (Toolkit::Cosine(fmod(directionView + 180 ,360)) * (player->GetSize().y/2)));

    CreateSurfaces(walls);
}

