#include <SFML/Graphics.hpp>
#include<iostream>
#include"SFML/Window.hpp"
#include"SFML/System.hpp"
#include<cstdlib>

using namespace sf;

void rexMove(Sprite& rex, RenderWindow& window, Texture& rexTexL, Texture& rexTexP);
void scoreLabel(int& timer, RenderWindow& window, Font& font);
void gameOver(Sprite& rex, RenderWindow& window, Font& font, Texture& deadRexTex);

int main()
{
    srand(time(NULL));

    RenderWindow window(VideoMode(1600, 900), "Rex dodge");
    window.setFramerateLimit(60);


    Font font;

    if (!font.loadFromFile("..\\rexTekstury\\StoneAge-OV4AO.ttf"))
    {
        std::cerr << "Nie udalo sie wczytac czcionki :(" << std::endl;
    }

    //Trex
    Texture rexTexL;
    Texture rexTexP;
    Sprite rex;

    if (!rexTexP.loadFromFile("..\\rexTekstury\\trexp.png"))
        std::cout << "Wtopa, tekstura trexa (p) siê nie za³adowa³a... :(";
    if (!rexTexL.loadFromFile("..\\rexTekstury\\trexl.png"))
        std::cout << "Wtopa, tekstura trexa (l) siê nie za³adowa³a... :(";

    Texture deadRexTex;

    if (!deadRexTex.loadFromFile("..\\rexTekstury\\martwytrexp.png"))
        std::cout << "Wtopa, tekstura martwego trexa (p) siê nie za³adowa³a... :(";

    rex.setTexture(rexTexP);
    rex.setScale(Vector2f(0.75f, 0.75f));
    rex.setPosition((window.getSize().x - rex.getGlobalBounds().width) / 2, window.getSize().y - rex.getGlobalBounds().height);

    //Meteory
    Texture meteorTex;
    Sprite meteor;
    int speed = 5;

    if (!meteorTex.loadFromFile("..\\rexTekstury\\meteor.png"))
        std::cout << "Wtopa, tekstura meteor siê nie za³adowa³a... :(";

    meteor.setTexture(meteorTex);
    meteor.setScale(Vector2f(0.1f, 0.1f));
    int meteorSpawnTimer = 45;
    int framesBetweenMeteors = 45;

    std::vector<Sprite> meteors;
    meteors.push_back(Sprite(meteor));

    //Mieso
    Texture meatTex;
    Sprite meat;

    if (!meatTex.loadFromFile("..\\rexTekstury\\mieso.png"))
        std::cout << "Wtopa, tekstura miesa siê nie za³adowa³a... :(";

    meat.setTexture(meatTex);
    meat.setScale(Vector2f(0.1f, 0.1f));
    int meatSpawnTimer = 400;

    std::vector<Sprite> meats;


    //T³o
    Sprite backgroundSprite;
    Texture background;

    if (!background.loadFromFile("..\\rexTekstury\\dinotlo.jpeg"))
        std::cout << "Wtopa, tekstura t³a siê nie za³adowa³a... :(";

    backgroundSprite.setTexture(background);
    backgroundSprite.setPosition(0, 0);
    backgroundSprite.setScale(1.6, 1.8);

    //Hp bar
    int hp = 100;
    int regenTimer = 0;

    Text hpText;
    hpText.setFont(font);
    hpText.setCharacterSize(30);
    hpText.setString(std::to_string(hp) + "%");
    hpText.setFillColor(Color::White);
    hpText.setOutlineThickness(5);
    hpText.setOutlineColor(Color::Red);

    RectangleShape hpBar;
    hpBar.setFillColor(Color::Red);

    hpBar.setPosition(10.f, 10.f);

    //Wynik
    int timer = 0;
    int frameCount = 0;


    //Pêtla gry
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {

            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                window.close();

        }

        if (hp > 0)
        {
            //UPDATE
            frameCount++;
            if (frameCount >= 60)
            {
                timer++;
                frameCount = 0;

                std::cout <<"Timer: " << timer << " / FramesPerMeteor:" << framesBetweenMeteors << " / MeteorSpeed:" << speed << std::endl;

                if (timer > 5 && speed < 12)
                {
                    speed++;
                }
                if (timer > 5 && framesBetweenMeteors > 10)
                {
                    framesBetweenMeteors--;
                }
            }

            //trex
            rexMove(rex, window, rexTexL, rexTexP);

            //meteory
            for (size_t i = 0; i < meteors.size(); i++)
            {
                meteors[i].move(0.f, float(speed));

                if (meteors[i].getPosition().y < 0 - meteor.getGlobalBounds().height)
                {
                    meteors.erase(meteors.begin() + i);
                }
            }

            if (meteorSpawnTimer < framesBetweenMeteors)
                meteorSpawnTimer++;
            else
            {
                meteor.setPosition(rand() % int(window.getSize().x - meteor.getGlobalBounds().width), 0);
                meteors.push_back(Sprite(meteor));
                meteorSpawnTimer = 0;
            }

            //mieso
            if (meatSpawnTimer < 400)
                meatSpawnTimer++;
            else
            {
                meat.setPosition(rand() % int(window.getSize().x - meat.getGlobalBounds().width), int(window.getSize().y - meat.getGlobalBounds().height - 30));
                meats.push_back(Sprite(meat));
                meatSpawnTimer = 0;
            }

            //hpbar
            if (hp < 100)
            {
                regenTimer++;
                if (regenTimer >= 30)
                {
                    hp++;
                    regenTimer = 0;
                }
            }

            //KOLIZJE

            FloatRect rexHitbox = rex.getGlobalBounds();
            rexHitbox.left += 50;
            rexHitbox.width -= 100;
            rexHitbox.top += 50;
            rexHitbox.height -= 50;


            for (size_t i = 0; i < meteors.size(); i++)
            {
                if (rexHitbox.intersects(meteors[i].getGlobalBounds()))
                {
                    hp -= 20;
                    meteors.erase(meteors.begin() + i);
                }
            }
            for (size_t i = 0; i < meats.size(); i++)
            {
                if (rexHitbox.intersects(meats[i].getGlobalBounds()) && hp < 100)
                {
                    if (hp <= 60)
                        hp += 40;
                    else
                        hp = 100;
                    meats.erase(meats.begin());
                }
            }
        }
        //DRAW
        window.clear();

        window.draw(backgroundSprite);


        for (size_t i = 0; i < meteors.size(); i++)
        {
            window.draw(meteors[i]);
        }
        for (size_t i = 0; i < meats.size(); i++)
        {
            window.draw(meats[i]);
        }

        hpBar.setSize(Vector2f((float)hp * float(7.9), 20.f));
        window.draw(hpBar);
        window.draw(rex);
        hpText.setString(std::to_string(hp) + "%");
        hpText.setPosition((float)hp * float(7.9) + 15, 10);
        window.draw(hpText);

        scoreLabel(timer, window, font);

        if (hp <= 0)
        {
            gameOver(rex, window, font, deadRexTex);
        }

        window.display();
    }

    return 0;
}

void rexMove(Sprite& rex, RenderWindow& window, Texture& rexTexL, Texture& rexTexP)
{
    if (Keyboard::isKeyPressed(Keyboard::A) && rex.getPosition().x > 0)
    {
        rex.move(-10.f, 0);
        rex.setTexture(rexTexL);
    }

    if (Keyboard::isKeyPressed(Keyboard::D) && rex.getPosition().x < window.getSize().x - rex.getGlobalBounds().width)
    {
        rex.move(10.f, 0);
        rex.setTexture(rexTexP);
    }
}

void scoreLabel(int& timer, RenderWindow& window, Font& font)
{


    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(70);
    scoreText.setString("WYNIK: " + std::to_string(timer));
    scoreText.setFillColor(Color::White);
    scoreText.setOutlineThickness(5);
    scoreText.setOutlineColor(Color::Black);

    scoreText.setPosition(window.getSize().x - 470.f, 0);
    window.draw(scoreText);
}

void gameOver(Sprite& rex, RenderWindow& window, Font& font, Texture& deadRexTex)
{


    rex.setTexture(deadRexTex);
    rex.setScale(Vector2f(0.85f, 0.85f));

    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(200);
    gameOverText.setString("KONIEC\n  GRY ");
    gameOverText.setFillColor(Color::White);
    gameOverText.setOutlineThickness(20);
    gameOverText.setOutlineColor(Color::Black);

    gameOverText.setPosition(400, 200);
    window.draw(gameOverText);


}