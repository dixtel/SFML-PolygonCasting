#include "collision.h"

sf::Vector2f GetPlayerCollisionPosition(sf::Vector2f old_pos, sf::Vector2f invalid_pos, sf::Vector2f start_pos_obstacle, sf::Vector2f end_pos_obstacle) {
    sf::Vector2f intersect_position = ToolKit::GetIntersection(old_pos, invalid_pos, start_pos_obstacle, end_pos_obstacle).position;
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


    float angleA1 = ToolKit::GetAngle(end_pos_obstacle, start_pos_obstacle);
    float angleA2 = ToolKit::GetAngle(start_pos_obstacle, end_pos_obstacle);

    if(angleA1 > angleA2) {
        float tmp = angleA2;
        angleA2 = angleA1;
        angleA1 = tmp;
    }

    float start_vector_angle = ToolKit::GetAngle(invalid_pos, old_pos);
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
        sf::Vector2f vector = sf::Vector2f(ToolKit::CoSine(new_direction), -ToolKit::Sine(new_direction));

        float reverse_angle = ToolKit::GetAngle(invalid_pos, old_pos);
        float x = ToolKit::CoSine(reverse_angle);
        float y = -ToolKit::Sine(reverse_angle);

        intersect_position += sf::Vector2f(x * space, y * space);

        new_player_position = intersect_position + (vector * distance);
    }
    else if(angle_A1_start < angle_A2_start) {
        double new_direction = angleA2;
        sf::Vector2f vector = sf::Vector2f(ToolKit::CoSine(new_direction), -ToolKit::Sine(new_direction));

        float reverse_angle = ToolKit::GetAngle(invalid_pos, old_pos);
        float x = ToolKit::CoSine(reverse_angle);
        float y = -ToolKit::Sine(reverse_angle);

        intersect_position += sf::Vector2f(x * space, y * space);

        new_player_position = intersect_position + (vector * distance);
    }
    else {
        double new_direction = ToolKit::GetAngle(intersect_position, old_pos);
        sf::Vector2f vector = sf::Vector2f(ToolKit::CoSine(new_direction), -ToolKit::Sine(new_direction));

        if(new_direction == 90 || new_direction == 270) vector.y = 0;
        else if(new_direction == 0 || new_direction == 360 || new_direction == 180) vector.x = 0;

        float reverse_angle = ToolKit::GetAngle(invalid_pos, old_pos);
        float space_x = ToolKit::CoSine(reverse_angle);
        float space_y = -ToolKit::Sine(reverse_angle);

        intersect_position += sf::Vector2f(space_x * space, space_y * space);

        new_player_position = intersect_position + (vector * distance);
    }

    return new_player_position;
}


Collision::Collision() {

}

void Collision::SetCollision(Player *player, std::vector<Wall> *walls) {
    static sf::Vector2f old_pos_A = player->GetPointPosition('A');
    static sf::Vector2f old_pos_B = player->GetPointPosition('B');
    static sf::Vector2f old_pos_C = player->GetPointPosition('C');
    static sf::Vector2f old_pos_D = player->GetPointPosition('D');
    sf::Vector2f current_pos_A = player->GetPointPosition('A');
    sf::Vector2f current_pos_B = player->GetPointPosition('B');
    sf::Vector2f current_pos_C = player->GetPointPosition('C');
    sf::Vector2f current_pos_D = player->GetPointPosition('D');

    for(int i = 0; i < walls->size(); ++i) {
        sf::Vector2f wall_pos = walls->at(i).GetPosition();
        sf::Vector2f wall_size = walls->at(i).GetSize();

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

        sf::Vector2f A = walls->at(i).GetPointPosition('A');
        sf::Vector2f B = walls->at(i).GetPointPosition('B');
        sf::Vector2f C = walls->at(i).GetPointPosition('C');
        sf::Vector2f D = walls->at(i).GetPointPosition('D');

        std::array<sf::Vector2f, 8> linesWall{ C, A,
                                               A, B,
                                               B, D,
                                               D, C };

        // A
        for(int j = 0; j < 8; j+=2) {
            if(ToolKit::GetIntersection(old_pos_A, current_pos_A, linesWall.at(j), linesWall.at(j + 1)).is_intersection) {
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
            if(ToolKit::GetIntersection(old_pos_B, current_pos_B, linesWall.at(j), linesWall.at(j + 1)).is_intersection) {
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
            if(ToolKit::GetIntersection(old_pos_C, current_pos_C, linesWall.at(j), linesWall.at(j + 1)).is_intersection) {
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
            if(ToolKit::GetIntersection(old_pos_D, current_pos_D, linesWall.at(j), linesWall.at(j + 1)).is_intersection) {
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



