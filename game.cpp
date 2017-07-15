#include "game.h"

Game::Game() {
    render     = new Render(window_width, window_height, window_title, fps);
    gameObject = new GameObject();
    world      = new World();
    collision  = new Collision();
    polygonCast = new PolygonCast(window_width, window_height);
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
        std::cout << "error load font (Game)" << std::endl;
        return;
    }

    sf::Text text_debugger;
    text_debugger.setFont(font);
    text_debugger.setCharacterSize(15);
    text_debugger.setStyle(sf::Text::Bold);
    text_debugger.setColor(sf::Color::Red);

    sf::Clock clock;
    float lastTime = 0;

    sf::View minimap(sf::FloatRect(-800, -800, window_width + 1600, window_height + 1600));
    minimap.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));

    sf::Mouse mouse;

    while(game_loop) {

        sf::Event event;
        while(render->GetEvent(&event)) {
            if(event.type == sf::Event::Closed) {
                render->Close();
                game_loop = false;
            }

            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::D)) {
                gameObject->GetPlayer().GoRight(true);
            }
            else if((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::D)) {
                 gameObject->GetPlayer().GoRight(false);
            }

            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::A)) {
                gameObject->GetPlayer().GoLeft(true);
            }
            else if((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::A)) {
                 gameObject->GetPlayer().GoLeft(false);
            }

            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::W)) {
                gameObject->GetPlayer().GoUp(true);
            }
            else if((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::W)) {
                 gameObject->GetPlayer().GoUp(false);
            }

            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S)) {
                gameObject->GetPlayer().GoDown(true);
            }
            else if((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::S)) {
                gameObject->GetPlayer().GoDown(false);
            }

            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Right)) {
                gameObject->GetPlayer().GoDirectionRight(true);

            }
            else if((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Right)) {
                gameObject->GetPlayer().GoDirectionRight(false);
            }

            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Left)) {
                gameObject->GetPlayer().GoDirectionLeft(true);
            }
            else if((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Left)) {
                gameObject->GetPlayer().GoDirectionLeft(false);
            }

        }

        gameObject->GetPlayer().UpdatePhysics();

        //collision
        collision->SetCollision(&gameObject->GetPlayer(), &gameObject->GetWalls());


        gameObject->GetPlayer().UpdatePlayer();


        polygonCast->CreateView(&gameObject->GetPlayer(), &gameObject->GetWalls());


        text_debugger.setString("fps:" + std::to_string(fps) +
                                " x:" + std::to_string(gameObject->GetPlayer().GetPosition().x) +
                                " y:" + std::to_string(gameObject->GetPlayer().GetPosition().y) +
                                " vx:" + std::to_string(gameObject->GetPlayer().GetVelocity().x) +
                                " vy:" + std::to_string(gameObject->GetPlayer().GetVelocity().y));

        minimap.setCenter(gameObject->GetPlayer().GetPosition());


        render->Clear();
//        render->Draw(world->GetCeiling(), &world->GetCeilingRenderState());
//        render->Draw(world->GetFloor(), &world->GetFloorRenderState());
        render->Draw(&polygonCast->GetSurfaces());
        render->SetView(minimap);
        render->Draw(gameObject->GetPlayer().GetPlayerView());
        render->Draw(&gameObject->GetPlayer());
        render->Draw(&gameObject->GetWalls());
        render->ResetViev();
        render->Draw(text_debugger);
        render->Display();

        float currentTime = clock.restart().asSeconds();
        float fps = 1.f / (currentTime - lastTime);
        lastTime = currentTime;
    }
}


