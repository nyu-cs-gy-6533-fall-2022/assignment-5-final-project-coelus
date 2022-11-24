
#pragma once
#include "Creature.h"
using namespace glm;

#ifndef _PLAYER_
#define _PLAYER_

class Player : public Creature
{
public:
	Player(SoundSystem *sndSys, Shader *s, double &time);
	void Update(Control ctrl);

private:
	bool canJumpAttack;
	int ctrlX;
	DefferedKey dAttack, dChain, dJump;
	void loadData();
};
#endif