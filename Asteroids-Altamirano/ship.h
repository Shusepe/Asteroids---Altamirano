#pragma once
#include "raylib.h"
#include <math.h>
#include "bullet.h"

#define PLAYER_BASE_SIZE    20.0f
#define PLAYER_SPEED        6.0f

typedef struct Player {
    Texture2D sprite;
    Sound explodeSfx;
    Vector2 position;
    Vector2 speed;
    float acceleration;
    float rotation;
    int lives;
    Vector3 collider;
    Color color;
} Player;

void initPlayer();
void playerMovement();
void playerAceleration();
void playerRotation();
void playerSpeed();
void playerBullet(Bullet bullet[]);
void playerCollisionWall();
void playerCollisionAsteroid(Asteroid bigAsteroid[], Asteroid mediumAsteroid[], Asteroid smallAsteroid[]);
void drawShip();