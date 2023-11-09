#pragma once
#include "framework.h"
#include "GameObject.h"	
#include "DataD3D12.h"
#include "RenderWindow.h"

class Player
{
public:
								Player();
								~Player();

	void						Init();
	void						Update();

private:
	GameObject gameObject;
	GameTimer timer;

	bool drawPlayer = true;

};