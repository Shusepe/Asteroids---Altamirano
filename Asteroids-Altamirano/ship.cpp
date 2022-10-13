#include "ship.h"
#include "asteroid.h"
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
	player.acceleration = 0;
	player.lives = 3;
	player.rotation = 0;
	player.collider = { (float)(player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f)), (float)(player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f)), 12 };
	player.color = LIGHTGRAY;
};

void playerMovement() 
{
	// Player logic: movement
	//nuevaPosNave = posNave + aceleracionNave * tiempoEntreFrames

	player.position.x += (player.speed.x * player.acceleration);
	player.position.y -= (player.speed.y * player.acceleration);
};

void playerAceleration() 
{
	//direccionNormalizada = vectorDireccion / modulo(vectorDireccion)
	// sumar aceleración en esa dirección 
	//aceleracionNave += direccionNormalizada
	
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

//void MovePlayer(Spaceship& player, Vector2 mousePos)
//{
//	Vector2 direcVector = { mousePos.x - player.body.x, mousePos.y - player.body.y };
//	Vector2 normVector = Vector2Normalize(direcVector);
//
//	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
//	{
//		player.aceleration.x += normVector.xGetFrameTime() / 6.0f;
//		player.aceleration.y += normVector.yGetFrameTime() / 6.0f;
//		AcelerationLimitator(player.aceleration.x);
//		AcelerationLimitator(player.aceleration.y);
//	}
//	player.body.x += player.aceleration.x;
//	player.body.y += player.aceleration.y;
//
//	WarpCoords(player) (Colision del jugador contra las paredes);
//}

//void AcelerationLimitator(float& aceleration)
//{
//	if (aceleration > 0.35f)
//	{
//		aceleration = 0.35f;
//	}
//	else if (aceleration < -0.35f)
//	{
//		aceleration = -0.35f;
//	}
//}

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
	shipImpacted = false;

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
		player.acceleration = 0;
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