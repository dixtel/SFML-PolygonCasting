#ifndef RENDER_H
#define RENDER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "gameobject.h"

class Render
{
    sf::RenderWindow *window;
public:
    Render(int width, int height, std::string title, int fps);
    ~Render();

    bool GetEvent(sf::Event *event);
    void Close();
    void Clear();
    void Draw(Player *player);
    void Draw(std::vector <Wall*> walls);
    void Draw(sf::Text text);
    void Display();
};

#endif // RENDER_H
