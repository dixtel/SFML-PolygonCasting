#ifndef POLYGONCAST_H
#define POLYGONCAST_H

#include <SFML/Graphics.hpp>
#include <gameobject.h>
#include "toolkit.h"
#include <vector>

struct Surface {
private:
    sf::Vector2f pos1; //pos1 -- pos2
    sf::Vector2f pos2; //pos4 -- pos3
    sf::Vector2f pos3;
    sf::Vector2f pos4;

    sf::VertexArray  polygon;
    std::string      texturePath;
    sf::Texture      texture;
    sf::RenderStates state;
public:
    Surface(sf::Vector2f pos1, sf::Vector2f pos2, sf::Vector2f pos3, sf::Vector2f pos4, std::string texturePath = "", sf::Color colorPos1Pos4 = sf::Color::Red, sf::Color colorPos2Pos3 = sf::Color::Red) {
        this->pos1 = pos1;
        this->pos2 = pos2;
        this->pos3 = pos3;
        this->pos4 = pos4;
        this->texturePath = texturePath;

        polygon.setPrimitiveType(sf::PrimitiveType::Quads);

        if(!texture.loadFromFile(texturePath)) {
            std::cout << "error: cannot load file (PolygonCast)" << std::endl;

            polygon.append(sf::Vertex(pos1, colorPos1Pos4));
            polygon.append(sf::Vertex(pos2, colorPos2Pos3));
            polygon.append(sf::Vertex(pos3, colorPos2Pos3));
            polygon.append(sf::Vertex(pos4, colorPos1Pos4));
        }
        else {
            sf::Vector2f textureSize = sf::Vector2f(texture.getSize().x, texture.getSize().y);
            polygon.append(sf::Vertex(pos1, sf::Vector2f(0, 0)));
            polygon.append(sf::Vertex(pos2, sf::Vector2f(textureSize.x, 0)));
            polygon.append(sf::Vertex(pos3, sf::Vector2f(textureSize.x, textureSize.y)));
            polygon.append(sf::Vertex(pos4, sf::Vector2f(0, textureSize.y)));
        }
    };


    sf::VertexArray &GetPolygon() {
        return polygon;
    };

    sf::RenderStates GetRenderState() {
        state.texture = &texture;

        return state;
    };
};

 struct lineSegment {
     Toolkit::Vector2f_pair line;
     float                  distance_A;
     float                  distance_B;
     float                  height;
     std::string            texturePath;

     lineSegment() {
         line.A      = sf::Vector2f(0, 0);
         line.B      = sf::Vector2f(0, 0);
         distance_A  = 0;
         distance_B  = 0;
         height      = 0;
         texturePath = "";
     };

     lineSegment(sf::Vector2f pos1, sf::Vector2f pos2, float distance_A, float distance_B, float height, std::string texturePath) {
         line.A            = pos1;
         line.B            = pos2;
         this->distance_A  = distance_A;
         this->distance_B  = distance_B;
         this->height      = height;
         this->texturePath = texturePath;
     };

     bool operator==(lineSegment &lineToCheck) {
         if((lineToCheck.distance_A == distance_A) &&
            (lineToCheck.distance_B == distance_B) &&
            (lineToCheck.height == height) &&
            (lineToCheck.line == line)) {
             return true;
         }
         return false;
     };
 };

class PolygonCast {
    int widthWindow;
    int heightWindow;

    float angleView;
    float distanceView;
    float directionView;
    sf::Vector2f cameraPosition;


    std::vector <Surface> surfaces;
    Surface CalculateSurface(Toolkit::Vector2f_pair line, float distanceA, float distanceB, float height, std::string texturePath);
    void SortLineSegments(std::vector <lineSegment> *lines);
    void ClearSurfaces();
    std::vector <Wall> GetWallsOnPlayerView(std::vector<Wall> *walls);
    void CreateSurfaces(std::vector<Wall> *walls);

public:
    PolygonCast(const int widthhWindow, const int heightWindow);

    void CreateView(Player *player, std::vector<Wall> *walls);

    std::vector <Surface> &GetSurfaces();

};

#endif // POLYGONCAST_H
