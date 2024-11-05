#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>

using namespace std;


class CustomTexture
{
    private:
        sf::Texture* texture;
        int originX;
        int originY;

    public:
        CustomTexture(string path, int originX, int originY)
        {
            this->originX = originX;
            this->originY = originY;
            this->texture = new sf::Texture();
            this->texture->loadFromFile(path);
        }

        int getOriginX()
        {
            return originX;
        }

        int getOriginY()
        {
            return originY;
        }

        sf::Texture* getTexture()
        {
            return texture;
        }
};


class CustomSprite
{
    protected:
        sf::Sprite* sprite;
        CustomTexture* texture;
        sf::Vector2f position;

    public:
        CustomSprite(CustomTexture* texture, int x, int y){

            this->texture = texture;

            this->sprite = new sf::Sprite(*texture->getTexture());

            this->position.x = x;
            this->position.y = y;

            this->sprite->setOrigin(texture->getOriginX(), texture->getOriginY());
            //sf::Vector2f increment(0.4f, 0.4f);
            this->sprite->setPosition(x, y);

        }

        sf::Sprite* getSprite(){

            return this->sprite;

        }

        void setPosition(int x, int y){

            this->position.x = x;
            this->position.y = y;
        }

        void move(double x, double y){

            this->position.x += x;
            this->position.y += y;

        }

        void update(){

            this->sprite->setPosition(position.x, position.y);

        }
};

class Grass : CustomSprite
{
    vector<Grass*>* grassVector;

    public:
        Grass(CustomTexture* texture, int x, int y, vector<Grass*>* grass) : CustomSprite(texture, x, y)
        {
            grassVector = grass;
        }

        void update()
        {
            grassVector->push_back(new Grass(texture, position.x, position.y, grassVector));
        }
};


int main()
{
    // Create window
    sf::RenderWindow window(sf::VideoMode(800, 450), "Simulation");
    sf::RectangleShape shape(sf::Vector2f(10.f, 10.f));
    shape.setFillColor(sf::Color::Yellow);

    // Load Textures
    CustomTexture* t_lemon = new CustomTexture("sprites\\lemon.png", 50, 50);
    CustomTexture* t_apple = new CustomTexture("sprites\\apple.png", 70, 50);

    // Declare Data Structures
    vector<Grass*> grasses{};

    // Create Sprites
    CustomSprite* apple = new CustomSprite(t_apple, 50, 50);
    Grass* grass = new Grass(t_apple, 50, 50, &grasses);

    vector<CustomSprite*> blocks{new CustomSprite(t_lemon, 50, 50), new CustomSprite(t_lemon, 100, 100)};

    // Main Loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            apple->move(-.1f, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            apple->move(.1f, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            apple->move(0, -.1f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            apple->move(0, .1f);
        }

        window.clear();

        apple->update();
        window.draw(*apple->getSprite());

        for(int i = 0; i < blocks.size(); i++)
        {
            window.draw(*blocks[i]->getSprite());
        }

        window.display();
    }

    return 0;
}