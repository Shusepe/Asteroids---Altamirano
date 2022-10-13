#include "asteroid.h"

extern float screenWidth;
extern float screenHeight;

int midAsteroidCount = 0;
int smallAsteroidCount = 0;
int bigAsteroidsNonAlive = 0;

static const char bigAsteroidImgUrl[] = "res/img/bigAsteroid.png";
static const char mediumAsteroidImgUrl[] = "res/img/mediumAsteroid.png";
static const char smallAsteroidImgUrl[] = "res/img/smallAsteroid.png";

static const char asteroidSfxUrl[] = "res/sfx/explosionDeAsteroide.wav";

void initAsteroid(Asteroid bigAsteroid[], Asteroid mediumAsteroid[], Asteroid smallAsteroid[])
{
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;
    bool range = false;

    for (int i = 0; i < MAX_BIG_ASTEROID; i++)
    {
        positionX = GetRandomValue(0, screenWidth);

        while (!range)
        {
            if (positionX > screenWidth / 2 - 150 && positionX < screenWidth / 2 + 150) positionX = GetRandomValue(0, screenWidth);
            else range = true;
        }

        range = false;

        positionY = GetRandomValue(0, screenHeight);

        while (!range)
        {
            if (positionY > screenHeight / 2 - 150 && positionY < screenHeight / 2 + 150)  positionY = GetRandomValue(0, screenHeight);
            else range = true;
        }

        bigAsteroid[i].position = { positionX, positionY };

        range = false;
        velocityX = GetRandomValue(-ASTEROID_SPEED, ASTEROID_SPEED);
        velocityY = GetRandomValue(-ASTEROID_SPEED, ASTEROID_SPEED);

        while (!range)
        {
            if (velocityX == 0 && velocityY == 0)
            {
                velocityX = GetRandomValue(-ASTEROID_SPEED, ASTEROID_SPEED);
                velocityY = GetRandomValue(-ASTEROID_SPEED, ASTEROID_SPEED);
            }
            else range = true;
        }

        bigAsteroid[i].explodeSfx = LoadSound(asteroidSfxUrl);
        bigAsteroid[i].sprite = LoadTexture(bigAsteroidImgUrl);
        bigAsteroid[i].speed = { velocityX, velocityY };
        bigAsteroid[i].radius = 40;
        bigAsteroid[i].active = true;
        bigAsteroid[i].color = WHITE;
    }

    for (int i = 0; i < MAX_MEDIUM_ASTEROID; i++)
    {
        mediumAsteroid[i].explodeSfx = LoadSound(asteroidSfxUrl);
        mediumAsteroid[i].sprite = LoadTexture(mediumAsteroidImgUrl);
        mediumAsteroid[i].position = { -100, -100 };
        mediumAsteroid[i].speed = { 0,0 };
        mediumAsteroid[i].radius = 20;
        mediumAsteroid[i].active = false;
        mediumAsteroid[i].color = WHITE;
    }

    for (int i = 0; i < MAX_SMALL_ASTEROID; i++)
    {
        smallAsteroid[i].explodeSfx = LoadSound(asteroidSfxUrl);
        smallAsteroid[i].sprite = LoadTexture(smallAsteroidImgUrl);
        smallAsteroid[i].position = { -100, -100 };
        smallAsteroid[i].speed = { 0,0 };
        smallAsteroid[i].radius = 10;
        smallAsteroid[i].active = false;
        smallAsteroid[i].color = WHITE;
    }

    midAsteroidCount = 0;
    smallAsteroidCount = 0;
};

void bigAsteroidLogic(Asteroid bigAsteroid[])
{
    // Meteors logic: big meteors
    for (int i = 0; i < MAX_BIG_ASTEROID; i++)
    {
        if (bigAsteroid[i].active)
        {
            // Movement
            bigAsteroid[i].position.x += bigAsteroid[i].speed.x;
            bigAsteroid[i].position.y += bigAsteroid[i].speed.y;

            // Collision logic: meteor vs wall
            if (bigAsteroid[i].position.x > screenWidth + bigAsteroid[i].radius) bigAsteroid[i].position.x = -(bigAsteroid[i].radius);
            else if (bigAsteroid[i].position.x < 0 - bigAsteroid[i].radius) bigAsteroid[i].position.x = screenWidth + bigAsteroid[i].radius;
            if (bigAsteroid[i].position.y > screenHeight + bigAsteroid[i].radius) bigAsteroid[i].position.y = -(bigAsteroid[i].radius);
            else if (bigAsteroid[i].position.y < 0 - bigAsteroid[i].radius) bigAsteroid[i].position.y = screenHeight + bigAsteroid[i].radius;
        }
    }
};

void mediumAsteroidLogic(Asteroid mediumAsteroid[])
{
    // Meteors logic: medium meteors
    for (int i = 0; i < MAX_MEDIUM_ASTEROID; i++)
    {
        if (mediumAsteroid[i].active)
        {
            // Movement
            mediumAsteroid[i].position.x += mediumAsteroid[i].speed.x;
            mediumAsteroid[i].position.y += mediumAsteroid[i].speed.y;

            // Collision logic: meteor vs wall
            if (mediumAsteroid[i].position.x > screenWidth + mediumAsteroid[i].radius) mediumAsteroid[i].position.x = -(mediumAsteroid[i].radius);
            else if (mediumAsteroid[i].position.x < 0 - mediumAsteroid[i].radius) mediumAsteroid[i].position.x = screenWidth + mediumAsteroid[i].radius;
            if (mediumAsteroid[i].position.y > screenHeight + mediumAsteroid[i].radius) mediumAsteroid[i].position.y = -(mediumAsteroid[i].radius);
            else if (mediumAsteroid[i].position.y < 0 - mediumAsteroid[i].radius) mediumAsteroid[i].position.y = screenHeight + mediumAsteroid[i].radius;
        }
    }
};

void smallAsteroidLogic(Asteroid smallAsteroid[])
{
    // Meteors logic: small meteors
    for (int i = 0; i < MAX_SMALL_ASTEROID; i++)
    {
        if (smallAsteroid[i].active)
        {
            // Movement
            smallAsteroid[i].position.x += smallAsteroid[i].speed.x;
            smallAsteroid[i].position.y += smallAsteroid[i].speed.y;

            // Collision logic: meteor vs wall
            if (smallAsteroid[i].position.x > screenWidth + smallAsteroid[i].radius) smallAsteroid[i].position.x = -(smallAsteroid[i].radius);
            else if (smallAsteroid[i].position.x < 0 - smallAsteroid[i].radius) smallAsteroid[i].position.x = screenWidth + smallAsteroid[i].radius;
            if (smallAsteroid[i].position.y > screenHeight + smallAsteroid[i].radius) smallAsteroid[i].position.y = -(smallAsteroid[i].radius);
            else if (smallAsteroid[i].position.y < 0 - smallAsteroid[i].radius) smallAsteroid[i].position.y = screenHeight + smallAsteroid[i].radius;
        }
    }
};

void drawAsteroid(Asteroid bigAsteroid[], Asteroid mediumAsteroid[], Asteroid smallAsteroid[])
{
    // Draw meteors
    for (int i = 0; i < MAX_BIG_ASTEROID; i++)
    {
        if (bigAsteroid[i].active) 
        { 
            DrawTexturePro(
                bigAsteroid[i].sprite,
                Rectangle{ 0,0,(float)bigAsteroid[i].sprite.width,(float)bigAsteroid[i].sprite.height },
                Rectangle{ bigAsteroid[i].position.x, bigAsteroid[i].position.y, bigAsteroid[i].radius * 2, bigAsteroid[i].radius * 2 },
                Vector2{ (bigAsteroid[i].radius * 2) / 2, (bigAsteroid[i].radius * 2) / 2 },
                1,
                bigAsteroid[i].color);
        }
    }

    for (int i = 0; i < MAX_MEDIUM_ASTEROID; i++)
    {
        if (mediumAsteroid[i].active) 
        { 
            DrawTexturePro(
                mediumAsteroid[i].sprite,
                Rectangle{ 0,0,(float)mediumAsteroid[i].sprite.width,(float)mediumAsteroid[i].sprite.height },
                Rectangle{ mediumAsteroid[i].position.x, mediumAsteroid[i].position.y, mediumAsteroid[i].radius * 2, mediumAsteroid[i].radius * 2 },
                Vector2{ (mediumAsteroid[i].radius * 2) / 2, (mediumAsteroid[i].radius * 2) / 2 },
                1,
                mediumAsteroid[i].color);
        }
    }

    for (int i = 0; i < MAX_SMALL_ASTEROID; i++)
    {
        if (smallAsteroid[i].active) 
        { 
            DrawTexturePro(
                smallAsteroid[i].sprite,
                Rectangle{ 0,0,(float)smallAsteroid[i].sprite.width,(float)smallAsteroid[i].sprite.height },
                Rectangle{ smallAsteroid[i].position.x, smallAsteroid[i].position.y, smallAsteroid[i].radius * 2, smallAsteroid[i].radius * 2 },
                Vector2{ (smallAsteroid[i].radius * 2) / 2, (smallAsteroid[i].radius * 2) / 2 },
                1,
                smallAsteroid[i].color);
        }
    }
};