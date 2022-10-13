#include "game.h"
#include <math.h>
#include "ship.h"
#include "bullet.h"
#include "asteroid.h"

enum Options
{
    Menu,
    Play,
    Credits,
    Quit
};

extern Player player;

static Bullet bullet[PLAYER_MAX_BULLET] = { 0 };

static Asteroid bigAsteroid[MAX_BIG_ASTEROID] = { 0 };
static Asteroid mediumAsteroid[MAX_MEDIUM_ASTEROID] = { 0 };
static Asteroid smallAsteroid[MAX_SMALL_ASTEROID] = { 0 };

typedef struct Button {
    float x;
    float y;
    float width;
    float height;

    const char* text;
};

Button buttonPlay;
Button buttonCredits;
Button buttonQuit;
Button buttonPause;
Button buttonReturn;
Button buttonContinue;

Options chosenOpc = Options::Menu;

Music music;

static const char musicUrl[] = "res/sfx/musicaJuego.wav";

float screenWidth;
float screenHeight;

bool shipImpacted = false;
bool gameOver = false;
bool checkPause = false;
bool activeMenu = true;

int destroyedAsteroidCount = 0;
int maxScore = 0;

int initGame();                                                                          // Initialize game
static void game();                                                                      // Play game
static void showCredits();                                                               // Show credits
static void updateGame();                                                                // Update game 
static void drawGame();                                                                  // Draw game 
static void logicMenu();                                                                 // Logic Menu 
static void drawMenu();                                                                  // Draw Menu 
static void updateDrawFrame();                                                           // Update and Draw 

void initButtons()
{
    buttonPlay.width = 120;
    buttonPlay.height = 30;
    buttonPlay.x = screenWidth / 2 - buttonPlay.width / 2;
    buttonPlay.y = screenHeight / 2 - buttonPlay.height / 2;
    buttonPlay.text = "Play";
    
    buttonCredits.width = 100;
    buttonCredits.height = 30;
    buttonCredits.x = screenWidth / 2 - buttonCredits.width / 2;
    buttonCredits.y = screenHeight / 2 - buttonCredits.height / 2 + 65;
    buttonCredits.text = "Credits";

    buttonQuit.width = 50;
    buttonQuit.height = 30;
    buttonQuit.x = screenWidth / 2 - buttonQuit.width / 2;
    buttonQuit.y = screenHeight / 2 - buttonQuit.height / 2 + 130;
    buttonQuit.text = "Quit";

    buttonReturn.width = 180;
    buttonReturn.height = 30;
    buttonReturn.x = 20;
    buttonReturn.y = screenHeight - buttonReturn.height - 20;
    buttonReturn.text = "Back To Menu";
    
    buttonPause.width = 80;
    buttonPause.height = 30;
    buttonPause.x = screenWidth - buttonPause.width - 20;
    buttonPause.y = 20;
    buttonPause.text = "Pause";
    
    buttonContinue.width = 110;
    buttonContinue.height = 30;
    buttonContinue.x = screenWidth / 2 - buttonContinue.width / 2;
    buttonContinue.y = screenHeight / 2 - buttonContinue.height / 2 + 50;
    buttonContinue.text = "Continue";
};

void logicMenu() 
{
    if (CheckCollisionCircleRec(Vector2{ (float)GetMouseX(), (float)GetMouseY() }, 5, Rectangle{ buttonPlay.x, buttonPlay.y, buttonPlay.width, buttonPlay.height }))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            game();
            chosenOpc = Options::Play;
        }
    }
    
    if (CheckCollisionCircleRec(Vector2{ (float)GetMouseX(), (float)GetMouseY() }, 5, Rectangle{ buttonCredits.x, buttonCredits.y, buttonCredits.width, buttonCredits.height }))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            chosenOpc = Options::Credits;
        }
    }
    
    if (CheckCollisionCircleRec(Vector2{ (float)GetMouseX(), (float)GetMouseY() }, 5, Rectangle{ buttonQuit.x, buttonQuit.y, buttonQuit.width, buttonQuit.height }))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            chosenOpc = Options::Quit;
        }
    }
}

void drawButton(Button button)
{
    if (CheckCollisionCircleRec(Vector2{ (float)GetMouseX(), (float)GetMouseY() }, 5, Rectangle{ button.x, button.y, button.width, button.height }))
    {
        DrawRectangle(button.x, button.y, button.width, button.height, DARKBLUE);
        DrawText(button.text, button.x, button.y, 25, WHITE);
    }
    else
    {
        DrawRectangle(button.x, button.y, button.width, button.height, WHITE);
        DrawText(button.text, button.x, button.y, 25, DARKBLUE);
    }
};

void showCredits()
{
    if (CheckCollisionCircleRec(Vector2{ (float)GetMouseX(), (float)GetMouseY() }, 5, Rectangle{ buttonReturn.x, buttonReturn.y, buttonReturn.width, buttonReturn.height }))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            chosenOpc = Options::Menu;
        }
    }

    BeginDrawing();

    ClearBackground(BLACK);

    HideCursor();
    DrawCircle((float)GetMouseX(), (float)GetMouseY(), 5, RED);

    DrawText("Created by:", 0, 2, 10, WHITE);
    DrawText("Jose Altamirano", 0, 12, 13, WHITE);
    
    DrawText("Texture of Ship:", 0, 30, 12, WHITE);
    DrawText("Created by: scofanogd", 0, 40, 10, WHITE);
    DrawText("Name of Asset: Spaceship", 0, 50, 10, WHITE);
    DrawText("Link: ( https://opengameart.org/content/spaceship-9 )", 0, 60, 10, WHITE);
    
    DrawText("Texture of Asteroids:", 0, 90, 12, WHITE);
    DrawText("Created by: IgalBlech", 0, 100, 10, WHITE);
    DrawText("Name of Asset: Spinning Asteroid Animation", 0, 110, 10, WHITE);
    DrawText("Link: ( https://opengameart.org/content/spinning-asteroid-animation )", 0, 120, 10, WHITE);
    
    DrawText("Music of Game:", 0, 150, 12, WHITE);
    DrawText("Created by: Matthew Pablo", 0, 160, 10, WHITE);
    DrawText("Name of Asset: Space Boss Batlle Theme", 0, 170, 10, WHITE);
    DrawText("Link: ( https://opengameart.org/content/space-boss-battle-theme )", 0, 180, 10, WHITE);
    
    DrawText("Sound of Shoot:", 0, 210, 12, WHITE);
    DrawText("Created by: jalastram", 0, 220, 10, WHITE);
    DrawText("Name of Asset: Sound Effects | SFX007", 0, 230, 10, WHITE);
    DrawText("Link: ( https://opengameart.org/content/sound-effects-sfx007 )", 0, 240, 10, WHITE);
    
    DrawText("Sound of Destruction Asteroid:", 0, 270, 12, WHITE);
    DrawText("Created by: Luke.RUSTLTD", 0, 280, 10, WHITE);
    DrawText("Name of Asset: bomb_explosion_8bit", 0, 290, 10, WHITE);
    DrawText("Link: ( https://opengameart.org/content/bombexplosion8bit )", 0, 300, 10, WHITE);
    
    DrawText("Sound of Destruction Ship:", 0, 330, 12, WHITE);
    DrawText("Created by: Blender Foundation", 0, 340, 10, WHITE);
    DrawText("Name of Asset: Big Explosion", 0, 350, 10, WHITE);
    DrawText("Link: ( https://opengameart.org/content/big-explosion )", 0, 360, 10, WHITE);

    //DrawButtonReturn
    drawButton(buttonReturn);

    EndDrawing();
};

void drawMenu()
{
    BeginDrawing();

    ClearBackground(BLACK);

    HideCursor();
    DrawCircle((float)GetMouseX(), (float)GetMouseY(), 5, RED);

    DrawText("Asteroids", 0, 0, 120,WHITE);

    //DrawButtonPlay
    drawButton(buttonPlay);

    //DrawButtonCredits
    drawButton(buttonCredits);

    //DrawButtonQuit
    drawButton(buttonQuit);

    EndDrawing();
};

int initGame()
{
    screenWidth = 1024;
    screenHeight = 768;

    if (!IsAudioDeviceReady())
    {
        InitAudioDevice();

        music = LoadMusicStream(musicUrl);
        SetMusicVolume(music, 0.15f);
    }

    InitWindow(screenWidth, screenHeight, "Asteroids - Altamirano");

    initButtons();

    SetTargetFPS(60);
    
    // Main game loop
    while (!WindowShouldClose() && chosenOpc != Options::Quit) // Detect window close button or ESC key
    {   
        if (!IsMusicStreamPlaying(music))
        {
            PlayMusicStream(music);
        }
        else UpdateMusicStream(music);

        // Update and Draw
        switch (chosenOpc)
        {
        case Menu:
            logicMenu();
            drawMenu();
            break;

        case Play:
            updateDrawFrame();
            break;

        case Credits:
            showCredits();
            break;
        }
    }

    CloseWindow();

    return 0;
}

float Vector2AngleCustom(Vector2 v1, Vector2 v2)
{
    float result = atan2f(v2.y - v1.y, v2.x - v1.x) * (180.0f / PI);
    if (result < 0) result += 360.0f;
    return result;
}   

// Initialize game variables
void game()
{
    checkPause = false;

    initPlayer();

    destroyedAsteroidCount = 0;

    initBullet(bullet);

    initAsteroid(bigAsteroid, mediumAsteroid, smallAsteroid);
}

// Update game
void updateGame()
{
    if (!shipImpacted)
    {
        if (!checkPause)
        {
            playerRotation();
            playerSpeed();
            movePlayer();
            playerCollisionWall();
            playerBullet(bullet);

            bulletTimeLife(bullet);
            bulletLogic(bullet);

            playerCollisionAsteroid(bigAsteroid, mediumAsteroid, smallAsteroid);

            bigAsteroidLogic(bigAsteroid);
            mediumAsteroidLogic(mediumAsteroid);
            smallAsteroidLogic(smallAsteroid);

            collisionBulletAsteroid(bullet, bigAsteroid, mediumAsteroid, smallAsteroid);

            if (CheckCollisionCircleRec(Vector2{ (float)GetMouseX(), (float)GetMouseY() }, 5, Rectangle{ buttonPause.x, buttonPause.y, buttonPause.width, buttonPause.height }))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    checkPause = !checkPause;
                }
            }
        }
        else
        {
            if (CheckCollisionCircleRec(Vector2{ (float)GetMouseX(), (float)GetMouseY() }, 5, Rectangle{ buttonContinue.x, buttonContinue.y, buttonContinue.width, buttonContinue.height }))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    checkPause = !checkPause;
                }
            }

            if (CheckCollisionCircleRec(Vector2{ (float)GetMouseX(), (float)GetMouseY() }, 5, Rectangle{ buttonReturn.x, buttonReturn.y, buttonReturn.width, buttonReturn.height }))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    chosenOpc = Options::Menu;
                }
            }
        }
         
        maxScore = destroyedAsteroidCount;
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            initAsteroid(bigAsteroid, mediumAsteroid, smallAsteroid);
            
            if (gameOver && player.lives <= 0)
            {
                player.lives = 3;
            }

            shipImpacted = false;
            gameOver = false;
            updateDrawFrame();
        }
    }
}

// Draw game 
void drawGame()
{
    BeginDrawing();

    ClearBackground(BLACK);

    if (!gameOver)
    {
        DrawCircle((float)GetMouseX(), (float)GetMouseY(), 5, RED);

        drawShip();

        drawAsteroid(bigAsteroid, mediumAsteroid, smallAsteroid);

        drawBullet(bullet);

        if (checkPause)
        {
            DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);

            drawButton(buttonReturn);
            drawButton(buttonContinue);
        }
        
        if (!shipImpacted)
        {
            //DrawButtonPause
            drawButton(buttonPause);
        }

        if (shipImpacted)
        {
            DrawText("YOUR SHIP CRASHED", GetScreenWidth() / 2 - MeasureText("YOUR SHIP CRASHED", 20) / 2, GetScreenHeight() / 2 - 100, 20, GRAY);
            DrawText("PRESS [ENTER] TO CONTINUE", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO CONTINUE", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
        }
        
        DrawText(TextFormat("Lives: %i", player.lives), 5, 5, 20, WHITE);
        DrawText(TextFormat("Score: %i", destroyedAsteroidCount), 5, 25, 20, WHITE);
    }
    else
    {
        DrawText("GAME OVER", GetScreenWidth() / 2 - MeasureText("GAME OVER", 20) / 2, GetScreenHeight() / 2 - 100, 20, GRAY);
        DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
        DrawText(TextFormat("YOUR MAX SCORE: %i", maxScore), GetScreenWidth() / 2 - MeasureText("GAME OVER", 20) / 2, GetScreenHeight() / 2, 20, GRAY);

        destroyedAsteroidCount = 0;
    }

    EndDrawing();
}

// Update and Draw
void updateDrawFrame()
{
    updateGame();
    drawGame();
}
