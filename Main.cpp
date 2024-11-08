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

        void move(float x, float y){

            this->position.x += x;
            this->position.y += y;

        }

        void update(){

            this->sprite->setPosition(position.x, position.y);

        }

        sf::Vector2f getPosition()
        {
            return position;
        }
};

class Grass : public CustomSprite
{
    public:
        static const int boardDimX = 70;
        static const int boardDimY = 40;
        static const int REPRODUCE_ITER = 1000;
        static const int REPRODUCE_RAD = 20;
        static Grass* locationArray[][boardDimY+1];

    private:
        
        //const sf::Vector2i GRID_POSITION = sf::Vector2i(100, 100);

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


        bool update()
        {
            currentReproduceIter = rand()%REPRODUCE_ITER;
            if(currentReproduceIter == 0)
            {
                //currentReproduceIter %= REPRODUCE_ITER;
                return true;
            }
            return false;
            /*currentReproduceIter += rand()%(REPRODUCE_ITER/50);
            if(currentReproduceIter > REPRODUCE_ITER)
            {
                currentReproduceIter %= REPRODUCE_ITER;
                return true;
            }
            return false;*/
        }

        Grass* reproduce()
        {
            Grass* newGrass = nullptr;
            if(true)
            {
                int dir = rand()%4;
                int x_pos = selfPosition.x;//(int)position.x;
                int y_pos = selfPosition.y;//(int)position.y;
                for(int i = 0; i < 4; i++)
                {
                    if(selfPosition.x < boardDimX && dir == 0 && locationArray[x_pos + 1][y_pos] == nullptr)
                    {
                        newGrass = new Grass(texture, position.x + REPRODUCE_RAD, position.y, grassVector, x_pos + 1, y_pos);
                        return newGrass;
                    }
                    if(selfPosition.x > 0 && dir == 1 && locationArray[x_pos - 1][y_pos] == nullptr)
                    {
                        newGrass = new Grass(texture, position.x - REPRODUCE_RAD, position.y, grassVector, x_pos - 1, y_pos);
                        return newGrass;
                    }
                    if(selfPosition.y < boardDimY && dir == 2 && locationArray[x_pos][y_pos + 1] == nullptr)
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

        void eat()
        {
            locationArray[selfPosition.x][selfPosition.y] = nullptr;
        }
};

class Cow : public CustomSprite
{
    public:
        static const int REPRODUCE_ITER = 1000;
        static const int MAX_FOOD = 1000;
        static const int FOOD_NEEDED_TO_REPRODUCE = 500;

    private:
        vector<Grass*>* grassVector;
        int currentReproduceIter;
        const float SPEED = .1;
        int food;

    public:
        Cow(CustomTexture* texture, int x, int y, vector<Grass*>* grass) : CustomSprite{texture, x, y}
        {
            grassVector = grass;
            currentReproduceIter = 0;
            food = MAX_FOOD;
        }

        bool update()
        {
            food--;
            float minDist = INT_MAX;
            sf::Vector2f grassPosition;
            int xDist = 0;
            int yDist = 0;
            int grassInd = 0;
            for(int i = 0; i < grassVector->size(); i++)
            {
                xDist = (*grassVector)[i]->getPosition().x - position.x;
                yDist = (*grassVector)[i]->getPosition().y - position.y;
                float distance = sqrt(xDist * xDist + yDist * yDist);
                if(distance < minDist)
                {
                    minDist = distance;
                    grassPosition = (*grassVector)[i]->getPosition();
                    grassInd = i;
                }
            }
            if(minDist > 5)
            {
                float xMove = ((grassPosition.x - position.x)/minDist)*SPEED;
                float yMove = ((grassPosition.y - position.y)/minDist)*SPEED;
                move(xMove, yMove);
            }
            else
            {
                (*grassVector)[grassInd]->eat();
                grassVector->erase(grassVector->begin() + grassInd);
                food = MAX_FOOD;
            }
            this->sprite->setPosition(position.x, position.y);

            // reproduce?
            if(rand()%REPRODUCE_ITER == 0 && food >= FOOD_NEEDED_TO_REPRODUCE)
                return true;
            else   
                return false;
        }       

        Cow* reproduce()
        {
            return new Cow(texture, position.x + 50, position.y + 50, grassVector);
        }

        bool hasStarved()
        {
            return food <= 0;
        }
};

// Declare underlying grass location array
Grass* Grass::locationArray[Grass::boardDimX+1][Grass::boardDimY+1] = {{nullptr}};

int main()
{
    // Create window
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Simulation");
    sf::RectangleShape shape(sf::Vector2f(10.f, 10.f));
    shape.setFillColor(sf::Color::Yellow);

    // Load Textures
    CustomTexture* t_lemon = new CustomTexture("sprites\\lemon.png", 50, 50);
    CustomTexture* t_apple = new CustomTexture("sprites\\apple.png", 70, 50);
    CustomTexture* t_grass = new CustomTexture("sprites\\grass.png", 10, 10);
    CustomTexture* t_cow = new CustomTexture("sprites\\cow.png", 10, 10);

    // Declare Data Structures
    vector<Grass*> grasses{};
    grasses.reserve(400);
    vector<Cow*> cows{};
    cows.reserve(200);

    // Create Sprites
    CustomSprite* apple = new CustomSprite(t_apple, 350, 200);
    for(int i = 0; i < 20; i++)
    {
        int place_x = rand()%Grass::boardDimX;
        int place_y = rand()%Grass::boardDimY;
        if(Grass::locationArray[place_x][place_y] == nullptr)
            grasses.push_back(new Grass(t_grass, place_x*Grass::REPRODUCE_RAD, place_y*Grass::REPRODUCE_RAD, &grasses, place_x, place_y));
    }

    cows.push_back(new Cow(t_cow, 100, 100, &grasses));



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
        
        //window.draw(*apple->getSprite());
        //window.draw(*grass->getSprite());


        vector<Grass*>::size_type grass_num = grasses.size();
        vector<Grass*>::size_type i = 0;
        while(i < grass_num)
        {
            window.draw(*grasses[i]->getSprite());
            Grass* newGrass;
            if(grasses[i]->update())
            {
                newGrass = grasses[i]->reproduce();
                if(newGrass != nullptr) grasses.push_back(newGrass);
            }
            i++;
        }

        for(int i = 0; i < cows.size(); i++)
        {
            window.draw(*cows[i]->getSprite());
            Cow* newCow;
            if(cows[i]->update())
            {
                newCow = cows[i]->reproduce();
                cows.push_back(newCow);
            }
            if(cows[i]->hasStarved())
            {
                cows.erase(cows.begin() + i);
            }
        }

        window.display();
    }

    return 0;
}