#pragma once
#include "raylib.h"

#define ASTEROID_SPEED       2
#define MAX_BIG_ASTEROID     4
#define MAX_MEDIUM_ASTEROID  8
#define MAX_SMALL_ASTEROID   16

typedef struct Asteroid {
    Vector2 position;
    Vector2 speed;
    float radius;
    bool active;
    Color color;
} Asteroid;

void initAsteroid(Asteroid bigAsteroid[], Asteroid mediumAsteroid[], Asteroid smallAsteroid[]);
void bigAsteroidLogic(Asteroid bigAsteroid[]);
void mediumAsteroidLogic(Asteroid mediumAsteroid[]);
void smallAsteroidLogic(Asteroid smallAsteroid[]);
void drawAsteroid(Asteroid bigAsteroid[], Asteroid mediumAsteroid[], Asteroid smallAsteroid[]);