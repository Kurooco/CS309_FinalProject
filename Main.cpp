#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>

using namespace std;
//Must add sfml path to c_cpp_properties.json

// g++ -c main.cpp -I"C:\Users\DJsul\OneDrive\Documents\libraries\SFML-2.5.1\include"
// g++ main.o -o Main -L"C:\Users\DJsul\OneDrive\Documents\libraries\SFML-2.5.1\lib" -lsfml-graphics -lsfml-window -lsfml-system

//g++ -c main.cpp -I"C:\Users\DJsul\OneDrive\Documents\libraries\SFML-2.5.1\include" -DSFML_STATIC
//g++ main.o -o Main -L"C:\Users\DJsul\OneDrive\Documents\libraries\SFML-2.5.1\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows

//To "make clean", just delete the object and executable file

// Circle
//sf::CircleShape shape(100.f);
// Rectangle
//sf::RectangleShape shape(sf::Vector2f(120.f, 50.f));
// Fill
//shape.setFillColor(sf::Color::Yellow);
//Position
//.setPosition(10.f, 50.f);

// -mwindows removes console

//Sprite only stores a pointer to texture. As soon as texture is destroyed, sprite is without a texture to refer to

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
        CustomSprite(int x, int y, CustomTexture* texture){

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

//Forward declare textures and sounds
sf::Texture t_apple;
CustomTexture* t_lemon;




int main()
{
    //Play music
    /*if (!music.openFromFile("sfx/mainSong.wav"))
       return -1; // error
    music.setVolume(100.f);
    music.play();
    music.setLoop(true);*/

    // Create window
    sf::RenderWindow window(sf::VideoMode(400, 400), "Game Test");
    sf::RectangleShape shape(sf::Vector2f(10.f, 10.f));
    shape.setFillColor(sf::Color::Yellow);

    //LOAD TEXTURES
    //t_apple.loadFromFile("sprites\\apple.png"); //Make program that automates naming/adding pics
    //t_lemon.loadFromFile("sprites\\lemon.png");
    t_lemon = new CustomTexture("sprites\\lemon.png", 50, 50);
   // SpritePresets* p_apple = new SpritePresets();

    //Create Sprites
    CustomSprite* apple = new CustomSprite(50, 50, t_lemon);

    vector<CustomSprite*> blocks{/*new CustomSprite(t_apple, 50, 50), new CustomSprite(t_apple, 100, 100)*/};

    /* Excellent! It seems to work fine.
    #pragma omp parallel for num_threads(4)
    for(int i = 0; i < 100; i++)
    {
        long id = omp_get_thread_num();
        printf("\n%d from %ld", i, id);
    }*/

    //Game Loop
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