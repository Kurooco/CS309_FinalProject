#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <ctime>
#include <chrono>
#include <boost/container/vector.hpp>

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
        CustomSprite(CustomTexture* texture, int x, int y)
        {
            this->texture = texture;

            this->sprite = new sf::Sprite(*texture->getTexture());

            this->position.x = x;
            this->position.y = y;

            this->sprite->setOrigin(texture->getOriginX(), texture->getOriginY());
            this->sprite->setPosition(x, y);
        }

        CustomSprite(CustomTexture* texture)
        {
            this->texture = texture;
            this->sprite = new sf::Sprite(*texture->getTexture());

            this->position.x = 0;
            this->position.y = 0;

            this->sprite->setOrigin(texture->getOriginX(), texture->getOriginY());
            this->sprite->setPosition(position.x, position.y);
        }

        void setTexture(CustomTexture* texture)
        {
            sprite->setTexture(*texture->getTexture());
            this->sprite->setOrigin(texture->getOriginX(), texture->getOriginY());
        }

        sf::Sprite* getSprite()
        {
            return this->sprite;
        }

        void setPosition(int x, int y)
        {
            this->position.x = x;
            this->position.y = y;
        }

        void move(float x, float y)
        {
            this->position.x += x;
            this->position.y += y;
        }

        void update()
        {
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
        static const int RECOVER_ITER = 1500;
        static const int REPRODUCE_RAD = 20;
        static Grass* locationArray[][boardDimY+1];

    private:
        boost::container::vector<Grass*>* grassVector;
        int currentReproduceIter = 0;
        sf::Vector2i selfPosition;
        bool degraded;
        CustomTexture* degradedTexture;

    public:
        Grass(CustomTexture* texture, CustomTexture* degradedTexture, int x, int y, boost::container::vector<Grass*>* grass, int indX, int indY) : CustomSprite{texture, x, y}
        {
            srand(x);
            Grass::locationArray[indX][indY] = this; 
            selfPosition = sf::Vector2i(indX, indY);
            degraded = false;
            this->degradedTexture = degradedTexture;
        }


        bool update()
        {
            if(degraded)
                currentReproduceIter = rand()%REPRODUCE_ITER;
            else
                currentReproduceIter = rand()%RECOVER_ITER;
            if(currentReproduceIter == 0)
            {
                return true;
            }
            return false;
        }

        Grass* reproduce()
        {
            if(isDegraded())
            {
                becomeHighQuality();
                return nullptr;
            }

            Grass* newGrass = nullptr;
            int dir = rand()%4;
            int x_pos = selfPosition.x;
            int y_pos = selfPosition.y;
            for(int i = 0; i < 4; i++)
            {
                if(selfPosition.x < boardDimX && dir == 0 && locationArray[x_pos + 1][y_pos] == nullptr)
                {
                    newGrass = new Grass(texture, degradedTexture, position.x + REPRODUCE_RAD, position.y, grassVector, x_pos + 1, y_pos);
                    return newGrass;
                }
                if(selfPosition.x > 0 && dir == 1 && locationArray[x_pos - 1][y_pos] == nullptr)
                {
                    newGrass = new Grass(texture, degradedTexture, position.x - REPRODUCE_RAD, position.y, grassVector, x_pos - 1, y_pos);
                    return newGrass;
                }
                if(selfPosition.y < boardDimY && dir == 2 && locationArray[x_pos][y_pos + 1] == nullptr)
                {
                    newGrass = new Grass(texture, degradedTexture, position.x, position.y + REPRODUCE_RAD, grassVector, x_pos, y_pos + 1);
                    return newGrass;
                }
                if(selfPosition.y > 0 && dir == 3 && locationArray[x_pos][y_pos - 1] == nullptr)
                {
                    newGrass = new Grass(texture, degradedTexture, position.x, position.y - REPRODUCE_RAD, grassVector, x_pos, y_pos - 1);
                    return newGrass;
                }
                dir++;
                dir %= 4;                
            }
            return newGrass;
        }

        // Returns true if the plant is eaten whole
        bool eat()
        {
            if(degraded)
            {
                locationArray[selfPosition.x][selfPosition.y] = nullptr;
                return true;
            }
            else
            {
                becomeDegraded();
                return false;
            }
        }

        bool isDegraded()
        {
            return degraded;
        }

        void becomeDegraded()
        {
            degraded = true;
            setTexture(degradedTexture);
        }

        void becomeHighQuality()
        {
            degraded = false;
            setTexture(texture);
        }
};

class Cow : public CustomSprite
{
    public:
        static const int REPRODUCE_ITER = 1000;
        static const int MAX_FOOD = 1500;
        static const int FOOD_NEEDED_TO_REPRODUCE = 750;
        static const int EYESIGHT = 100;
        static const int DESPERATION_THRESHOLD = 500;

    private:
        boost::container::vector<Grass*>* grassVector;
        int currentReproduceIter;
        const float SPEED = .1;
        int food;
        sf::Vector2i searchLocation;

    public:
        Cow(CustomTexture* texture, int x, int y, boost::container::vector<Grass*>* grass) : CustomSprite{texture, x, y}
        {
            srand(x);
            grassVector = grass;
            currentReproduceIter = 0;
            food = MAX_FOOD/2;
            searchLocation = sf::Vector2i(rand()%1600, rand()%900);
        }

        bool update()
        {
            food--;
            float minDist = INT_MAX;
            sf::Vector2f grassPosition;
            int xDist = 0;
            int yDist = 0;
            int grassInd = 0;
            //printf("\n%f", minDist);
            for(int i = 0; i < grassVector->size(); i++)
            {
                // Satisfied cows pick over the less preferable grass
                if(food > DESPERATION_THRESHOLD && (*grassVector)[i]->isDegraded()) continue; 

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

            // If food is not in sight, go to random part of map in search of food
            if(minDist > EYESIGHT)
            {
                xDist = searchLocation.x - position.x;
                yDist = searchLocation.y - position.y;
                float distance = sqrt(xDist * xDist + yDist * yDist);
                if(distance < 5)
                    searchLocation = sf::Vector2i(rand()%1600, rand()%900);
                else
                {
                    float xMove = (xDist/distance)*SPEED;
                    float yMove = (yDist/distance)*SPEED;
                    move(xMove, yMove);
                }
            }
            // Can see food, but not close enough to eat
            else if(minDist > 5)
            {
                float xMove = ((grassPosition.x - position.x)/minDist)*SPEED;
                float yMove = ((grassPosition.y - position.y)/minDist)*SPEED;
                move(xMove, yMove);
            }
            // Can eat food
            else
            {
                bool eatenDegraded = (*grassVector)[grassInd]->eat();
                if(eatenDegraded)
                {
                    food += (int)(MAX_FOOD*0.5f); // Food is not as nourishing
                    delete (*grassVector)[grassInd];
                    grassVector->erase(grassVector->begin() + grassInd);
                }
                else
                    food += (int)(MAX_FOOD*0.8f); // Food is very good (Hasn't been touched)
                if(food > MAX_FOOD) food = MAX_FOOD;
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
            return new Cow(texture, position.x - 50 + rand()%100, position.y - 50 + rand()%100, grassVector);
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

    //boost::container::vector<string> coolbeans{};
    
    // Create window
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Simulation");

    // Load Textures
    CustomTexture* t_lemon = new CustomTexture("sprites\\lemon.png", 50, 50);
    CustomTexture* t_apple = new CustomTexture("sprites\\apple.png", 70, 50);
    CustomTexture* t_grass = new CustomTexture("sprites\\grass.png", 10, 10);
    CustomTexture* t_grass_degraded = new CustomTexture("sprites\\bad_grass.png", 10, 10);
    CustomTexture* t_cow = new CustomTexture("sprites\\cow.png", 10, 10);

    // Declare Data Structures and other states
    const int SIM_ITER = 10000; //8000
    const int BIRD_ITER = 4000;
    int bird = 0;

    boost::container::vector<Grass*> grasses{};
    grasses.reserve(400);
    boost::container::vector<Cow*> cows{};
    cows.reserve(200);
    vector<int> grassPopulation{};
    grassPopulation.reserve(SIM_ITER);
    vector<int> cowPopulation{};
    cowPopulation.reserve(SIM_ITER);

    
    // Random seed
    srand(5);

    // Create sprites
    for(int i = 0; i < 40; i++)
    {
        int place_x = rand()%Grass::boardDimX;
        int place_y = rand()%Grass::boardDimY;
        if(Grass::locationArray[place_x][place_y] == nullptr)
            grasses.push_back(new Grass(t_grass, t_grass_degraded, place_x*Grass::REPRODUCE_RAD, place_y*Grass::REPRODUCE_RAD, &grasses, place_x, place_y));
    }

    for(int i = 0; i < 10; i++)
        cows.push_back(new Cow(t_cow, rand()%1000, rand()%800, &grasses));

    // Start timer
    std::chrono::time_point startc = chrono::system_clock::now();

    // Main loop
    int iter = 0;
    while (window.isOpen() && iter < SIM_ITER)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear window
        window.clear();

        // Update/draw grass
        int grass_num = grasses.size();
        int i = 0;
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

        // Birds (Dropping seeds in random places)
        bird++;
        if(bird >= BIRD_ITER)
        {
            int place_x = rand()%Grass::boardDimX;
            int place_y = rand()%Grass::boardDimY;
            if(Grass::locationArray[place_x][place_y] == nullptr)
                grasses.push_back(new Grass(t_grass, t_grass_degraded, place_x*Grass::REPRODUCE_RAD, place_y*Grass::REPRODUCE_RAD, &grasses, place_x, place_y)); 
            bird = 0;
        }


        // Update/draw cows
        for(int i = 0; i < cows.size(); i++)
        {
            window.draw(*cows[i]->getSprite());
        }
        for(int i = 0; i < cows.size(); i++)
        {
            //window.draw(*cows[i]->getSprite());
            Cow* newCow;
            if(cows[i]->update())
            {
                newCow = cows[i]->reproduce();
                cows.push_back(newCow);
            }
            if(cows[i]->hasStarved())
            {
                delete cows[i];
                cows.erase(cows.begin() + i);
            }
        }

        window.display();

        grassPopulation[iter] = grasses.size();
        cowPopulation[iter] = cows.size();
        iter++;
    } // End of main loop

    // Print time difference
    std::chrono::time_point endc = chrono::system_clock::now();
    const auto end_mills = chrono::duration_cast<std::chrono::milliseconds>(endc.time_since_epoch()).count();
    const auto mills = chrono::duration_cast<std::chrono::milliseconds>(startc.time_since_epoch()).count();
    printf("Time: %lf\n", (end_mills-mills)/1000.f);

    // Draw the stats 
    // https://www.sfml-dev.org/tutorials/2.6/graphics-shape.php
    window.clear();
    sf::Vertex grassLine[1600]{};
    sf::Vertex cowLine[1600]{};
    int forward = SIM_ITER/1600;
    for(int i = 0; i < 1600; i++)
    {
        grassLine[i] = sf::Vertex(sf::Vector2f(i, 800-(grassPopulation[forward*i])/3), sf::Color::Green);
        cowLine[i] = sf::Vertex(sf::Vector2f(i, 800-(cowPopulation[forward*i])/3), sf::Color::White);
        if(i%100 == 0) printf("%d, ", grassPopulation[forward*i]+cowPopulation[forward*i]);
    }
    window.draw(grassLine, 1600, sf::Lines);
    window.draw(cowLine, 1600, sf::Lines);

    window.display();

    while (window.isOpen())
    {
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

    }

    return 0;
}