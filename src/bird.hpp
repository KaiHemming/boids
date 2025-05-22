#ifndef BIRD_HPP
#define BIRD_HPP
#include <SFML/Graphics.hpp>

int BOUNDS_X = 900;
int BOUNDS_Y = 600;
int SIGHT = 100;
int SEPARATION_STEER = 5;
float ALIGNMENT_STEER = 0.1f;
int COHESION_STEER = 5;
float SPEED = 1.0f;

class Bird : public sf::Drawable {
    public:
        sf::CircleShape triangle;
        float speed;
        sf::Vector2f velocity;
        sf::Vector2f direction;

        Bird() {
            triangle = sf::CircleShape(10, 3);
            triangle.setFillColor(sf::Color(150, 50, 250));
            triangle.setOrigin(sf::Vector2f(10,10));
            speed = SPEED;
        }
        void setRotation(int rotation) {
            triangle.setRotation(sf::degrees(rotation));
            // orient tip and convert to radians
            float adjustedRotation = (rotation - 90) * 3.14159265f / 180.f;
            direction = sf::Vector2f(std::cos(adjustedRotation), std::sin(adjustedRotation));
            velocity = direction * speed;
        }
        void setPosition(float x, float y) {
            triangle.setPosition(sf::Vector2f(x, y));
        }
        std::list<Bird> getFlock(std::list<Bird> birds) {
            std::list<Bird> flock = {};
            // d = √[(x2 − x1)2 + (y2 − y1)2]
            sf::Vector2f pos = triangle.getPosition();
            for (Bird& bird : birds) {
                sf::Vector2f birdPos = bird.triangle.getPosition();
                float distance = 
                    sqrt(pow(birdPos.x - pos.x, 2) + pow(birdPos.y - pos.y,2));
                if (distance <= SIGHT) {
                    flock.push_back(bird);
                }
            }
            return flock;
        }
        void move(std::list<Bird> flock) {
            // Check flock
            sf::Vector2f alignmentTotal;
            for (Bird& bird: flock) {

                // Separation

                // Alignment
                alignmentTotal += bird.direction;

                // Cohesion

            }
            // Update direction based on flock alignment
            sf::Vector2f flockDirection = alignmentTotal / static_cast<float>(flock.size());
            direction = (direction * (1.0f - ALIGNMENT_STEER) + flockDirection * ALIGNMENT_STEER).normalized();
            velocity = direction * speed;

            // Bound checking - Wrap
            if (triangle.getPosition().x >= BOUNDS_X) setPosition(0, triangle.getPosition().y);
            else if (triangle.getPosition().x <= 0) setPosition(BOUNDS_X, triangle.getPosition().y);

            if (triangle.getPosition().y >= BOUNDS_Y) setPosition(triangle.getPosition().x, 0);
            else if (triangle.getPosition().y <= 0) setPosition(triangle.getPosition().x, BOUNDS_Y);

            // Bound checking - Bounce - this had weird behaviours
            // if (triangle.getPosition().x >= BOUNDS_X || triangle.getPosition().x <= 0) {
            //     direction.x = direction.x * -1;
            //     velocity.x = velocity.x * -1;
            // }
            // if (triangle.getPosition().y >= BOUNDS_Y || triangle.getPosition().y <= 0) {
            //     direction.y = direction.y * -1;
            //     velocity.y = velocity.y * -1;
            // }

            // Rotation
            float angleRadians = std::atan2(direction.y, direction.x);  // radians
            float angleDegrees = angleRadians * 180.f / 3.14159265f;
            triangle.setRotation(sf::degrees(angleDegrees + 90)); // tip forwards

            triangle.move(velocity);
        }
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            float lineLength = 30.0f;
            std::array line =
            {
                sf::Vertex{triangle.getPosition()},
                sf::Vertex{triangle.getPosition() + direction * 15.0f}
            };

            target.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
            target.draw(triangle, states);
        }
};

#endif