#include "player.h"

Player::Player(sf::Vector2f position, sf::Vector2f size, std::string texturePath) {
    this->position    = position;
    this->texturePath = texturePath;
    radius            = size.x;
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

    UpdatePlayer();
}

void Player::UpdatePhysics() {
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

    std::cout << velocity.x << " " << velocity.y << std::endl;

    position += velocity;
}

void Player::UpdatePlayer() {
    player.setPosition(position);
    player.setRadius(radius);
    player.setFillColor(color);
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

sf::Vector2f Player::GetVelocity() {
    return velocity;
}