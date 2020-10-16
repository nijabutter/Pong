#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <cmath>
#include "RenderWindow.h"

const int SCREEN_HEIGHT = 720;
const int SCREEN_WIDTH = 1280;
const int CENTER_X = SCREEN_WIDTH / 2;
const int CENTER_Y = SCREEN_HEIGHT / 2;
const int PADDEL_WIDTH = 20;
const int PADDEL_HEIGHT = 200;
const int BALL_WIDTH = 10;
const int BALL_HEIGHT = 10;
const int BALL_ACCELLERATION = 5;
const int BALL_MINSPEED = 15;
const int BALL_MAXSPEED = 30;
const int PADDEL_SPEED = 10;

short l_paddel_color = 90;
short r_paddel_color = 90;

int ballVelocityX = BALL_MINSPEED;
int ballVelocityY = BALL_MINSPEED;
RenderWindow window;

bool gameRunning = true;
bool about = true;
bool wdown = false;
bool sdown = false;

int playerScore = 0;
int enemyScore = 0;

SDL_Rect ball;
SDL_Rect l_paddel;
SDL_Rect r_paddel;

Mix_Chunk* hitSfx;
Mix_Chunk* scoreSfx;

TTF_Font* font;

void init()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	font = TTF_OpenFont("res/ttf/Cozette.ttf", 50);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	hitSfx = Mix_LoadWAV("res/sfx/hit.wav");
	scoreSfx = Mix_LoadWAV("res/sfx/score.wav");

	window.create("Pong", SCREEN_WIDTH, SCREEN_HEIGHT);
	ball.x = CENTER_X - BALL_WIDTH / 2;
	ball.y = CENTER_Y - BALL_HEIGHT / 2;
	ball.w = BALL_WIDTH;
	ball.h = BALL_HEIGHT;

	l_paddel.x = PADDEL_WIDTH; // distance from left border
	l_paddel.y = CENTER_Y - PADDEL_HEIGHT / 2;
	l_paddel.w = PADDEL_WIDTH;
	l_paddel.h = PADDEL_HEIGHT;

	r_paddel.x = SCREEN_WIDTH - PADDEL_WIDTH*2; // distance from right border
	r_paddel.y = CENTER_Y - PADDEL_HEIGHT / 2;
	r_paddel.w = PADDEL_WIDTH;
	r_paddel.h = PADDEL_HEIGHT;
}

void resetBall()
{
	ball.x = CENTER_X - BALL_WIDTH / 2;
	ball.y = CENTER_Y - BALL_HEIGHT / 2;
}

void Input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				gameRunning = false;
				break;
			}
			case SDL_KEYDOWN:
			{
				switch (event.key.keysym.scancode)
				{
					case SDL_SCANCODE_W:
					{
						wdown = true;
						break;
					}
					case SDL_SCANCODE_S:
					{
						sdown = true;
						break;
					}
					default: {break;}
				}
				break;
			}
			case SDL_KEYUP:
			{
				switch(event.key.keysym.scancode)
				{
					case SDL_SCANCODE_W:
					{
						wdown = false;
						break;
					}
					case SDL_SCANCODE_S:
					{
						sdown = false;
						break;
					}
					default: {break;}
				}
				break;
			}
			default:
			{
				break;
			}
		}
	}
	if (wdown && !sdown)
	{
		if (l_paddel.y > 0) // top clamp
			l_paddel.y -= PADDEL_SPEED;
	}
	if (sdown && !wdown)
	{
		if (l_paddel.y < SCREEN_HEIGHT - PADDEL_HEIGHT) // bottom clamp
			l_paddel.y += PADDEL_SPEED;
	}
}

void Update()
{

	if (ballVelocityX > BALL_MINSPEED) {
		ballVelocityX--;

	}
	if (ballVelocityY > BALL_MINSPEED) {
		ballVelocityY--;

	}
	if (ball.x <= l_paddel.x+PADDEL_WIDTH && ball.x >= l_paddel.x && ball.y <= l_paddel.y+PADDEL_HEIGHT && ball.y >= l_paddel.y)
	{
		//collided with left paddle
		ballVelocityX = -ballVelocityX;
		if (ballVelocityX < 0) {
			ballVelocityX -= BALL_ACCELLERATION;
		}

		else {
			ballVelocityX += BALL_ACCELLERATION;

		}

		std::cout<<"left paddle"<<std::endl;
		l_paddel_color = 255;
		Mix_PlayChannel(-1, hitSfx, 0);
	}
	else if (ball.x <= r_paddel.x+PADDEL_WIDTH && ball.x >= r_paddel.x && ball.y <= r_paddel.y+PADDEL_HEIGHT && ball.y >= r_paddel.y)
	{
		// collided with right paddle
		ballVelocityX = -ballVelocityX;
		if (ballVelocityX < 0) {
			ballVelocityX -= BALL_ACCELLERATION;
		}

		else {
			ballVelocityX += BALL_ACCELLERATION;

		}

		std::cout<<"right paddle"<<std::endl;
		r_paddel_color = 255;
		Mix_PlayChannel(-1, hitSfx, 0);
	}
	else if (ball.x+BALL_WIDTH <= 0)
	{
		// off screen left
		ballVelocityX = BALL_MINSPEED;
		ballVelocityY = -BALL_MINSPEED;
		resetBall();
		enemyScore++;
		std::cout<<"off screen left"<<std::endl;
		Mix_PlayChannel(-1, scoreSfx, 0);
	}
	else if (ball.x >= SCREEN_WIDTH)
	{
		// off screen right
		ballVelocityX = -BALL_MINSPEED;
		ballVelocityY = BALL_MINSPEED;
		resetBall();
		playerScore++;
		std::cout<<"off screen right"<<std::endl;
		Mix_PlayChannel(-1, scoreSfx, 0);
	}
	else if (ball.y <= 0)
	{
		// collided with top of screen
		ballVelocityY = -ballVelocityY;
		if (ballVelocityY < 0) {
			ballVelocityY -= BALL_ACCELLERATION;
		}

		else {
			ballVelocityY += BALL_ACCELLERATION;

		}
		std::cout<<"top screen"<<std::endl;
	}
	else if (ball.y+BALL_HEIGHT >= SCREEN_HEIGHT)
	{
		// collided with bottom of screen
		ballVelocityY = -ballVelocityY;
		if (ballVelocityY < 0) {
			ballVelocityY -= BALL_ACCELLERATION;
		}

		else {
			ballVelocityY+= BALL_ACCELLERATION;

		}
		std::cout<<"bottom of screen"<<std::endl;
	}
	else
	{
		r_paddel_color = 90;
		l_paddel_color = 90;
	}
	ball.x += ballVelocityX;
	ball.y+= ballVelocityY;

	if (ballVelocityX > 0)
	{
		int distanceToWall = SCREEN_WIDTH - BALL_WIDTH - ball.x;
		int time = distanceToWall / (ballVelocityX * PADDEL_SPEED * 0.25 );
		int newY = ball.y + (ballVelocityY * PADDEL_SPEED * 0.5) * time;
		if (newY > SCREEN_HEIGHT)
			newY = SCREEN_HEIGHT;
		if (newY < 0)
			newY= 0;
		if (newY < r_paddel.y + (PADDEL_HEIGHT / 2)) {
			if (r_paddel.y > 0)
				r_paddel.y -= PADDEL_SPEED ;
		}
		else
		{
			if (r_paddel.y + PADDEL_HEIGHT < SCREEN_HEIGHT)
				r_paddel.y += PADDEL_SPEED ;
		}
	}
}

void Render()
{
	window.clear();
	window.render(CENTER_X/2, CENTER_Y, std::to_string(playerScore).c_str(), font, {60, 60, 60, 255});
	window.render(CENTER_X/2*3, CENTER_Y, std::to_string(enemyScore).c_str(), font, {60, 60, 60, 255});
	window.render(&ball, 255, 255, 255, 255);
	window.render(&l_paddel, l_paddel_color, l_paddel_color, l_paddel_color, 255);
	window.render(&r_paddel, r_paddel_color, r_paddel_color, r_paddel_color, 255);
	window.setColor(0, 0, 0, 255);
	window.display();
}

void About()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			gameRunning = false;
		else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			about = false;
	}
	window.clear();
	window.render(CENTER_X, CENTER_Y, "Use W to move up and S to move down", font, {0, 0, 0, 255});
	window.render(CENTER_X, CENTER_Y+100, "Press ESC to play", font, {0, 0, 0, 255});
	window.setColor(90, 90, 90, 255);
	window.display();
}

int main(int argc, char* args[])
{
	init();
	while (gameRunning) {
		if (about) {
			About();
			SDL_Delay(16);
		}
		else
		{
			Input();
			Update();
			Render();
			SDL_Delay(16);
		}

	}
	window.cleanUp();
	SDL_Quit();
	return 0;
}
