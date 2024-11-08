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

        CustomSprite(CustomTexture* texture){

            this->texture = texture;

            this->sprite = new sf::Sprite(*texture->getTexture());

            this->position.x = 0;
            this->position.y = 0;

            this->sprite->setOrigin(texture->getOriginX(), texture->getOriginY());
            //sf::Vector2f increment(0.4f, 0.4f);
            this->sprite->setPosition(position.x, position.y);

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
    static Grass* locationArray[][20];

    private:
        const int REPRODUCE_ITER = 1000;
        int REPRODUCE_RAD = 20;
        const sf::Vector2i GRID_POSITION = sf::Vector2i(100, 100);

        vector<Grass*>* grassVector;
        int currentReproduceIter = 0;
        sf::Vector2i selfPosition;

    public:
        Grass(CustomTexture* texture, int x, int y, vector<Grass*>* grass, int indX, int indY) : CustomSprite{texture, x, y}
        {
            //this->texture = texture;
            //position = sf::Vector2f((x*REPRODUCE_RAD + GRID_POSITION.x), (y*REPRODUCE_RAD + GRID_POSITION.y));
            //printf("\nx: %d y: %d realx: %d realy: %d", position.x, position.y, x*REPRODUCE_RAD + GRID_POSITION.x, y*REPRODUCE_RAD + GRID_POSITION.y);
            
            //Grass::locationArray[(x-GRID_POSITION.x)/REPRODUCE_RAD][(y-GRID_POSITION.y)/REPRODUCE_RAD] = this; 
            //selfPosition = sf::Vector2i((x-GRID_POSITION.x)/REPRODUCE_RAD, (y-GRID_POSITION.y)/REPRODUCE_RAD);

            Grass::locationArray[indX][indY] = this; 
            selfPosition = sf::Vector2i(indX, indY);
        }


        void update()
        {
            currentReproduceIter++;
            currentReproduceIter %= REPRODUCE_ITER;
        }

        Grass* reproduce()
        {
            Grass* newGrass = nullptr;
            if(currentReproduceIter == REPRODUCE_ITER-1)
            {
                int dir = rand()%4;
                int x_pos = selfPosition.x;//(int)position.x;
                int y_pos = selfPosition.y;//(int)position.y;
                for(int i = 0; i < 4; i++)
                {
                    if(selfPosition.x < 20 && dir == 0 && locationArray[x_pos + 1][y_pos] == nullptr)
                    {
                        newGrass = new Grass(texture, position.x + REPRODUCE_RAD, position.y, grassVector, x_pos + 1, y_pos);
                        return newGrass;
                    }
                    if(selfPosition.x > 0 && dir == 1 && locationArray[x_pos - 1][y_pos] == nullptr)
                    {
                        newGrass = new Grass(texture, position.x - REPRODUCE_RAD, position.y, grassVector, x_pos - 1, y_pos);
                        return newGrass;
                    }
                    if(selfPosition.y < 20 && dir == 2 && locationArray[x_pos][y_pos + 1] == nullptr)
                    {
                        newGrass = new Grass(texture, position.x, position.y + REPRODUCE_RAD, grassVector, x_pos, y_pos + 1);
                        return newGrass;
                    }
                    if(selfPosition.y > 0 && dir == 3 && locationArray[x_pos][y_pos - 1] == nullptr)
                    {
                        newGrass = new Grass(texture, position.x, position.y - REPRODUCE_RAD, grassVector, x_pos, y_pos - 1);
                        return newGrass;
                    }
                    dir++;
                    dir %= 4;
                    //printf("%d", dir);
                }                
            }

            return newGrass;
        }


        /*
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
        }*/
};

// Declare underlying grass location array
Grass* Grass::locationArray[20][20] = {{nullptr}};

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
    grasses.reserve(400);

    // Create Sprites
    CustomSprite* apple = new CustomSprite(t_apple, 350, 200);
    grasses.push_back(new Grass(t_grass, 100, 100, &grasses, 0, 0));

    vector<CustomSprite*> blocks{/*new CustomSprite(t_lemon, 50, 50), new CustomSprite(t_lemon, 100, 100)*/};

    vector<char*> stuff = {};
        stuff.push_back((char*)"cool");
        stuff.push_back((char*)"look");

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
        //grass->update();
        
        window.draw(*apple->getSprite());
        //window.draw(*grass->getSprite());

        for(int i = 0; i < blocks.size(); i++)
        {
            window.draw(*blocks[i]->getSprite());
        }
        vector<Grass*>::size_type grass_num = grasses.size();
        
        //vector<char*>::size_type stuff_num = stuff.size();
        /*for(vector<Grass*>::size_type i = 0; i < grass_num; i++)
        {
            //printf(stuff[i]);
            //if(i == 1) stuff.push_back((char*)"apple");
            window.draw(*grasses[i]->getSprite());
            grasses[i]->update();
        }*/
        vector<Grass*>::size_type i = 0;
        while(i < grass_num)
        {
            window.draw(*grasses[i]->getSprite());
            grasses[i]->update();
            Grass* newGrass = grasses[i]->reproduce();
            if(newGrass != nullptr) grasses.push_back(newGrass);
            i++;
        }
        //printf("\n%d", grasses.size());
        window.display();
    }

    return 0;
}