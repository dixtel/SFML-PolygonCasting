#include "game.h"

Game::Game() {
    render     = new Render(window_width, window_height, window_title, fps);
    gameObject = new GameObject();
    world      = new World();
    collision  = new Collision();
}

Game::~Game() {
    if(render) {
        delete render;
        render = NULL;
    }
    if(gameObject) {
        delete gameObject;
        gameObject = NULL;
    }
    if(world) {
        delete world;
        world = NULL;
    }
    if(collision) {
        delete collision;
        collision = NULL;
    }
}

void Game::StartGameLoop() {
    world->LoadMap("Levels/level1.txt");
    if(!world->isLoad()) {
        return;
    }
    world->InitGameObjects(gameObject);


    sf::Font font;
    if(!font.loadFromFile("font.ttf")) {
        std::cout << "error load font ;/" << std::endl;
        return;
    }

    sf::Text text_debugger;
    text_debugger.setFont(font);
    text_debugger.setCharacterSize(15);
    text_debugger.setStyle(sf::Text::Bold);
    text_debugger.setColor(sf::Color::Red);

    sf::Clock clock;
    float lastTime = 0;

    while(game_loop) {

        sf::Event event;
        while(render->GetEvent(&event)) {
            if(event.type == sf::Event::Closed) {
                render->Close();
                game_loop = false;
            }

            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Right)) {
                gameObject->GetPlayer()->GoRight(true);
            }
            else if((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Right)) {
                 gameObject->GetPlayer()->GoRight(false);
            }

            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Left)) {
                gameObject->GetPlayer()->GoLeft(true);
            }
            else if((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Left)) {
                 gameObject->GetPlayer()->GoLeft(false);
            }

            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Up)) {
                gameObject->GetPlayer()->GoUp(true);
            }
            else if((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Up)) {
                 gameObject->GetPlayer()->GoUp(false);
            }

            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down)) {
                gameObject->GetPlayer()->GoDown(true);
            }
            else if((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Down)) {
                gameObject->GetPlayer()->GoDown(false);
            }
        }

        gameObject->GetPlayer()->UpdatePhysics();

        //collision
        collision->SetCollision(gameObject->GetPlayer(), gameObject->GetWalls());


        gameObject->GetPlayer()->UpdatePlayer();


        text_debugger.setString("fps:" + std::to_string(fps) +
                                " x:" + std::to_string(gameObject->GetPlayer()->GetPosition().x) +
                                " y:" + std::to_string(gameObject->GetPlayer()->GetPosition().y) +
                                " vx:" + std::to_string(gameObject->GetPlayer()->GetVelocity().x) +
                                " vy:" + std::to_string(gameObject->GetPlayer()->GetVelocity().y));


        render->Clear();
        render->Draw(gameObject->GetPlayer());
        render->Draw(gameObject->GetWalls());
        render->Draw(text_debugger);
        render->Display();

        float currentTime = clock.restart().asSeconds();
        float fps = 1.f / (currentTime - lastTime);
        lastTime = currentTime;
    }
}


