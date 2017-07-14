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

sf::View Render::GetDefaultView() {
    return window->getDefaultView();
}

bool Render::GetEvent(sf::Event *event) {
    return window->pollEvent(*event);
}

void Render::ResetViev() {
    sf::View view = window->getDefaultView();
    window->setView(view);
}

void Render::Close() {
    window->close();
}

void Render::Clear() {
    window->clear();;
}

void Render::Draw(std::vector <Surface> *surfaces) {
    for(int i = 0; i < surfaces->size(); ++i) {
       window->draw(surfaces->at(i).GetPolygon(), surfaces->at(i).GetRenderState());
    }
}

void Render::Draw(Player *player) {
    window->draw(player->player);
}

void Render::Draw(std::vector <Wall> *walls) {
    for (int i = 0; i < walls->size(); ++i) {
        window->draw(walls->at(i).GetRect());
    }
}

void Render::Draw(sf::Text text) {
    window->draw(text);
}

void Render::Draw(sf::ConvexShape object) {
    window->draw(object);
}

void Render::SetView(sf::View view) {
    window->setView(view);
}

void Render::Display() {
    window->display();
}
