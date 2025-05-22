#include <SFML/Graphics.hpp>
#include "bird.hpp"
#include <list>
#include <iostream>

int NUM_BIRDS = 30;
int WINDOW_X = 900;
int WINDOW_Y = 600;

int main()
{
    std::cout << "Program started.\n";
    std::list<Bird> birds = {};
    auto window = sf::RenderWindow(sf::VideoMode({900u, 600u}), "boids");
    window.setFramerateLimit(60);

    // Create birds
    for (int i = 0; i < NUM_BIRDS; i++) {
        Bird bird;

        int rotation = rand() % 360;
        bird.setRotation(rotation);

        int x = rand() % 900;
        int y = rand() % 600;
        bird.setPosition(x, y);

        birds.push_back(bird);
        printf("Adding bird %d rotation %d pos %d,%d.\n", i, rotation, x, y);
    }
    printf("Created %d birds.\n", birds.size());

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();

        // Draw
        for (Bird& bird : birds) {
            window.draw(bird);
            // TODO: Maybe assign flock less frequently.
            std::list<Bird> flock = bird.getFlock(birds);
            bird.move(flock);
        }

        window.display();
    }
    return 0;
}
