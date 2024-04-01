#pragma once

#include <iostream>

struct SpriteAnimation
{
	uint8_t index;
	uint8_t frames;
	uint8_t speed;
	uint16_t spriteWidth;
	uint16_t spriteHeight;

	SpriteAnimation()
		:index(0), frames(0), speed(0), spriteWidth(0), spriteHeight(0)
	{}
	SpriteAnimation(uint8_t i, uint8_t f, uint8_t s, uint16_t w, uint16_t h)
		:index(i), frames(f), speed(s), spriteWidth(w), spriteHeight(h)
	{}

};