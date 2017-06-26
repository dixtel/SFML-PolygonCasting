#include "collision.h"

#define PI 3.14159265359

typedef struct {
    bool is_intersection;
    sf::Vector2f position; // position intersect
    sf::Vector2f A1; //positions of line intersection;
    sf::Vector2f A2;
    sf::Vector2f B1; //positions of line to intersection;
    sf::Vector2f B2;
} intersect_propeties;

float GetAngle(sf::Vector2f start, sf::Vector2f end) {
    sf::Vector2f direction = start - end;

    float angleRad = atan2(direction.x, direction.y);
    float angleDeg = (angleRad / 3.14159265359) * 180.0;

    if(angleDeg < 0) angleDeg = 360 + angleDeg;
    angleDeg += 90;
    if(angleDeg >= 360) angleDeg = angleDeg - 360;

    return angleDeg;
}

float cosine(float degree) {
    if(degree == 90.0) {
        return 0;
    }
    else if(degree == 270.0) {
        return 0;
    }

    return std::cos(degree * PI / 180);
}

float sine(float degree) {
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

intersect_propeties GetIntersectPosition(sf::Vector2f A1, sf::Vector2f A2, sf::Vector2f B1, sf::Vector2f B2) {
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

sf::Vector2f FixPlayerPosition(sf::Vector2f old_pos, sf::Vector2f invalid_pos, sf::Vector2f start_pos_obstacle, sf::Vector2f end_pos_obstacle, float radius) {
    sf::Vector2f intersect_position = GetIntersectPosition(old_pos, invalid_pos, start_pos_obstacle, end_pos_obstacle).position;

    float distance;

    if(intersect_position.x == invalid_pos.x) {
        distance = fabs(intersect_position.y - invalid_pos.y);
    }
    else if(intersect_position.y == invalid_pos.y) {
        distance = fabs(intersect_position.x - invalid_pos.x);
    }
    else {
        float x = fabs(intersect_position.x - (invalid_pos.x));
        float y = fabs(intersect_position.y - (invalid_pos.y));
        distance = sqrt(pow(x, 2) + pow(y, 2));
    }


    float angleA1 = GetAngle(end_pos_obstacle, start_pos_obstacle);
    float angleA2 = GetAngle(start_pos_obstacle, end_pos_obstacle);

    if(angleA1 > angleA2) {
        float tmp = angleA2;
        angleA2 = angleA1;
        angleA1 = tmp;
    }

    float start_vector_angle = GetAngle(invalid_pos, old_pos);
    float angle_A1_start;
    float angle_A2_start;

    if((start_vector_angle > angleA1) && (start_vector_angle < angleA2)) {
        angle_A1_start = start_vector_angle - angleA1;
        angle_A2_start = 180 - angle_A1_start;
    }
    else {
        if(start_vector_angle < angleA1) {
            angle_A1_start = angleA1 - start_vector_angle;
            angle_A2_start = 180 - angle_A1_start;
        }
        else {
            angle_A2_start = start_vector_angle - angleA2;
            angle_A1_start = 180 - angle_A2_start;
        }
    }

    sf::Vector2f new_player_position;
    float space = 0.2;
    if(angle_A1_start > angle_A2_start) {
        double new_direction = angleA1;
        sf::Vector2f vector = sf::Vector2f(cosine(new_direction), -sine(new_direction));

        float reverse_angle = GetAngle(invalid_pos, old_pos);
        float x = cosine(reverse_angle);
        float y = -sine(reverse_angle);

        intersect_position -= sf::Vector2f(radius, radius);
        intersect_position += sf::Vector2f(x * space, y * space);

        new_player_position = intersect_position + (vector * distance);
    }
    else if(angle_A1_start < angle_A2_start) {
        double new_direction = angleA2;
        sf::Vector2f vector = sf::Vector2f(cosine(new_direction), -sine(new_direction));

        float reverse_angle = GetAngle(invalid_pos, old_pos);
        float x = cosine(reverse_angle);
        float y = -sine(reverse_angle);

        intersect_position -= sf::Vector2f(radius, radius);
        intersect_position += sf::Vector2f(x * space, y * space);

        new_player_position = intersect_position + (vector * distance);
    }
    else {
        double new_direction = GetAngle(intersect_position, old_pos);
        sf::Vector2f vector = sf::Vector2f(cosine(new_direction), -sine(new_direction));

        if(new_direction == 90 || new_direction == 270) vector.y = 0;
        else if(new_direction == 0 || new_direction == 360 || new_direction == 180) vector.x = 0;

        float reverse_angle = GetAngle(invalid_pos, old_pos);
        float space_x = cosine(reverse_angle);
        float space_y = -sine(reverse_angle);

        intersect_position -= sf::Vector2f(radius, radius);
        intersect_position += sf::Vector2f(space_x * space, space_y * space);

        new_player_position = intersect_position + (vector * distance);
    }

    return new_player_position;
}

std::vector <sf::Vector2f> GetPointsOfCorrner(sf::Vector2f corrner_pos, char corrner, float radius) {
    std::vector <sf::Vector2f> points;
    sf::CircleShape circle(radius);
    circle.setPointCount(40);
    //  0  - 10  = B,
    //  10 - 20 = D,
    //  20 - 30 = C,
    //  30 - 40 = A

    circle.setPosition(corrner_pos);

    if(corrner == 'A') {
        for(int i = 30; i <= 40; ++i) {
            points.push_back(circle.getPoint(i));
        }
    }
    else if(corrner == 'B') {
        for(int i = 0; i <= 10; ++i) {
            points.push_back(circle.getPoint(i));
        }
    }
    else if(corrner == 'C') {
        for(int i = 20; i <= 30; ++i) {
            points.push_back(circle.getPoint(i));
        }
    }
    else if(corrner == 'D') {
        for(int i = 10; i <= 20; ++i) {
            points.push_back(circle.getPoint(i));
        }
    }

    corrner_pos -= sf::Vector2f(radius, radius);

    for(int i = 0; i < points.size(); i++) {
        points[i] += corrner_pos;
    }

    return points;
}

Collision::Collision() {

}

void Collision::SetCollision(Player *player, std::vector<Wall*> walls) {
    static sf::Vector2f old_pos_player = player->GetPosition();
    sf::Vector2f current_pos_player = player->GetPosition();

    float radius = player->radius;

    for(int i = 0; i < walls.size(); ++i) {
        sf::Vector2f wall_pos = walls[i]->wall.getPosition();
        sf::Vector2f wall_size = walls[i]->wall.getSize();

        /*      *----coll1---*
         *    *                *
         *  *                    *
         *  |    A----------B    |
         *  |    |          |    |
         *  |    |          |    |
         * coll4 |          |-r--coll2
         *  |    |          |    |
         *  |    |          |    |
         *  |    C----------D    |
         *  *                    *
         *    *                *
         *      *---coll3----*
         */

        sf::Vector2f A = walls[i]->GetPointPosition('A');
        sf::Vector2f B = walls[i]->GetPointPosition('B');
        sf::Vector2f C = walls[i]->GetPointPosition('C');
        sf::Vector2f D = walls[i]->GetPointPosition('D');


        //collision 1

        if(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), A + sf::Vector2f(0, -radius), B + sf::Vector2f(0, -radius)).is_intersection) {
            std::cout << "Collision 1!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

            player->position = FixPlayerPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), A + sf::Vector2f(0, -radius), B + sf::Vector2f(0, -radius), radius);
            current_pos_player = player->position;
        }
        if(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius),B + sf::Vector2f(radius, 0), D + sf::Vector2f(radius, 0)).is_intersection) {
            std::cout << "Collision 2!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

            player->position = FixPlayerPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), B + sf::Vector2f(radius, 0), D + sf::Vector2f(radius, 0), radius);
            current_pos_player = player->position;
        }
        if(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), D + sf::Vector2f(0, radius), C + sf::Vector2f(0, radius)).is_intersection) {
            std::cout << "Collision 3!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

            player->position = FixPlayerPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), C + sf::Vector2f(0, radius), D + sf::Vector2f(0, radius), radius);
            current_pos_player = player->position;
        }
        if(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), A + sf::Vector2f(-radius, 0), C + sf::Vector2f(-radius, 0)).is_intersection) {
            std::cout << "Collision 4!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

            player->position = FixPlayerPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), A + sf::Vector2f(-radius, 0), C + sf::Vector2f(-radius, 0), radius);
            current_pos_player = player->position;
        }


//        std::vector <sf::Vector2f> corrner_A = GetPointsOfCorrner(A, 'A', radius);
//        std::vector <sf::Vector2f> corrner_B = GetPointsOfCorrner(B, 'B', radius);
//        std::vector <sf::Vector2f> corrner_C = GetPointsOfCorrner(C, 'C', radius);
//        std::vector <sf::Vector2f> corrner_D = GetPointsOfCorrner(D, 'D', radius);

//        for(int i = 0; i < 4; i++) {
//            std::vector <intersect_propeties> intersection_propeties;

//            if(i == 0) {
//                for(int j = 0; j < corrner_A.size()-1; ++j) {
//                    if(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_A[j], corrner_A[j+1]).is_intersection) {
//                        std::cout << "collision corrner A" << std::endl;

//                        intersection_propeties.push_back(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_A[j], corrner_A[j+1]));
//                    }
//                }
//            }
//            if(i == 1) {
//                for(int j = 0; j < corrner_B.size()-1; ++j) {
//                    if(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_B[j], corrner_B[j+1]).is_intersection) {
//                        std::cout << "collision corrner B" << std::endl;

//                        intersection_propeties.push_back(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_B[j], corrner_B[j+1]));
//                    }
//                }
//            }
//            if(i == 2) {
//                for(int j = 0; j < corrner_C.size()-1; ++j) {
//                    if(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_C[j], corrner_C[j+1]).is_intersection) {
//                        std::cout << "collision corrner C" << std::endl;

//                        intersection_propeties.push_back(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_C[j], corrner_C[j+1]));
//                    }
//                }
//            }
//            if(i == 3) {
//                for(int j = 0; j < corrner_D.size()-1; ++j) {
//                    if(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_D[j], corrner_D[j+1]).is_intersection) {
//                        std::cout << "collision corrner D" << std::endl;

//                        intersection_propeties.push_back(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_D[j], corrner_D[j+1]));
//                    }
//                }
//            }

//            if(intersection_propeties.size() == 1) {
//                // found position of intersection for corner
//                sf::Vector2f B1 = intersection_propeties[0].B1;
//                sf::Vector2f B2 = intersection_propeties[0].B2;

//                player->position = FixPlayerPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), B1, B2, radius);
//                current_pos_player = player->position;
//                break;
//            }
//            else if(intersection_propeties.size() > 1) {
//                std::cout << "tessssssssssssssssssssssssssssssssssssssssssssssssssssssssssss" << std::endl;
//                // find the nearest position of intersect
//                intersect_propeties nearest_intersection = intersection_propeties[0];

//                for (int j = 1; j < intersection_propeties.size(); ++j) {
//                    sf::Vector2f distance_this_case = old_pos_player - intersection_propeties[i].position;
//                    distance_this_case.x = fabs(distance_this_case.x);
//                    distance_this_case.y = fabs(distance_this_case.y);

//                    sf::Vector2f distance_nearest = old_pos_player - nearest_intersection.position;
//                    distance_nearest.x = fabs(distance_nearest.x);
//                    distance_nearest.y = fabs(distance_nearest.y);

//                    float hypotenuse_this_case = sqrt(pow(distance_this_case.x, 2) + pow(distance_this_case.y, 2));
//                    float hypotenuse_nearest = sqrt(pow(distance_nearest.x, 2) + pow(distance_nearest.y, 2));

//                    if(hypotenuse_this_case > hypotenuse_nearest) nearest_intersection = intersection_propeties[j];

//                }

//                // intersection is "nearest_intersection"`
//                sf::Vector2f B1 = nearest_intersection.B1;
//                sf::Vector2f B2 = nearest_intersection.B2;

//                player->position = FixPlayerPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), B1, B2, radius);
//                current_pos_player = player->position;
//                break;
//            }

//        }

    }

    for(int i = 0; i < walls.size(); ++i) {
        sf::Vector2f A = walls[i]->GetPointPosition('A');
        sf::Vector2f B = walls[i]->GetPointPosition('B');
        sf::Vector2f C = walls[i]->GetPointPosition('C');
        sf::Vector2f D = walls[i]->GetPointPosition('D');

        std::vector <sf::Vector2f> corrner_A = GetPointsOfCorrner(A, 'A', radius);
        std::vector <sf::Vector2f> corrner_B = GetPointsOfCorrner(B, 'B', radius);
        std::vector <sf::Vector2f> corrner_C = GetPointsOfCorrner(C, 'C', radius);
        std::vector <sf::Vector2f> corrner_D = GetPointsOfCorrner(D, 'D', radius);

        for(int i = 0; i < 4; i++) {
            std::vector <intersect_propeties> intersection_propeties;

            if(i == 0) {
                for(int j = 0; j < corrner_A.size()-1; ++j) {
                    if(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_A[j], corrner_A[j+1]).is_intersection) {
                        std::cout << "collision corrner A" << std::endl;

                        intersection_propeties.push_back(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_A[j], corrner_A[j+1]));
                    }
                }
            }
            if(i == 1) {
                for(int j = 0; j < corrner_B.size()-1; ++j) {
                    if(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_B[j], corrner_B[j+1]).is_intersection) {
                        std::cout << "collision corrner B" << std::endl;

                        intersection_propeties.push_back(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_B[j], corrner_B[j+1]));
                    }
                }
            }
            if(i == 2) {
                for(int j = 0; j < corrner_C.size()-1; ++j) {
                    if(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_C[j], corrner_C[j+1]).is_intersection) {
                        std::cout << "collision corrner C" << std::endl;

                        intersection_propeties.push_back(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_C[j], corrner_C[j+1]));
                    }
                }
            }
            if(i == 3) {
                for(int j = 0; j < corrner_D.size()-1; ++j) {
                    if(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_D[j], corrner_D[j+1]).is_intersection) {
                        std::cout << "collision corrner D" << std::endl;

                        intersection_propeties.push_back(GetIntersectPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), corrner_D[j], corrner_D[j+1]));
                    }
                }
            }

            if(intersection_propeties.size() == 1) {
                // found position of intersection for corner
                sf::Vector2f B1 = intersection_propeties[0].B1;
                sf::Vector2f B2 = intersection_propeties[0].B2;

                player->position = FixPlayerPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), B1, B2, radius);
                current_pos_player = player->position;
                break;
            }
            else if(intersection_propeties.size() > 1) {
                std::cout << "tessssssssssssssssssssssssssssssssssssssssssssssssssssssssssss" << std::endl;
                // find the nearest position of intersect
                intersect_propeties nearest_intersection = intersection_propeties[0];

                for (int j = 1; j < intersection_propeties.size(); ++j) {
                    sf::Vector2f distance_this_case = old_pos_player - intersection_propeties[i].position;
                    distance_this_case.x = fabs(distance_this_case.x);
                    distance_this_case.y = fabs(distance_this_case.y);

                    sf::Vector2f distance_nearest = old_pos_player - nearest_intersection.position;
                    distance_nearest.x = fabs(distance_nearest.x);
                    distance_nearest.y = fabs(distance_nearest.y);

                    float hypotenuse_this_case = sqrt(pow(distance_this_case.x, 2) + pow(distance_this_case.y, 2));
                    float hypotenuse_nearest = sqrt(pow(distance_nearest.x, 2) + pow(distance_nearest.y, 2));

                    if(hypotenuse_this_case > hypotenuse_nearest) nearest_intersection = intersection_propeties[j];

                }

                // intersection is "nearest_intersection"`
                sf::Vector2f B1 = nearest_intersection.B1;
                sf::Vector2f B2 = nearest_intersection.B2;

                player->position = FixPlayerPosition(old_pos_player + sf::Vector2f(radius, radius), current_pos_player + sf::Vector2f(radius, radius), B1, B2, radius);
                current_pos_player = player->position;
                break;
            }

        }
    }
    //std::cout << old_pos_player.x << " " << old_pos_player.y << " - " << current_pos_player.x << " " << current_pos_player.y << std::endl;

    old_pos_player = current_pos_player;
}



