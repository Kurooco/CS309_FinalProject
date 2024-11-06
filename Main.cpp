#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <math.h>

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

class Grass : public CustomSprite
{
    private:
        vector<Grass*>* grassVector;
        const int REPRODUCE_ITER = 500;
        const int REPRODUCE_RAD = 10;
        int currentReproduceIter = 0;

    public:
        Grass(CustomTexture* texture, int x, int y, vector<Grass*>* grass) : CustomSprite(texture, x, y)
        {
            grassVector = grass;
        }
        
        void update()
        {
            currentReproduceIter++;
            if(currentReproduceIter >= REPRODUCE_ITER)
            {
                if(getGrassesInRange() < 5)
                {
                    float rand_x = (position.x + REPRODUCE_RAD)-((rand()%REPRODUCE_RAD)*2);
                    float rand_y = (position.y + REPRODUCE_RAD)-((rand()%REPRODUCE_RAD)*2);
                    grassVector->push_back(new Grass(texture, rand_x, rand_y, grassVector));
                }
                currentReproduceIter = 0;
            }
        }

        int getGrassesInRange()
        {
            int count = 0;
            int distance = 0;
            int x_dif = 0;
            int y_dif = 0;
            for(int i = 0; i < grassVector->size(); i++)
            {
                x_dif = ((*grassVector)[i]->position.x - position.x);
                y_dif = ((*grassVector)[i]->position.y - position.y);
                distance = sqrt(x_dif*x_dif + y_dif*y_dif);
                if(distance < REPRODUCE_RAD) count++;
            }
            return count;
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
    CustomTexture* t_grass = new CustomTexture("sprites\\grass.png", 10, 10);

    // Declare Data Structures
    vector<Grass*> grasses{};

    // Create Sprites
    CustomSprite* apple = new CustomSprite(t_apple, 50, 50);
    Grass* grass = new Grass(t_grass, 200, 200, &grasses);

    vector<CustomSprite*> blocks{/*new CustomSprite(t_lemon, 50, 50), new CustomSprite(t_lemon, 100, 100)*/};

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
        grass->update();
        
        window.draw(*apple->getSprite());
        window.draw(*grass->getSprite());

        for(int i = 0; i < blocks.size(); i++)
        {
            window.draw(*blocks[i]->getSprite());
        }
        for(int i = 0; i < grasses.size(); i++)
        {
            grasses[i]->update();
            window.draw(*grasses[i]->getSprite());
        }

        window.display();
    }

    return 0;
}