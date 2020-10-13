// This line is notes

#include "pch.h" //# means directive... used to add another codes to use in our code (just like library?)
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp> //including important C++ libraries
#include <SFML/Audio.hpp>

using namespace sf;

//function declaration
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
//where is the player/branch? left or right
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main() //there's only one main function for one executable file
{
    //std::cout << "Hello World!\n";
    //return 3;

    //create a video mode object
    VideoMode vm(1920, 1080);

    //create and open a window for the game
    RenderWindow window(vm, "Timber!!", Style::Fullscreen); //3 instances used

    //create a texture to hold a graphic on the GPU
    Texture textureBackground;

    //load a graphic into the texture
    textureBackground.loadFromFile("graphics/background.png");

    //create a sprite
    Sprite spriteBackground;

    //attach the texture to the sprite
    spriteBackground.setTexture(textureBackground);

    //set the sprite background to cover the screen
    spriteBackground.setPosition(0, 0);

    //make a tree sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    //prepare a bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/insect.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    //Is the bee flying?
    bool beeActive = false;
    //How fast is it flying?
    float beeSpeed = 0.0f; //pixels per sec

    //make 3 clouds sprites from 1 texture
    Texture textureCloud;
    //load 1 new textrue
    textureCloud.loadFromFile("graphics/cloud.png");
    //make 3 cloud sprites and set texture
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    //position them
    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(500, 50);
    spriteCloud3.setPosition(1000, 0);
    //clouds on the screen?
    bool cloudActive1 = false;
    bool cloudActive2 = false;
    bool cloudActive3 = false;
    //how fast?
    float cloudSpeed1 = 0.0f;
    float cloudSpeed2 = 0.0f;
    float cloudSpeed3 = 0.0f;

    //variables to control time
    Clock clock;

    //time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Blue);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    //track whether the game is running
    bool paused = true;

    //draw some text
    int score = 0;
    sf::Text messageText;
    sf::Text scoreText;
    Text nameText;

    //choose a font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    //setting font
    messageText.setFont(font);
    scoreText.setFont(font);
    nameText.setFont(font);

    //assign the message
    messageText.setString("Press Enter to Start!");
    scoreText.setString("Score = 0");
    nameText.setString("By Hailey :)");

    //make it really big
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    nameText.setCharacterSize(30);

    //set colour
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::Red);
    nameText.setFillColor(Color::White);

    //position the texts
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);
    nameText.setPosition(1700, 1020);

    //prepare 6 branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");
    //set the texture for each branch
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        //set the sprite's origin to dead center...we can then spin it round without changing its position
        branches[i].setOrigin(220, 20);
    }

    //temporary calling updateBranches methods.
    //updateBranches(1);
    //updateBranches(2);
    //updateBranches(3);
    //updateBranches(4);
    //updateBranches(5);

    //prepare player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);

    //player starts on the left
    side playerSide = side::LEFT;

    //prepare gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    //prepare axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    //line up the axe with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    //prepare the log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    //some other useful log related variables
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    //control the player input
    bool acceptInput = false;

    //prepare the sound
    //chopping sound
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);

    //death sound
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

    //out of time sound
    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    Sound oot;
    oot.setBuffer(ootBuffer);
    

    while (window.isOpen())
    {
        //handle player's input 
        //score++; //checking if the score number is increasing when game is not paused

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused)
            {
                //listen for key presses here again
                acceptInput = true;

                //hide the axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        //start the game
        if (Keyboard::isKeyPressed(Keyboard::Return)) //if player presses enter key the game starts (not paused)
        {
            paused = false;
            score = 0;
            timeRemaining = 5;

            //no branches in the beginning
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }

            //hide the gravestone
            spriteRIP.setPosition(675, 2000);

            //move the player into position
            spritePlayer.setPosition(580, 720);
            acceptInput = true;

        }

        //wrap the player controls to make sure we are accepting input for player chopping
        if (acceptInput)
        {
            //first handling...right key press
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                //player is on the right
                playerSide = side::RIGHT;
                score++;

                //add to the amount of time remaining
                timeRemaining += (2 / score) + .15;

                FloatRect axeRect = spriteAxe.getLocalBounds();
                spriteAxe.setTextureRect(sf::IntRect(0, 0, axeRect.width, axeRect.height));
                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

                spritePlayer.setPosition(1200, 720);

                //update the branhces
                updateBranches(score);

                //set the log flying to the left
                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;

                //play a chop sound;
                chop.play();
            }

            //when player presses left key
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                //make sure the player is on the lft
                playerSide = side::LEFT;
                score++;

                //add amount of time reamining
                timeRemaining += (2 / score) + .15;
            
                //flip the axe (because the axe image is poining left)
                FloatRect axeRect = spriteAxe.getLocalBounds();
                spriteAxe.setTextureRect(sf::IntRect(axeRect.width, 0, -axeRect.width, axeRect.height));
                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                //spriteAxe.setRotation(180);

                spritePlayer.setPosition(580, 720);

                //update branches
                updateBranches(score);

                //set log flying
                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;

                //play a chop sound;
                chop.play();
            }
        }
        //update the scene

        if (!paused)
        {
            //measure time
            Time dt = clock.restart(); //restarts the clock...we want to restart the clock each and every frame

            //subtract from the amount of time remaining
            timeRemaining -= dt.asSeconds();
            //size up the bar
            timeBar.setSize(
                Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f) //if time all runs out
            {
                //pause the game
                paused = true;

                //Change the message shown to the player
                messageText.setString("Time Over!");

                //Reposition the text based on its new size
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                //play oot sound
                oot.play();
            }

            //set up the bee with movement
            if (!beeActive) //if bee is not active
            {
                //speed of the bee
                srand((int)time(0) * 10);
                beeSpeed = (rand() % 200) + 200;

                //how high?
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(0, height);
                beeActive = true;
            }

            else //if bee is active...moving from the right to the left
            {
                spriteBee.setPosition(
                    spriteBee.getPosition().x + (beeSpeed * dt.asSeconds()), //dt.asSeconds...fraction of 1...how long the previous frame of the game took
                    spriteBee.getPosition().y
                );

                //bee reaches left edge of the screen
                if (spriteBee.getPosition().x > 2000)
                {
                    beeActive = false;
                }

            }

            //cloud movements
            //cloud 1
            if (!cloudActive1)
            {
                //random speed for the cloud 1
                srand(int(time(0) * 10));
                cloudSpeed1 = (rand() % 200) + 200;

                //height of the cloud 1
                srand(int(time(0) * 10));
                float height = (rand() % 150);
                spriteCloud1.setPosition(2000, height);
                cloudActive1 = true;
            }

            else //if cloud1 is active
            {
                spriteCloud1.setPosition(
                    spriteCloud1.getPosition().x - (cloudSpeed1 * dt.asSeconds()),
                    spriteCloud1.getPosition().y
                );

                if (spriteCloud1.getPosition().x < -300)
                {
                    cloudActive1 = false;
                }
            }

            //cloud 2
            if (!cloudActive2)
            {
                //random speed for the cloud 2
                srand(int(time(0) * 20));
                cloudSpeed2 = (rand() % 200) + 200;

                //height of the cloud 2
                srand(int(time(0) * 20));
                float height = (rand() % 300) - 50;
                spriteCloud2.setPosition(2000, height);
                cloudActive2 = true;
            }

            else //if cloud2 is active
            {
                spriteCloud2.setPosition(
                    spriteCloud2.getPosition().x - (cloudSpeed2 * dt.asSeconds()),
                    spriteCloud2.getPosition().y
                );

                if (spriteCloud2.getPosition().x < -300)
                {
                    cloudActive2 = false;
                }
            }

            //cloud 3
            if (!cloudActive3)
            {
                //random speed for the cloud 3
                srand(int(time(0) * 30));
                cloudSpeed3 = (rand() % 200) + 200;

                //height of the cloud 3
                srand(int(time(0) * 30));
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition(2000, height);
                cloudActive3 = true;
            }

            else //if cloud3 is active
            {
                spriteCloud3.setPosition(
                    spriteCloud3.getPosition().x - (cloudSpeed3 * dt.asSeconds()),
                    spriteCloud3.getPosition().y
                );

                if (spriteCloud3.getPosition().x < -300)
                {
                    cloudActive3 = false;
                }
            }

            //update the score text
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            //update the branch sprites
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT)
                {
                    branches[i].setPosition(610, height); //move the sprite to the left
                    branches[i].setRotation(180); //flip the sprite
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    branches[i].setPosition(1330, height); //move the sprite to the right
                    branches[i].setRotation(0); //set the sprite to normal
                }
                else
                {
                    //hide the branch
                    branches[i].setPosition(3000, height);
                }

            }

            //handle flying log
            if (logActive)
            {
                spriteLog.setPosition(
                    spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                    spriteLog.getPosition().y + (logSpeedY * dt.asSeconds())
                );

                //has the log reached the right hand edge?
                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
                {
                    //ready to be a whole new thing next frame
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
            }

            //squished by branches?
            if (branchPositions[5] == playerSide)
            {
                //death
                paused = true;
                acceptInput = false;

                //draw gravestone
                if (playerSide == side::RIGHT)
                {
                    spriteRIP.setPosition(1200, 760);
                }
                else
                spriteRIP.setPosition(525, 760);

                //hide the player
                spritePlayer.setPosition(2000, 660);
                spriteAxe.setPosition(3000, spriteAxe.getPosition().y);

                //change the text of the message
                messageText.setString("SQUISHED!!");

                //center it on the screen
                FloatRect textRect = messageText.getLocalBounds();

                messageText.setOrigin(textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                //play the death sound
                death.play();
            }

        } //end if(!paused)

        //draw the scene

        //clear everything from the last frame
        window.clear();

        //draw our game scene here
        window.draw(spriteBackground); //drawing the background sprite first

        //draw clouds before the tree otherwise the clouds will be covering the tree
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        //draw branches after the clouds
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        //draw tree
        window.draw(spriteTree);

        //draw player
        window.draw(spritePlayer);

        //draw axe
        window.draw(spriteAxe);

        //draw log
        window.draw(spriteLog);

        //draw RIP
        window.draw(spriteRIP);

        //draw bee
        window.draw(spriteBee);

        //draw the message
        window.draw(scoreText);
        window.draw(nameText);

        //draw the timebar
        window.draw(timeBar);

        if (paused)
        {
            window.draw(messageText);
        }

        //show everything we just drew
        window.display();
    }
    return 0;
}//end of main function

    //function definition
    void updateBranches(int seed)
    {
        //move all branches down one place... there's no branch position for position 0
        for (int j = NUM_BRANCHES - 1; j > 0; j--)
        {
            branchPositions[j] = branchPositions[j - 1];
        }

        //spawn a new tree branch at position 0
        srand((int)time(0) + seed);
        int r = (rand() % 5);

        switch (r)
        {
        case 0:
            branchPositions[0] = side::LEFT;
            break;

        case 1:
            branchPositions[0] = side::RIGHT;
            break;
            
        default:
            branchPositions[0] = side::NONE;
            break;
        }
    }

    

    



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
