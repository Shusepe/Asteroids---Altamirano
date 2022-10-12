#include "ship.h"
#include "asteroid.h"
#include "game.h"

extern float screenWidth;
extern float screenHeight;

extern bool gameOver;

float shipHeight = 0.0f;

void initPlayer()
{
	shipHeight = (PLAYER_BASE_SIZE / 2) / tanf(20 * DEG2RAD);
	
	// Initialization player
	player.position = { (float) (screenWidth / 2), (float) (screenHeight / 2 - shipHeight / 2) };
	player.speed = { 0, 0 };
	player.acceleration = 0;
	player.rotation = 0;
	player.collider = { (float)(player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f)), (float)(player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f)), 12 };
	player.color = LIGHTGRAY;
};

void playerMovement() 
{
	// Player logic: movement
	player.position.x += (player.speed.x * player.acceleration);
	player.position.y -= (player.speed.y * player.acceleration);
};

void playerAceleration() 
{
	// Player logic: acceleration
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		if (player.acceleration < 1) player.acceleration += 0.04f;
	}
	else
	{
		if (player.acceleration > 0) player.acceleration -= 0.02f;
		else if (player.acceleration < 0) player.acceleration = 0;
	}
};

void playerRotation() 
{
	// Player logic: rotation
	player.rotation = Vector2Angle(player.position, GetMousePosition()) + 90;
};

void playerSpeed() 
{
	// Player logic: speed
	player.speed.x = sin(player.rotation * DEG2RAD) * PLAYER_SPEED;
	player.speed.y = cos(player.rotation * DEG2RAD) * PLAYER_SPEED;
};

void playerBullet(Bullet bullet[])
{
	// Player shoot logic
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		for (int i = 0; i < PLAYER_MAX_BULLET; i++)
		{
			if (!bullet[i].active)
			{
				bullet[i].position = { (float)(player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight)), (float)(player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight)) };
				bullet[i].active = true;
				bullet[i].speed.x = 1.5 * sin(player.rotation * DEG2RAD) * PLAYER_SPEED;
				bullet[i].speed.y = 1.5 * cos(player.rotation * DEG2RAD) * PLAYER_SPEED;
				bullet[i].rotation = player.rotation;
				break;
			}
		}
	}
};

void playerCollisionWall() 
{
	// Collision logic: player vs walls
	if (player.position.x > screenWidth + shipHeight) player.position.x = -(shipHeight);
	else if (player.position.x < -(shipHeight)) player.position.x = screenWidth + shipHeight;
	if (player.position.y > (screenHeight + shipHeight)) player.position.y = -(shipHeight);
	else if (player.position.y < -(shipHeight)) player.position.y = screenHeight + shipHeight;
};

void playerCollisionAsteroid(Asteroid bigAsteroid[], Asteroid mediumAsteroid[], Asteroid smallAsteroid[])
{
	// Collision logic: player vs meteors
	player.collider = { (float)(player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f)), (float)(player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f)), 12 };

	for (int a = 0; a < MAX_BIG_ASTEROID; a++)
	{
		if (CheckCollisionCircles({ player.collider.x, player.collider.y }, player.collider.z, bigAsteroid[a].position, bigAsteroid[a].radius) && bigAsteroid[a].active) gameOver = true;
	}

	for (int a = 0; a < MAX_MEDIUM_ASTEROID; a++)
	{
		if (CheckCollisionCircles({ player.collider.x, player.collider.y }, player.collider.z, mediumAsteroid[a].position, mediumAsteroid[a].radius) && mediumAsteroid[a].active) gameOver = true;
	}

	for (int a = 0; a < MAX_SMALL_ASTEROID; a++)
	{
		if (CheckCollisionCircles({ player.collider.x, player.collider.y }, player.collider.z, smallAsteroid[a].position, smallAsteroid[a].radius) && smallAsteroid[a].active) gameOver = true;
	}
};

void drawShip()
{
	// Draw spaceship
	Vector2 v1 = { player.position.x + sinf(player.rotation * DEG2RAD) * (shipHeight), player.position.y - cosf(player.rotation * DEG2RAD) * (shipHeight) };
	Vector2 v2 = { player.position.x - cosf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2), player.position.y - sinf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2) };
	Vector2 v3 = { player.position.x + cosf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2), player.position.y + sinf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2) };
	DrawTriangle(v1, v2, v3, DARKGREEN);
};