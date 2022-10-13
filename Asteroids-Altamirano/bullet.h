#pragma once
#include "raylib.h"
#include "asteroid.h"

#define PLAYER_MAX_BULLET   3

typedef struct Bullet {
    Sound shootSfx;
    Vector2 position;
    Vector2 speed;
    float radius;
    float rotation;
    int lifeSpawn;
    bool active;
    Color color;
} Bullet;

void initBullet(Bullet bullet[]);
void bulletTimeLife(Bullet bullet[]);
void bulletLogic(Bullet bullet[]);
void collisionBulletAsteroid(Bullet bullet[], Asteroid bigAsteroid[], Asteroid mediumAsteroid[], Asteroid smallAsteroid[]);
void drawBullet(Bullet bullet[]);