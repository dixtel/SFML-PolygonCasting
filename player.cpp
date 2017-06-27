#include "player.h"

Player::Player(sf::Vector2f position, sf::Vector2f size, std::string texturePath) {
    this->position    = position;
    this->texturePath = texturePath;
    this->size        = size;
    direction         = 90;
    color             = sf::Color::Green;
    force.x           = 2;
    force.y           = 2;
    velocity.x        = 0;
    velocity.y        = 0;
    max_velocity.x    = 5;
    max_velocity.y    = 5;
    acceleration.x    = 0.5;
    acceleration.y    = 0.5;
    friction          = 0.5;


    is_colising       = false;
    right             = false;
    left              = false;
    up                = false;
    down              = false;

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

    position += velocity;
    UpdatePoints();

    std::cout << direction << std::endl;
}

void Player::UpdatePlayer() {
    player.setSize(size);
    player.setPosition(position);
    player.setFillColor(color);
}

void Player::GoDirectionRight(float moveLenght) {
    direction -= moveLenght;
    if(direction < 0) {
        direction = 360 + fmod(direction, 360);
    }
}

void Player::GoDirectionLeft(bool moveLenght) {
    direction += moveLenght;
    if(direction > 369) {
        direction = fmod(direction, 360);
    }
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

