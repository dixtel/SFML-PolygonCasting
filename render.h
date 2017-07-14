#ifndef RENDER_H
#define RENDER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "gameobject.h"
#include "polygoncast.h"

class Render
{
    sf::RenderWindow *window;
public:
    Render(int width, int height, std::string title, int fps);
    ~Render();

    sf::View GetDefaultView();
    bool GetEvent(sf::Event *event);
    void ResetViev();
    void SetView(sf::View view);
    void Close();
    void Clear();
    void Draw(std::vector <Surface> *surfaces);
    void Draw(Player *player);
    void Draw(std::vector <Wall> *walls);
    void Draw(sf::RectangleShape object);
    void Draw(sf::Text text);
    void Draw(sf::ConvexShape object);
    void Draw(sf::VertexArray vertex, sf::RenderStates *state);
    void Display();
};

#endif // RENDER_H
