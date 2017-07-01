#ifndef POLYGONCAST_H
#define POLYGONCAST_H

#include <SFML/Graphics.hpp>
#include <gameobject.h>
#include "toolkit.h"
#include <vector>

class PolygonCast
{
   struct Surface {
    private:
        sf::Vector2f pos1; //pos1 -- pos2
        sf::Vector2f pos2; //pos4 -- pos3
        sf::Vector2f pos3;
        sf::Vector2f pos4;

        sf::VertexArray polygon;
        std::string texturePath;
        sf::Texture texture;
        sf::RenderStates state;
    public:
        Surface(sf::Vector2f pos1, sf::Vector2f pos2, sf::Vector2f pos3, sf::Vector2f pos4, std::string texturePath = "", sf::Color color = sf::Color::Black) {
            this->pos1 = pos1;
            this->pos2 = pos2;
            this->pos3 = pos3;
            this->pos4 = pos4;
            this->texturePath = texturePath;

            if(!texture.loadFromFile(texturePath)) {
                std::cout << "error: cannot load file (PolygonCast)" << std::endl;
            }
            else {
                state.texture = &texture;
            }

        }

        sf::VertexArray GetPolygon() {
            return polygon;
        }

        sf::RenderStates GetRenderState() {
            return state;
        }
    };

    struct lineSegment {
        ToolKit::Vector2f_pair line;
        float                  distance_A;
        float                  distance_B;
        float                  height;
        std::string            texturePath;

        lineSegment(sf::Vector2f pos1, sf::Vector2f pos2, float distance_A, float distance_B, float height, std::string texturePath) {
            line.A            = pos1;
            line.B            = pos2;
            this->distance_A  = distance_A;
            this->distance_B  = distance_B;
            this->texturePath = texturePath;
        }
    };


    float angleView;
    float distanceView;
    float player_dir;
    sf::Vector2f player_pos;
    sf::Vector2f player_center_pos;


    std::vector <Surface*> surfaces;
    void ClearSurfaces();
    std::vector <Wall*> GetWallsOnPlayerView(std::vector <Wall*> walls);
    void CreateSurfaces(std::vector<Wall*> *walls);

public:
    PolygonCast();

    void SetAngleView(float angle);
    void SetDistanceView(float distance);

    void CreateView(Player *player, std::vector<Wall*> *walls);
};

#endif // POLYGONCAST_H
