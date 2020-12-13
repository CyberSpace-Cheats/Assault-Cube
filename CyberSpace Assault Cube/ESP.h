#pragma once

namespace ESP
{
	void espLine(Player* LocalPlayer, Player* ent, float matrix[16], bool teamcheck);
	void drawBox(Player* LocalPlayer, Player* ent, float matrix[16], bool teamcheck);
}