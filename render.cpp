#include "render.h"

Render::Render(int width, int height, std::string title, int fps) {
    window = new sf::RenderWindow(sf::VideoMode(width, height), title);
    window->setFramerateLimit(fps);
}

Render::~Render() {
    if(window) {
        delete window;
        window = NULL;
    }
}

bool Render::GetEvent(sf::Event *event) {
    return window->pollEvent(*event);
}

void Render::Close() {
    window->close();
}

void Render::Clear() {
    window->clear();;
}

void Render::Draw(Player *player) {
    window->draw(player->player);
}

void Render::Draw(std::vector <Wall*> walls) {
    for(std::vector<Wall*>::iterator it = walls.begin(); it != walls.end(); ++it) {
        window->draw((*it)->wall);
    }
}

void Render::Draw(sf::Text text) {
    window->draw(text);
}

void Render::Display() {
    window->display();
}
