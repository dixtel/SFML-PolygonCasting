#include "player.h"

Player::Player() {

}

Player::Player(sf::Vector2f position, sf::Vector2f size, std::string texturePath) {
    this->position     = position;
    this->texturePath  = texturePath;
    this->size         = size;
    direction          = 90;
    angleView          = 70;
    distanceView       = 1000;
    color              = sf::Color::Green;
    force.x            = 5;
    force.y            = 5;
    velocity.x         = 0;
    velocity.y         = 0;
    max_velocity.x     = 5;
    max_velocity.y     = 5;
    acceleration.x     = 0.5;
    acceleration.y     = 0.5;
    friction           = 0.7;
    moveDirectionSpeed = 1;
    is_colising        = false;
    right              = false;
    left               = false;
    up                 = false;
    down               = false;
    directionRight     = false;
    directionLeft      = false;

    UpdatePoints();
    UpdatePlayer();
}

void Player::SetPosition(sf::Vector2f pos) {
    position = pos;
    UpdatePoints();
}

void Player::UpdatePoints() {
    pos_A = sf::Vector2f(GetPosition().x, GetPosition().y);
    pos_B = sf::Vector2f(GetPosition().x + GetSize().x, GetPosition().y);
    pos_C = sf::Vector2f(GetPosition().x, GetPosition().y + GetSize().y);
    pos_D = sf::Vector2f(GetPosition().x + GetSize().x, GetPosition().y + GetSize().y);
}

void Player::UpdatePhysics() {
    //move
    if(right) {
        velocity.x += force.x * acceleration.x;
    }
    if(left) {
        velocity.x -= force.x * acceleration.x;
    }
    if(up) {
        velocity.y -= force.y * acceleration.y;
    }
    if(down) {
        velocity.y += force.y * acceleration.y;
    }

    //friction
    if(velocity.x > 0) {
        velocity.x -= friction;
        if(velocity.x < 0) velocity.x = 0;
    }
    else if(velocity.x < 0) {
        velocity.x += friction;
        if(velocity.x > 0) velocity.x = 0;
    }

    if(velocity.y > 0) {
        velocity.y -= friction;
        if(velocity.y < 0) velocity.y = 0;
    }
    else if(velocity.y < 0) {
        velocity.y += friction;
        if(velocity.y > 0) velocity.y = 0;
    }

    //max speed
    if(velocity.x > max_velocity.x) velocity.x = max_velocity.x;
    else if(velocity.x < -max_velocity.x) velocity.x = -max_velocity.x;

    if(velocity.y > max_velocity.y) velocity.y = max_velocity.y;
    else if(velocity.y < -max_velocity.y) velocity.y = -max_velocity.y;

    //direction
    if(directionRight) {
        direction -= moveDirectionSpeed;
        if(direction < 0) {
            direction = 360 + fmod(direction, 360);
        }
    }
    else if(directionLeft) {
        direction += moveDirectionSpeed;
        if(direction > 360) {
            direction = fmod(direction, 360);
        }
    }

    position += velocity;

    UpdatePoints();
}

void Player::UpdatePlayer() {
    player.setSize(size);
    player.setPosition(position);
    player.setFillColor(color);
}

void Player::GoDirectionRight(bool enable) {
    directionRight = enable;
}

void Player::GoDirectionLeft(bool enable) {
    directionLeft = enable;
}

void Player::GoRight(bool enable) {
    right = enable;
}

void Player::GoLeft(bool enable) {
    left = enable;
}

void Player::GoUp(bool enable) {
    up = enable;
}

void Player::GoDown(bool enable) {
    down = enable;
}

float Player::GetAngleView() {
    return angleView;
}

float Player::GetDistanceView() {
    return distanceView;
}

float Player::GetDirection() {
    return direction;
}

sf::Vector2f Player::GetPosition() {
    return position;
}

sf::Vector2f Player::GetSize() {
    return size;
}

sf::Vector2f Player::GetVelocity() {
    return velocity;
}

sf::Vector2f Player::GetPointPosition(const char point) {
    switch (point) {
    case 'A':
        return pos_A;
        break;
    case 'B':
        return pos_B;
        break;
    case 'C':
        return pos_C;
        break;
    case 'D':
        return pos_D;
        break;
    }

    return sf::Vector2f(0, 0);
}

sf::ConvexShape Player::GetPlayerView() {
    sf::Vector2f player_center_pos = sf::Vector2f(position.x + (size.x/2), position.y + (size.y/2));

    sf::ConvexShape view;
    view.setFillColor(sf::Color(255, 255, 255, 100));
    view.setPointCount(3);
    view.setPoint(0, player_center_pos);
    view.setPoint(1, sf::Vector2f(player_center_pos.x + (ToolKit::cosine(fmod((direction + (angleView/2)), 360)) * distanceView), player_center_pos.y + (-ToolKit::sine(fmod((direction + (angleView/2)), 360)) * distanceView)));
    view.setPoint(2, sf::Vector2f(player_center_pos.x + (ToolKit::cosine(fmod((direction - (angleView/2)), 360)) * distanceView), player_center_pos.y + (-ToolKit::sine(fmod((direction - (angleView/2)), 360)) * distanceView)));

    return view;
}


