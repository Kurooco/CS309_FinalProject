#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include "Log.h"
#include "TextureAtlas.h"
#include "SpriteBatch.h"
using namespace std;
/*
    Current State:

        - Pass all necessary info into the object's constructor beforehand, so that all objects can be updated at the same time. Use
        GameState object for holding needed primitive types. 
*/

class CustomTexture
{
    private:
        string textureName;
        int originX;
        int originY;

    public:
        CustomTexture(string name, int originX, int originY)
        {
            this->originX = originX;
            this->originY = originY;
            this->textureName = name;
        }

        int getOriginX()
        {
            return originX;
        }

        int getOriginY()
        {
            return originY;
        }

        string getName()
        {
            return textureName;
        }
};

class SpritePresets
{
    /*
    * Holds the important information about the sprite's texture/animation and collision. These attributes are the same for all
    * sprites of the same type. (For example, all trees should have collision at the same place relative to their origin)
    */
    private:
        CustomTexture* texture;
        sf::Vector2i* topLeftCollision = nullptr;
        sf::Vector2i* bottomRightCollision = nullptr;
        bool isPointCollision;

    public:
        SpritePresets(CustomTexture* texture, sf::Vector2i* topLeftCollision, sf::Vector2i* bottomRightCollision)
        {
            this->texture = texture;
            this->topLeftCollision = topLeftCollision;
            this->bottomRightCollision = bottomRightCollision;
            this->isPointCollision = false;
        }

        SpritePresets(CustomTexture* texture)
        {
            this->texture = texture;
            this->isPointCollision = true;
        }

        int getOriginX()
        {
            return texture->getOriginX();
        }

        int getOriginY()
        {
            return texture->getOriginY();
        }

        string getTextureName()
        {
            return texture->getName();
        }

        sf::Vector2i* getTopLeftCollision()
        {
            return topLeftCollision;
        }

        sf::Vector2i* getBottomRightCollision()
        {
            return bottomRightCollision;
        }

        bool getIsPointCollision()
        {
            return isPointCollision;
        }
};

class CustomSprite
{
    protected:
        moony::Sprite* sprite;
        moony::TextureAtlas* textureAtlas;
        SpritePresets* presets;
        float xPosition;
        float yPosition;
        vector<CustomSprite*>* mapDetails;
        float width;
        float height;

    public:
        CustomSprite(vector<CustomSprite*>* mapDetails, moony::TextureAtlas* textureAtlas, SpritePresets* presets, int x, int y, int zIndex)
        {
            this->mapDetails = mapDetails;
            this->textureAtlas = textureAtlas;
            this->presets = presets;

            this->sprite = new moony::Sprite();
            sprite->m_subtexture = textureAtlas->findSubTexture(presets->getTextureName());
            sprite->m_layer = zIndex;

            this->width = sprite->m_subtexture.m_rect.width;
            this->height = sprite->m_subtexture.m_rect.height;

            this->xPosition = x;
            this->yPosition = y;

            sf::Vector2u size = textureAtlas->findSubTexture(presets->getTextureName()).m_texture->getSize();
            this->sprite->setOrigin(presets->getOriginX(), presets->getOriginY());
            sf::Vector2f increment(0.4f, 0.4f); // Why is this here?
            this->sprite->setPosition(x, y);
        }

        moony::Sprite* getSprite()
        {
            return this->sprite;
        }

        void setPosition(int x, int y)
        {
            this->xPosition = x;
            this->yPosition = y;
        }

        void move(double x, double y)
        {
            this->xPosition += x;
            this->yPosition += y;
        }

        float getX()
        {
            return xPosition;
        }

        float getY()
        {
            return yPosition;
        }

        virtual void update(float cameraX, float cameraY, float deltaTime)
        {
            this->sprite->setPosition(xPosition+cameraX, yPosition+cameraY);
        }

        static bool compare(CustomSprite* a, CustomSprite* b)
        {
            float centerA = 0;
            float centerB = 0;
            if(!a->presets->getIsPointCollision())
                centerA = (a->presets->getBottomRightCollision()->y + a->presets->getTopLeftCollision()->y) / 2;
            if(!b->presets->getIsPointCollision())
                centerB = (b->presets->getBottomRightCollision()->y + b->presets->getTopLeftCollision()->y) / 2;
            if(b->yPosition + centerB < a->yPosition + centerA) return false;
            if(b->yPosition + centerB > a->yPosition + centerA) return true;
            if(b->xPosition + centerB < a->xPosition + centerA) return false;
            if(b->xPosition + centerB > a->xPosition + centerA) return true;
            return false;
        }

        bool isOverlapping(CustomSprite* other)
        {
            if (other->presets->getIsPointCollision() && presets->getIsPointCollision())
                return false;
            else if(!other->presets->getIsPointCollision() && !presets->getIsPointCollision())
            {
                int xRightSelf = xPosition+presets->getBottomRightCollision()->x;
                int yUpSelf = yPosition+presets->getTopLeftCollision()->y;
                int xLeftSelf = xPosition+presets->getTopLeftCollision()->x;
                int yDownSelf = yPosition+presets->getBottomRightCollision()->y;
                int xRightOther = other->xPosition+other->presets->getBottomRightCollision()->x;
                int yUpOther = other->yPosition+other->presets->getTopLeftCollision()->y;
                int xLeftOther = other->xPosition+other->presets->getTopLeftCollision()->x;
                int yDownOther = other->yPosition+other->presets->getBottomRightCollision()->y;
                // TODO: needs to be adjusted for smaller/larger hitboxs
                if((xRightSelf > xLeftOther && xLeftSelf < xRightOther) && (yUpSelf < yDownOther && yDownSelf > yUpOther))
                    return true;
            }
            else if (other->presets->getIsPointCollision())
            {
                int xRightSelf = xPosition+presets->getBottomRightCollision()->x;
                int yUpSelf = yPosition+presets->getTopLeftCollision()->y;
                int xLeftSelf = xPosition+presets->getTopLeftCollision()->x;
                int yDownSelf = yPosition+presets->getBottomRightCollision()->y;
                if(other->xPosition < xRightSelf && other->xPosition > xLeftSelf && other->yPosition > yUpSelf && other->yPosition < yDownSelf)
                    return true;
            }
            else
            {
                int xRightOther = other->xPosition+other->presets->getBottomRightCollision()->x;
                int yUpOther = other->yPosition+other->presets->getTopLeftCollision()->y;
                int xLeftOther = other->xPosition+other->presets->getTopLeftCollision()->x;
                int yDownOther = other->yPosition+other->presets->getBottomRightCollision()->y;
                if(xPosition < xRightOther && xPosition > xLeftOther && yPosition > yUpOther && yPosition < yDownOther)
                    return true;
            }

            return false;
        }

        SpritePresets* getPresets()
        {
            return presets;
        }
};


//Forward declare textures and sounds
moony::TextureAtlas* texture_atlas = new moony::TextureAtlas();
moony::SpriteBatch sprite_batch;
moony::Sprite player1;
sf::Music music;
sf::Text text;
sf::Font font;
clock_t timer;
float deltaTime = 0.f;
float cameraX = 200;
float cameraY = 0;
float cameraVel = .3;

// Textures
CustomTexture t_grass("grassFall.png", 0, 0);
CustomTexture t_tree("treeFall7.png", 10, 30);
CustomTexture t_hero("pablo.png", 10, 33);

// Sprite Presets
SpritePresets p_grass(&t_grass);
SpritePresets p_tree(&t_tree, new sf::Vector2i(0, 0), new sf::Vector2i(50, 50));
SpritePresets p_hero(&t_hero);

int main()
{
    sf::RenderWindow window(sf::VideoMode(400, 400), "Game Test");

    vector<CustomSprite*> mapDetails{};
    int wait = 0;
    int count = 10;
    float vel = 0.f;
    texture_atlas->loadFromFile("C:\\Users\\DJsul\\OneDrive\\Desktop\\Asset Pack\\PaintFate\\PaintFate.mtpf");

    for(int r = 0; r < 2; r++)
    {
        for(int c = 0; c < 2; c++)
        {
            mapDetails.push_back(new CustomSprite(&mapDetails, texture_atlas, &p_grass, c*49, r*49, 0));
            if(rand() % 3 == 0)
                mapDetails.push_back(new CustomSprite(&mapDetails, texture_atlas, &p_tree, c*49, r*49, 1));
        }
    }
    
    CustomSprite* hero = new CustomSprite(&mapDetails, texture_atlas, &p_hero, 0, 0, 1);
    mapDetails.push_back(hero);

    font.loadFromFile("fonts\\Chocolada-Bold.ttf");
    text.setFont(font); 
    text.setString("Testing");
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::Blue);
    text.setPosition(10.f, 10.f);

    //Game Loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        timer = clock();
        window.clear();

        // Update Camera
        //cameraX += (hero->getX()*-1 + 200 - cameraX)*deltaTime/500;
        //cameraY += (hero->getY()*-1 + 200 - cameraY)*deltaTime/500;

        

        // Sort map
        sort(mapDetails.begin(), mapDetails.end(), CustomSprite::compare);

        // Update/Draw Sprites
        for(int i = 0; i < mapDetails.size(); i++)
        {
            mapDetails[i]->update(cameraX, cameraY, deltaTime);
            if(abs(mapDetails[i]->getX()*-1 + 200 - cameraX) < 400 && abs(mapDetails[i]->getY()*-1 + 200 - cameraY) < 400)
                sprite_batch.draw(*mapDetails[i]->getSprite());
        }
        
        sprite_batch.order();
        window.draw(sprite_batch);
        sprite_batch.clear();
        window.draw(text);

        window.display();

        deltaTime = (float)(clock() - timer);
    }

    return 0;
}
