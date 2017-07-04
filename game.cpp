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

    polygonCast->SetAngleView(gameObject->GetPlayer().GetAngleView());
    polygonCast->SetDistanceView(gameObject->GetPlayer().GetDistanceView());

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

    sf::View minimap(render->GetDefaultView());
    minimap.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));

    sf::Mouse mouse;

    while(game_loop) {

        sf::Event event;
        while(render->GetEvent(&event)) {
            if(event.type == sf::Event::Closed) {
                render->Close();
                game_loop = false;
            }

            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Right)) {
                gameObject->GetPlayer().GoRight(true);
            }
            else if((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Right)) {
                 gameObject->GetPlayer().GoRight(false);
            }

            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Left)) {
                gameObject->GetPlayer().GoLeft(true);
            }
            else if((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Left)) {
                 gameObject->GetPlayer().GoLeft(false);
            }

            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Up)) {
                gameObject->GetPlayer().GoUp(true);
            }
            else if((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Up)) {
                 gameObject->GetPlayer().GoUp(false);
            }

            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down)) {
                gameObject->GetPlayer().GoDown(true);
            }
            else if((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Down)) {
                gameObject->GetPlayer().GoDown(false);
            }
        }


        static float old_mouse_x = mouse.getPosition().x;
        float new_mouse_x = mouse.getPosition().x;
        if(old_mouse_x != new_mouse_x) {
            float mouse_move = new_mouse_x - old_mouse_x;
            if(mouse_move > 0) {
                mouse_move = fabs(mouse_move) / 5;
                gameObject->GetPlayer().GoDirectionRight(mouse_move);
            }
            else {
                mouse_move = fabs(mouse_move) / 5;
                gameObject->GetPlayer().GoDirectionLeft(mouse_move);

            }
        }
        old_mouse_x = new_mouse_x;

        gameObject->GetPlayer().UpdatePhysics();

        //collision
        collision->SetCollision(&gameObject->GetPlayer(), &gameObject->GetWalls());


        gameObject->GetPlayer().UpdatePlayer();


        text_debugger.setString("fps:" + std::to_string(fps) +
                                " x:" + std::to_string(gameObject->GetPlayer().GetPosition().x) +
                                " y:" + std::to_string(gameObject->GetPlayer().GetPosition().y) +
                                " vx:" + std::to_string(gameObject->GetPlayer().GetVelocity().x) +
                                " vy:" + std::to_string(gameObject->GetPlayer().GetVelocity().y));



        minimap.setCenter(gameObject->GetPlayer().GetPosition());


        polygonCast->CreateView(&gameObject->GetPlayer(), &gameObject->GetWalls());

        render->Clear();
        //render->SetView(minimap);
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


