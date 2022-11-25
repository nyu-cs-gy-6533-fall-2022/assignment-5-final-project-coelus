
#pragma once
#include "Creature.h"

#ifndef _PLAYER_
#define _PLAYER_

class Player : public Creature
{
public:
	Player(SoundSystem *sndSys, vector<Shader*> &s, double &time);
	void Update(Control ctrl);

private:
	void loadData();
};
#endif