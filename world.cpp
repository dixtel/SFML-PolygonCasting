#include "world.h"

float ToFloat(std::string number) {
    float x;
    std::istringstream stream(number);
    stream >> x;
    return x;
}

sf::Vector2f ToFloats(std::string numbers) {
    std::string var1 = numbers.substr(0, numbers.find(","));
    std::string var2 = numbers.substr(numbers.find(",") + 1, numbers.size() - 1);

    return sf::Vector2f(ToFloat(var1), ToFloat(var2));
}

World::World() {

}

void World::LoadMap(const std::string path) {
    std::ifstream file;
    std::vector <std::string> data;

    file.open(path);
    if(file.is_open()) {
        is_load = true;
        std::string tmp_line;
        while(std::getline(file, tmp_line)) {
            if(tmp_line.size() >= 2) {
                if((tmp_line[0] == '/') && (tmp_line[1] == '/')) {
                    continue;
                }
            }
            data.push_back(tmp_line);
        }
        file.close();
    }
    else {
        is_load = false;
        return;
    }

    //parsing

    for(int i = 0; i < data.size(); ++i) {
        Object object;
        std::vector <std::string> object_attributes;
        std::string tmp_attribute;

        std::istringstream stream(data[i]);

        while(stream >> tmp_attribute) {
            object_attributes.push_back(tmp_attribute);
        }

        for(int j = 0; j < object_attributes.size(); j++) {
            if((object_attributes[j].substr(0, object_attributes[j].find(":"))) == "type") {
                std::cout << "type found\n";
                object.type = object_attributes[j].substr(object_attributes[j].find(":") + 1, object_attributes[j].size() - 1);
            }

            if((object_attributes[j].substr(0, object_attributes[j].find(":"))) == "position") {
                std::cout << "position found\n";

                object.position = ToFloats(object_attributes[j].substr(object_attributes[j].find(":") + 1, object_attributes[j].size() - 1));
            }

            if((object_attributes[j].substr(0, object_attributes[j].find(":"))) == "size") {
                std::cout << "size found\n";
                object.size = ToFloats(object_attributes[j].substr(object_attributes[j].find(":") + 1, object_attributes[j].size() - 1));
            }

            if((object_attributes[j].substr(0, object_attributes[j].find(":"))) == "height") {
                std::cout << "height found\n";
                object.height = ToFloat(object_attributes[j].substr(object_attributes[j].find(":") + 1, object_attributes[j].size() - 1));
            }

            if((object_attributes[j].substr(0, object_attributes[j].find(":"))) == "angle") {
                std::cout << "angle found\n";
                object.angle = ToFloat(object_attributes[j].substr(object_attributes[j].find(":") + 1, object_attributes[j].size() - 1));
            }

            if((object_attributes[j].substr(0, object_attributes[j].find(":"))) == "texturepath") {
                std::cout << "texturepath found\n";
                object.texturePath = object_attributes[j].substr(object_attributes[j].find(":") + 1, object_attributes[j].size() - 1);
            }

        }

        //TODO IN WORK fix wall y position

        objects.push_back(object);
        std::cout << "type: " << object.type               << std::endl
                  << "position: " << object.position.x
                  << "," << object.position.y              << std::endl
                  << "size: " << object.size.x
                  << "," << object.size.y                  << std::endl
                  << "height: " << object.height           << std::endl
                  << "angle: " << object.angle             << std::endl
                  << "texturepath: " << object.texturePath << std::endl;
    }

    for(int i = 0; i < objects.size(); ++i) {
        if(objects[i].type == "floor") {
            floor.setPrimitiveType(sf::PrimitiveType::Quads);

            if(!floorTexture.loadFromFile(objects[i].texturePath)) {
                std::cout << "error: " << objects[i].texturePath << " path to texture are valid (World)" << std::endl;
            }
            else {
                floorRenderState.texture = &floorTexture;

                sf::Vector2f floorTextureSize = sf::Vector2f(floorTexture.getSize().x, floorTexture.getSize().y);

                sf::Vector2f pos1 = sf::Vector2f(objects[i].position.x, objects[i].position.y);
                sf::Vector2f pos2 = sf::Vector2f(objects[i].position.x + objects[i].size.x, objects[i].position.y);
                sf::Vector2f pos3 = sf::Vector2f(objects[i].position.x + objects[i].size.x, objects[i].position.y + + objects[i].size.y);
                sf::Vector2f pos4 = sf::Vector2f(objects[i].position.x, objects[i].position.y + + objects[i].size.y);

                floor.append(sf::Vertex(pos1, sf::Vector2f(0, 0)));
                floor.append(sf::Vertex(pos2, sf::Vector2f(floorTextureSize.x, 0)));
                floor.append(sf::Vertex(pos3, sf::Vector2f(floorTextureSize.x, floorTextureSize.y)));
                floor.append(sf::Vertex(pos4, sf::Vector2f(0, floorTextureSize.y)));
            }

        }

        if(objects[i].type == "ceiling") {
            ceiling.setPrimitiveType(sf::PrimitiveType::Quads);


            if(!ceilingTexture.loadFromFile(objects[i].texturePath)) {
                std::cout << "error: " << objects[i].texturePath << " path to texture are valid (World)" << std::endl;
            }
            else {
                ceilingRenderState.texture = &ceilingTexture;

                sf::Vector2f ceilingTextureSize = sf::Vector2f(ceilingTexture.getSize().x, ceilingTexture.getSize().y);

                sf::Vector2f pos1 = sf::Vector2f(objects[i].position.x, objects[i].position.y);
                sf::Vector2f pos2 = sf::Vector2f(objects[i].position.x + objects[i].size.x, objects[i].position.y);
                sf::Vector2f pos3 = sf::Vector2f(objects[i].position.x + objects[i].size.x, objects[i].position.y + + objects[i].size.y);
                sf::Vector2f pos4 = sf::Vector2f(objects[i].position.x, objects[i].position.y + + objects[i].size.y);

                ceiling.append(sf::Vertex(pos1, sf::Vector2f(0, 0)));
                ceiling.append(sf::Vertex(pos2, sf::Vector2f(ceilingTextureSize.x, 0)));
                ceiling.append(sf::Vertex(pos3, sf::Vector2f(ceilingTextureSize.x, ceilingTextureSize.y)));
                ceiling.append(sf::Vertex(pos4, sf::Vector2f(0, ceilingTextureSize.y)));
            }
        }
    }

}

void World::InitGameObjects(GameObject *gameObject) {
    for(int i = 0; i < objects.size(); ++i) {
        if(objects[i].type == "player") {
            gameObject->CreatePlayer(objects[i].position, objects[i].size, objects[i].texturePath);
        }

        if(objects[i].type == "wall") {
            gameObject->CreateWall(objects[i].position, objects[i].size, objects[i].height, objects[i].angle, objects[i].texturePath);
        }
    }
}

sf::VertexArray World::GetFloor() {
    return floor;
}

sf::VertexArray World::GetCeiling() {
    return ceiling;
}

sf::RenderStates &World::GetFloorRenderState() {
    return floorRenderState;
}

sf::RenderStates &World::GetCeilingRenderState() {
    return ceilingRenderState;
}

bool World::isLoad() {
    return this->is_load;
}
