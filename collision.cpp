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

sf::Vector2f GetPlayerCollisionPosition(sf::Vector2f old_pos, sf::Vector2f invalid_pos, sf::Vector2f start_pos_obstacle, sf::Vector2f end_pos_obstacle) {
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

        intersect_position += sf::Vector2f(x * space, y * space);

        new_player_position = intersect_position + (vector * distance);
    }
    else if(angle_A1_start < angle_A2_start) {
        double new_direction = angleA2;
        sf::Vector2f vector = sf::Vector2f(cosine(new_direction), -sine(new_direction));

        float reverse_angle = GetAngle(invalid_pos, old_pos);
        float x = cosine(reverse_angle);
        float y = -sine(reverse_angle);

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

        intersect_position += sf::Vector2f(space_x * space, space_y * space);

        new_player_position = intersect_position + (vector * distance);
    }

    return new_player_position;
}


Collision::Collision() {

}

void Collision::SetCollision(Player *player, std::vector<Wall*> walls) {
    static sf::Vector2f old_pos_A = player->GetPointPosition('A');
    static sf::Vector2f old_pos_B = player->GetPointPosition('B');
    static sf::Vector2f old_pos_C = player->GetPointPosition('C');
    static sf::Vector2f old_pos_D = player->GetPointPosition('D');
    sf::Vector2f current_pos_A = player->GetPointPosition('A');
    sf::Vector2f current_pos_B = player->GetPointPosition('B');
    sf::Vector2f current_pos_C = player->GetPointPosition('C');
    sf::Vector2f current_pos_D = player->GetPointPosition('D');

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

        std::array<sf::Vector2f, 8> linesWall{ C, A,
                                               A, B,
                                               B, D,
                                               D, C };

        // A
        for(int j = 0; j < 8; j+=2) {
            if(GetIntersectPosition(old_pos_A, current_pos_A, linesWall.at(j), linesWall.at(j + 1)).is_intersection) {
                std::cout << "Collision A" << std::endl;

                sf::Vector2f new_position = GetPlayerCollisionPosition(old_pos_A, current_pos_A, linesWall.at(j), linesWall.at(j + 1));
                player->SetPosition(new_position);

                current_pos_A = player->GetPointPosition('A');
                current_pos_B = player->GetPointPosition('B');
                current_pos_C = player->GetPointPosition('C');
                current_pos_D = player->GetPointPosition('D');
            }
        }


        // B
        for(int j = 0; j < 8; j+=2) {
            if(GetIntersectPosition(old_pos_B, current_pos_B, linesWall.at(j), linesWall.at(j + 1)).is_intersection) {
                std::cout << "Collision B" << std::endl;

                sf::Vector2f new_position = GetPlayerCollisionPosition(old_pos_B, current_pos_B, linesWall.at(j), linesWall.at(j + 1));
                new_position -= sf::Vector2f(player->GetSize().x, 0);

                player->SetPosition(new_position);

                current_pos_A = player->GetPointPosition('A');
                current_pos_B = player->GetPointPosition('B');
                current_pos_C = player->GetPointPosition('C');
                current_pos_D = player->GetPointPosition('D');
            }
        }

        // C
        for(int j = 0; j < 8; j+=2) {
            if(GetIntersectPosition(old_pos_C, current_pos_C, linesWall.at(j), linesWall.at(j + 1)).is_intersection) {
                std::cout << "Collision C" << std::endl;

                sf::Vector2f new_position = GetPlayerCollisionPosition(old_pos_C, current_pos_C, linesWall.at(j), linesWall.at(j + 1));
                new_position -= sf::Vector2f(0, player->GetSize().y);

                player->SetPosition(new_position);

                current_pos_A = player->GetPointPosition('A');
                current_pos_B = player->GetPointPosition('B');
                current_pos_C = player->GetPointPosition('C');
                current_pos_D = player->GetPointPosition('D');
            }
        }

        // D
        for(int j = 0; j < 8; j+=2) {
            if(GetIntersectPosition(old_pos_D, current_pos_D, linesWall.at(j), linesWall.at(j + 1)).is_intersection) {
                std::cout << "Collision D" << std::endl;

                sf::Vector2f new_position = GetPlayerCollisionPosition(old_pos_D, current_pos_D, linesWall.at(j), linesWall.at(j + 1));
                new_position -= sf::Vector2f(player->GetSize().x, player->GetSize().y);

                player->SetPosition(new_position);

                current_pos_A = player->GetPointPosition('A');
                current_pos_B = player->GetPointPosition('B');
                current_pos_C = player->GetPointPosition('C');
                current_pos_D = player->GetPointPosition('D');
            }
        }

    }

    //std::cout << old_pos_player.x << " " << old_pos_player.y << " - " << current_pos_player.x << " " << current_pos_player.y << std::endl;

    old_pos_A = current_pos_A;
    old_pos_B = current_pos_B;
    old_pos_C = current_pos_C;
    old_pos_D = current_pos_D;
}



