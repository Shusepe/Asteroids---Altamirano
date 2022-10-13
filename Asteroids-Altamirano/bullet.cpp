#include "bullet.h"
#include <math.h>

extern float screenHeight;
extern float screenWidth;

extern int midAsteroidCount;
extern int smallAsteroidCount;
extern int destroyedAsteroidCount;
extern int bigAsteroidsNonAlive;

static const char shootSfxUrl[] = "res/sfx/disparo.mp3";

void initBullet(Bullet bullet[])
{
    // Initialization shoot
    for (int i = 0; i < PLAYER_MAX_BULLET; i++)
    {
        bullet[i].shootSfx = LoadSound(shootSfxUrl);
        bullet[i].position = { 0, 0 };
        bullet[i].speed = { 0, 0 };
        bullet[i].radius = 2;
        bullet[i].active = false;
        bullet[i].lifeSpawn = 0;
        bullet[i].color = RED;
    }
};

void bulletTimeLife(Bullet bullet[])
{
    // Shoot life timer
    for (int i = 0; i < PLAYER_MAX_BULLET; i++)
    {
        if (bullet[i].active) bullet[i].lifeSpawn++;
    }
};

void bulletLogic(Bullet bullet[])
{
    // Shot logic
    for (int i = 0; i < PLAYER_MAX_BULLET; i++)
    {
        if (bullet[i].active)
        {
            // Movement
            bullet[i].position.x += bullet[i].speed.x;
            bullet[i].position.y -= bullet[i].speed.y;

            // Collision logic: shoot vs walls
            if (bullet[i].position.x > screenWidth + bullet[i].radius)
            {
                bullet[i].active = false;
                bullet[i].lifeSpawn = 0;
            }
            else if (bullet[i].position.x < 0 - bullet[i].radius)
            {
                bullet[i].active = false;
                bullet[i].lifeSpawn = 0;
            }
            if (bullet[i].position.y > screenHeight + bullet[i].radius)
            {
                bullet[i].active = false;
                bullet[i].lifeSpawn = 0;
            }
            else if (bullet[i].position.y < 0 - bullet[i].radius)
            {
                bullet[i].active = false;
                bullet[i].lifeSpawn = 0;
            }

            // Life of shoot
            if (bullet[i].lifeSpawn >= 60)
            {
                bullet[i].position = { 0, 0 };
                bullet[i].speed = { 0, 0 };
                bullet[i].lifeSpawn = 0;
                bullet[i].active = false;
            }
        }
    }
};

void collisionBulletAsteroid(Bullet bullet[], Asteroid bigAsteroid[], Asteroid mediumAsteroid[], Asteroid smallAsteroid[])
{
    // Collision logic: player-shoots vs meteors
    for (int i = 0; i < PLAYER_MAX_BULLET; i++)
    {
        if ((bullet[i].active))
        {
            for (int a = 0; a < MAX_BIG_ASTEROID; a++)
            {
                if (bigAsteroid[a].active && CheckCollisionCircles(bullet[i].position, bullet[i].radius, bigAsteroid[a].position, bigAsteroid[a].radius))
                {
                    bullet[i].active = false;
                    bullet[i].lifeSpawn = 0;
                    bigAsteroid[a].active = false;
                    destroyedAsteroidCount++;

                    SetSoundPitch(bigAsteroid[a].explodeSfx, ((float)GetRandomValue(0, 15) / 100) + 1);
                    PlaySound(bigAsteroid[a].explodeSfx);

                    for (int j = 0; j < 2; j++)
                    {
                        if (midAsteroidCount % 2 == 0)
                        {
                            mediumAsteroid[midAsteroidCount].position = { bigAsteroid[a].position.x, bigAsteroid[a].position.y };
                            mediumAsteroid[midAsteroidCount].speed = { (float)(cos(bullet[i].rotation * DEG2RAD) * ASTEROID_SPEED * -1), (float)(sin(bullet[i].rotation * DEG2RAD) * ASTEROID_SPEED * -1) };
                        }
                        else
                        {
                            mediumAsteroid[midAsteroidCount].position = { bigAsteroid[a].position.x, bigAsteroid[a].position.y };
                            mediumAsteroid[midAsteroidCount].speed = { (float)(cos(bullet[i].rotation * DEG2RAD) * ASTEROID_SPEED), (float)(sin(bullet[i].rotation * DEG2RAD) * ASTEROID_SPEED) };
                        }

                        mediumAsteroid[midAsteroidCount].active = true;
                        midAsteroidCount++;
                    }
                    a = MAX_BIG_ASTEROID;
                }

                if (!bigAsteroid[a].active)
                {
                    bigAsteroidsNonAlive += 1;
                }
            }

            if (bigAsteroidsNonAlive == MAX_BIG_ASTEROID)
            {
                for (int a = 0; a < MAX_BIG_ASTEROID; a++)
                {
                    bigAsteroid[a].active = true;
                    bigAsteroid[a].position.y = GetRandomValue(0, screenHeight);
                    bigAsteroid[a].position.x = GetRandomValue(0, screenWidth);
                }

                bigAsteroidsNonAlive = 0;
            }
            else
            {
                bigAsteroidsNonAlive = 0;
            }

            for (int b = 0; b < MAX_MEDIUM_ASTEROID; b++)
            {
                if (mediumAsteroid[b].active && CheckCollisionCircles(bullet[i].position, bullet[i].radius, mediumAsteroid[b].position, mediumAsteroid[b].radius))
                {
                    bullet[i].active = false;
                    bullet[i].lifeSpawn = 0;
                    mediumAsteroid[b].active = false;
                    destroyedAsteroidCount++;

                    SetSoundPitch(mediumAsteroid[b].explodeSfx, ((float)GetRandomValue(0, 15) / 100) + 1);
                    PlaySound(mediumAsteroid[b].explodeSfx);

                    for (int j = 0; j < 2; j++)
                    {
                        if (smallAsteroidCount % 2 == 0)
                        {
                            smallAsteroid[smallAsteroidCount].position = { mediumAsteroid[b].position.x, mediumAsteroid[b].position.y };
                            smallAsteroid[smallAsteroidCount].speed = { (float)(cos(bullet[i].rotation * DEG2RAD) * ASTEROID_SPEED * -1), (float)(sin(bullet[i].rotation * DEG2RAD) * ASTEROID_SPEED * -1) };
                        }
                        else
                        {
                            smallAsteroid[smallAsteroidCount].position = { mediumAsteroid[b].position.x, mediumAsteroid[b].position.y };
                            smallAsteroid[smallAsteroidCount].speed = { (float)(cos(bullet[i].rotation * DEG2RAD) * ASTEROID_SPEED), (float)(sin(bullet[i].rotation * DEG2RAD) * ASTEROID_SPEED) };
                        }

                        smallAsteroid[smallAsteroidCount].active = true;
                        smallAsteroidCount++;
                    }
                    b = MAX_MEDIUM_ASTEROID;
                }
            }

            for (int c = 0; c < MAX_SMALL_ASTEROID; c++)
            {
                if (smallAsteroid[c].active && CheckCollisionCircles(bullet[i].position, bullet[i].radius, smallAsteroid[c].position, smallAsteroid[c].radius))
                {
                    bullet[i].active = false;
                    bullet[i].lifeSpawn = 0;
                    smallAsteroid[c].active = false;
                    destroyedAsteroidCount++;

                    SetSoundPitch(smallAsteroid[c].explodeSfx, ((float)GetRandomValue(0, 15) / 100) + 1);
                    PlaySound(smallAsteroid[c].explodeSfx);

                    c = MAX_SMALL_ASTEROID;
                }
            }
        }
    }
};

void drawBullet(Bullet bullet[])
{
    // Draw shoot
    for (int i = 0; i < PLAYER_MAX_BULLET; i++)
    {
        if (bullet[i].active) DrawCircleV(bullet[i].position, bullet[i].radius, bullet[i].color);
    }
};