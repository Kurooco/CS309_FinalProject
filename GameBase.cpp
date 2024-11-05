#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
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

//Sprite only stores a pointer to texture. As soon as texture is destroyed, sprite is without a texture to refer to


//CURRENT TASK: Inheritance of constructors

class CustomSprite{
    protected:
        sf::Sprite* sprite;
        sf::Texture texture;
        double xPosition;
        double yPosition;

    public:
        CustomSprite(sf::Texture texture, int x, int y){

            this->texture = texture;

            this->sprite = new sf::Sprite(this->texture);

            this->xPosition = x;
            this->yPosition = y;

            sf::Vector2u size = this->texture.getSize();
            this->sprite->setOrigin(size.x / 2, size.y / 2);
            sf::Vector2f increment(0.4f, 0.4f);
            this->sprite->setPosition(x, y);

        }

        sf::Sprite* getSprite(){

            return this->sprite;

        }

        void setPosition(int x, int y){

            this->xPosition = x;
            this->yPosition = y;

        }

        void move(double x, double y){

            this->xPosition += x;
            this->yPosition += y;

        }

        void update(){

            this->sprite->setPosition(xPosition, yPosition);

        }


};

//Forward declare textures
sf::Texture t_apple;
sf::Texture t_lemon;
sf::Texture t_block;


int main()
{
    sf::RenderWindow window(sf::VideoMode(400, 400), "Game Test");
    sf::RectangleShape shape(sf::Vector2f(10.f, 10.f));
    shape.setFillColor(sf::Color::Yellow);

    //LOAD TEXTURES
    t_apple.loadFromFile("sprites\\apple.png"); //Make program that automates naming/adding pics
    t_lemon.loadFromFile("sprites\\lemon.png");
    t_block.loadFromFile("sprites\\block.png");


    //Create Sprites
    CustomSprite* apple = new CustomSprite(t_block, 50, 50);

    vector<CustomSprite*> blocks{new CustomSprite(t_block, 50, 50), new CustomSprite(t_block, 100, 100)};


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

        for(int i = 0; i < 2; i++){//sizeof(blocks) / sizeof(CustomSprite*)
            window.draw(*blocks[i]->getSprite());
        }

        window.display();
    }

    return 0;
}