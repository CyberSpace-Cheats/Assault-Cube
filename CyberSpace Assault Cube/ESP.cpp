#include "includes.h"
#include "Hack.h"
#include "ESP.h"
#include "Menu.h"

#define ABS(x) ((x < 0) ? (-x) : (x))

extern int windowWidth;
extern int windowHeight;

void ESP::espLine(Player* LocalPlayer, Player* ent, float matrix[16], bool teamcheck)
{
	Vec2 vScreen{ 0 };
	Vec3 enemyPos = ent->Position;
	if (Hack::WorldToScreen(enemyPos, vScreen, matrix, windowWidth, windowHeight))
	{
		ImVec2 _screen = { vScreen.x, vScreen.y };
		
		if (teamcheck)
			if (LocalPlayer->TeamNum == ent->TeamNum)
				Menu::DrawLine(ImVec2(windowWidth / 2, windowHeight), _screen, ImVec4(0, 0, 255, 255), 1);
			else
				Menu::DrawLine(ImVec2(windowWidth / 2, windowHeight), _screen, ImVec4(255, 0, 0, 255), 1);
		else
			Menu::DrawLine(ImVec2(windowWidth / 2, windowHeight), _screen, ImVec4(0, 255, 0, 255), 1);
	}
};

void ESP::drawBox(Player* LocalPlayer, Player* ent, float matrix[16], bool teamcheck)
{
	Vec2 vScreen{ 0 };
	Vec2 vHead{ 0 };

	Vec3 enemyPos = ent->Position;
	Vec3 headPos = ent->HeadPos;

	if (Hack::WorldToScreen(enemyPos, vScreen, matrix, windowWidth, windowHeight))
	{
		if (Hack::WorldToScreen(headPos, vHead, matrix, windowWidth, windowHeight))
		{
			auto height = ABS(vScreen.y - vHead.y);
			auto width = height / 2;
			auto x = vHead.x - (width / 2);
			auto y = vHead.y;

			if (teamcheck)
				if (LocalPlayer->TeamNum == ent->TeamNum)
					Menu::DrawBox(ImVec2(x, y), ImVec2(width, height), ImVec4(0, 0, 255, 255), 2.f);
				else
					Menu::DrawBox(ImVec2(x, y), ImVec2(width, height), ImVec4(255, 0, 0, 255), 2.f);
			else
				Menu::DrawBox(ImVec2(x, y), ImVec2(width, height), ImVec4(0, 255, 0, 255), 1.f);
		}
	}
}