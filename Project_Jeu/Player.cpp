#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	gameObject.BuildRenderOpCircle();
}

void Player::Update()
{
	if (drawPlayer)
	{
		Init();
		drawPlayer = false;
	}
}
