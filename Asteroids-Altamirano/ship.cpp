#include "ship.h"
#include "asteroid.h"
#include "raymath.h"
#include "game.h"

extern float screenWidth;
extern float screenHeight;

extern bool shipImpacted;
extern bool gameOver;

float shipHeight = 0.0f;

static const char shipImgUrl[] = "res/img/ship.png";

static const char explodeSfxUrl[] = "res/sfx/explosionDeNave.wav";

Player player;

void initPlayer()
{
	shipHeight = (PLAYER_BASE_SIZE / 2) / tanf(20 * DEG2RAD);
	
	// Initialization player
	player.sprite = LoadTexture(shipImgUrl);
	player.explodeSfx = LoadSound(explodeSfxUrl);
	player.position = { (float) (screenWidth / 2), (float) (screenHeight / 2 - shipHeight / 2) };
	player.speed = { 0, 0 };
	player.acceleration = { 0,0 };
	player.lives = 3;
	player.rotation = 0;
	player.collider = { (float)(player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f)), (float)(player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f)), 12 };
	player.color = LIGHTGRAY;
};

//void playerMovement() 
//{
//	// Player logic: movement
//	//nuevaPosNave = posNave + aceleracionNave * tiempoEntreFrames
//
//	player.position.x += (player.speed.x * player.acceleration);
//	player.position.y -= (player.speed.y * player.acceleration);
//};

//void playerAceleration() 
//{
//	//direccionNormalizada = vectorDireccion / modulo(vectorDireccion)
//	// sumar aceleración en esa dirección 
//	//aceleracionNave += direccionNormalizada
//	
//	// Player logic: acceleration
//	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
//	{
//		if (player.acceleration < 1) player.acceleration += 0.04f;
//	}
//	else
//	{
//		if (player.acceleration > 0) player.acceleration -= 0.02f;
//		else if (player.acceleration < 0) player.acceleration = 0;
//	}
//};

void playerRotation() 
{
	// Player logic: rotation
	player.rotation = Vector2AngleCustom(player.position, GetMousePosition()) + 90;
};

void playerSpeed() 
{
	// Player logic: speed
	player.speed.x = sin(player.rotation * DEG2RAD) * PLAYER_SPEED;
	player.speed.y = cos(player.rotation * DEG2RAD) * PLAYER_SPEED;
};

void movePlayer()
{
	Vector2 dirVector = { GetMouseX() - player.position.x, GetMouseY() - player.position.y };
	Vector2 normVector = Vector2Normalize(dirVector);

	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		player.acceleration.x += playerAcceleration(normVector.x);
		player.acceleration.y += playerAcceleration(normVector.y);
	}
	player.position.x += player.acceleration.x * GetFrameTime();
	player.position.y += player.acceleration.y * GetFrameTime();
}

float playerAcceleration(float acceleration)
{
	float finalAcceleration = acceleration;

	if (acceleration > 0.7f)
	{
		finalAcceleration = 0.7f;
	}
	else if (acceleration < -0.7f)
	{
		finalAcceleration = -0.7f;
	}

	return finalAcceleration;
}

void playerBullet(Bullet bullet[])
{
	// Player shoot logic
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		for (int i = 0; i < PLAYER_MAX_BULLET; i++)
		{
			if (!bullet[i].active)
			{
				SetSoundPitch(bullet[i].shootSfx, ((float)GetRandomValue(0, 15) / 100) + 1);
				PlaySound(bullet[i].shootSfx);
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
		if (CheckCollisionCircles({ player.collider.x, player.collider.y }, player.collider.z, bigAsteroid[a].position, bigAsteroid[a].radius) && bigAsteroid[a].active)
		{
			SetSoundPitch(player.explodeSfx, 1.2f);
			PlaySound(player.explodeSfx);
			shipImpacted = true;
		}
	}

	for (int a = 0; a < MAX_MEDIUM_ASTEROID; a++)
	{
		if (CheckCollisionCircles({ player.collider.x, player.collider.y }, player.collider.z, mediumAsteroid[a].position, mediumAsteroid[a].radius) && mediumAsteroid[a].active) 
		{
			SetSoundPitch(player.explodeSfx, 1.2f);
			PlaySound(player.explodeSfx);
			shipImpacted = true;
		}
	}

	for (int a = 0; a < MAX_SMALL_ASTEROID; a++)
	{
		if (CheckCollisionCircles({ player.collider.x, player.collider.y }, player.collider.z, smallAsteroid[a].position, smallAsteroid[a].radius) && smallAsteroid[a].active)
		{
			SetSoundPitch(player.explodeSfx, 1.2f);
			PlaySound(player.explodeSfx);
			shipImpacted = true;
		}
	}

	if (shipImpacted)
	{
		player.lives -= 1;

		player.position = { (float)(screenWidth / 2), (float)(screenHeight / 2 - shipHeight / 2) };
		player.speed = { 0, 0 };
		player.acceleration = { 0,0 };
		player.rotation = 0;
		player.collider = { (float)(player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f)), (float)(player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f)), 12 };
		
	}

	if (player.lives <= 0)
	{
		gameOver = true;
	}

};

void drawShip()
{
	// Draw spaceship
	DrawTexturePro(
		player.sprite,
		Rectangle{ 0,0,(float)player.sprite.width,(float)player.sprite.height },
		Rectangle{ player.position.x, player.position.y, (float)player.sprite.width * 1.0f,(float)player.sprite.height * 1.0f },
		Vector2{ ((float)player.sprite.width * 1.0f) / 2, ((float)player.sprite.height * 1.0f) / 2 },
		player.rotation,
		player.color);
};