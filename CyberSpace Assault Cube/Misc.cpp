#include "includes.h"
#include "Hack.h"
#include "Misc.h"

extern bool b_rapidfire;
extern bool b_infiniteammo;
extern bool b_infinitehealth;
extern bool b_paperwall;

void Misc::PapersWall()
{
	if (b_paperwall)
		glDisable(GL_DEPTH_TEST);
}

void Misc::InfiniteAmmo()
{
	if (b_infiniteammo)
	{
		Hack::Patch((uintptr_t)Hack::dwdecAmmo, 2, "\0");
	}
	else
	{
		Hack::Patch((uintptr_t)Hack::dwdecAmmo, 2, "\xFF\x0E");
	}
}

void Misc::RapidFire()
{
	if (b_rapidfire)
	{
		Hack::Patch((uintptr_t)Hack::dwRapidFire, 2, "\0");
	}
	else
	{
		Hack::Patch((uintptr_t)Hack::dwRapidFire, 2, "\x89\x0A");
	}
}

void Misc::InfHealth(Player* localPlayer)
{
	if (b_infinitehealth)
	{
		localPlayer->Health = 999;
		localPlayer->Armor = 999;
	}
}