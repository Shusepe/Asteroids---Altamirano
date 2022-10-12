#include "game.h"
#include <math.h>
#include "ship.h"
#include "bullet.h"
#include "asteroid.h"

static Bullet bullet[PLAYER_MAX_BULLET] = { 0 };

static Asteroid bigAsteroid[MAX_BIG_ASTEROID] = { 0 };
static Asteroid mediumAsteroid[MAX_MEDIUM_ASTEROID] = { 0 };
static Asteroid smallAsteroid[MAX_SMALL_ASTEROID] = { 0 };

typedef struct button {
    float x;
    float y;
    float width;
    float height;
};

button buttonPlay;
button buttonCredits;
button buttonQuit;
button buttonPause;
button buttonBackToMenu;

float screenWidth;
float screenHeight;

bool gameOver = false;
bool checkPause = false;
bool victory = false;
bool activeMenu = true;
bool onQuit = false;

int destroyedAsteroidCount = 0;

int initGame();                                                 // Initialize game
static void game();                                             // Play game
//static void credits();                                        // Show credits
static void updateGame();                                       // Update game 
static void drawGame();                                         // Draw game 
static void logicMenu();                                        // Logic Menu 
static void drawMenu();                                         // Draw Menu 
static void updateDrawFrame();                                  // Update and Draw 
static void mainMenu(button buttonPlay, button buttonQuit);     // Menu

void initMenu(button *buttonPlay, button *buttonQuit)
{
    (*buttonPlay).width = 100;
    (*buttonPlay).height = 30;
    (*buttonPlay).x = screenWidth / 2 - (*buttonPlay).width / 2;
    (*buttonPlay).y = screenHeight / 2 - (*buttonPlay).height / 2;

    (*buttonQuit).width = 50;
    (*buttonQuit).height = 30;
    (*buttonQuit).x = screenWidth / 2 - (*buttonQuit).width / 2;
    (*buttonQuit).y = screenHeight / 2 - (*buttonQuit).height / 2 + 130;
};

void mainMenu(button buttonPlay, button buttonQuit)
{
    logicMenu();

    drawMenu();
};

void logicMenu()
{
    if (CheckCollisionCircleRec(Vector2{ (float) GetMouseX(), (float) GetMouseY() }, 5, Rectangle{ buttonPlay.x, buttonPlay.y, buttonPlay.width, buttonPlay.height }))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            game();
            activeMenu = false;
        }
    }
    
    if (CheckCollisionCircleRec(Vector2{ (float) GetMouseX(), (float) GetMouseY() }, 5, Rectangle{ buttonQuit.x, buttonQuit.y, buttonQuit.width, buttonQuit.height }))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            onQuit = true;
            CloseWindow();
        }
    }
};

void drawMenu()
{
    if (!onQuit)
    {
        BeginDrawing();

        ClearBackground(BLACK);

        HideCursor();
        DrawCircle((float)GetMouseX(), (float)GetMouseY(), 5, RED);

        if (CheckCollisionCircleRec(Vector2{ (float)GetMouseX(), (float)GetMouseY() }, 5, Rectangle{ buttonPlay.x, buttonPlay.y, buttonPlay.width, buttonPlay.height }))
        {
            DrawRectangle(buttonPlay.x, buttonPlay.y, buttonPlay.width, buttonPlay.height, DARKBLUE);
            DrawText("Play", buttonPlay.x, buttonPlay.y, 25, WHITE);
        }
        else
        {
            DrawRectangle(buttonPlay.x, buttonPlay.y, buttonPlay.width, buttonPlay.height, WHITE);
            DrawText("Play", buttonPlay.x, buttonPlay.y, 25, DARKBLUE);
        }

        /*if (CheckCollisionCircleRec(Vector2{ (float)GetMouseX(), (float)GetMouseY() }, 5, Rectangle{ buttonPlay.x - buttonPlay.width / 2, buttonPlay.y - buttonPlay.height / 2, buttonPlay.width, buttonPlay.height }))
        {
            DrawRectangle(buttonPlay.x - buttonPlay.width / 2, buttonPlay.y - buttonPlay.height / 2, buttonPlay.width, buttonPlay.height, BLUE);
        }
        else
        {
            DrawRectangle(buttonPlay.x - buttonPlay.width / 2, buttonPlay.y - buttonPlay.height / 2, buttonPlay.width, buttonPlay.height, WHITE);
        }*/

        if (CheckCollisionCircleRec(Vector2{ (float)GetMouseX(), (float)GetMouseY() }, 5, Rectangle{ buttonQuit.x, buttonQuit.y, buttonQuit.width, buttonQuit.height }))
        {
            DrawRectangle(buttonQuit.x, buttonQuit.y, buttonQuit.width, buttonQuit.height, DARKBLUE);
            DrawText("Quit", buttonQuit.x, buttonQuit.y, 22, WHITE);
        }
        else
        {
            DrawRectangle(buttonQuit.x, buttonQuit.y, buttonQuit.width, buttonQuit.height, WHITE);
            DrawText("Quit", buttonQuit.x, buttonQuit.y, 22, DARKBLUE);
        }

        EndDrawing();
    }
};

int initGame()
{
    screenWidth = 800;
    screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Asteroids - Altamirano");

    initMenu(&buttonPlay, &buttonQuit);
    //game();

    SetTargetFPS(60);
    
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update and Draw
        if (activeMenu)
        {
            mainMenu(buttonPlay, buttonQuit);
        }
        else
        {
            updateDrawFrame();
        }
    }
    
    return 0;
}

float Vector2Angle(Vector2 v1, Vector2 v2)
{
    float result = atan2f(v2.y - v1.y, v2.x - v1.x) * (180.0f / PI);
    if (result < 0) result += 360.0f;
    return result;
}   

// Initialize game variables
void game()
{
    victory = false;
    checkPause = false;

    initPlayer();

    destroyedAsteroidCount = 0;

    initBullet(bullet);

    initAsteroid(bigAsteroid, mediumAsteroid, smallAsteroid);
}

// Update game
void updateGame()
{
    if (!gameOver)
    {
        if (IsKeyPressed('P')) checkPause = !checkPause;

        if (!checkPause)
        {
            playerRotation();
            playerSpeed();
            playerAceleration();
            playerMovement();
            playerCollisionWall();
            playerBullet(bullet);

            bulletTimeLife(bullet);
            bulletLogic(bullet);

            playerCollisionAsteroid(bigAsteroid, mediumAsteroid, smallAsteroid);

            bigAsteroidLogic(bigAsteroid);
            mediumAsteroidLogic(mediumAsteroid);
            smallAsteroidLogic(smallAsteroid);

            collisionBulletAsteroid(bullet, bigAsteroid, mediumAsteroid, smallAsteroid);
        }

        if (destroyedAsteroidCount == MAX_BIG_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID) victory = true;
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
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

        if (victory) DrawText("VICTORY", screenWidth / 2 - MeasureText("VICTORY", 20) / 2, screenHeight / 2, 20, LIGHTGRAY);

        if (checkPause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
    }
    else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);

    EndDrawing();
}

// Update and Draw
void updateDrawFrame()
{
    updateGame();
    drawGame();
}
