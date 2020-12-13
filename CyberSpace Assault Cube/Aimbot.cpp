#include "includes.h"
#include "Hack.h"
#include "Aimbot.h"

extern bool b_aimbotteamcheck;
extern bool b_menu;

extern float smooth_aimbot;

bool ready = false;

Vec2 Smooth(Vec2 curAngles, Vec2 finalAngles, float smoothValue)
{
    Vec2 angles{ 0 };

    float PitchSmooth = (finalAngles.x - curAngles.x) / (smoothValue * 2);
    float YawSmooth = (finalAngles.y - curAngles.y) / (smoothValue * 2);

    angles.x = PitchSmooth;
    angles.y = YawSmooth;

    return angles;
}

void Aimbot::Aimbot(Player* LocalPlayer, std::vector<Player*> entList)
{
    Player* target = Hack::GetBestTarget(LocalPlayer, entList);
    if (target == nullptr || IsBadReadPtr(target, sizeof(target)))
    {
        return;
    }

    if (GetAsyncKeyState(VK_LBUTTON) && !b_menu)
    {
        if (b_aimbotteamcheck)
            if (LocalPlayer->TeamNum == target->TeamNum)
                return;
        Vec2 angles = Hack::CalcAngles(LocalPlayer->HeadPos, target->HeadPos);
        
        if (smooth_aimbot > 0)
            LocalPlayer->VisionAngle = (LocalPlayer->VisionAngle + Smooth(LocalPlayer->VisionAngle, angles, smooth_aimbot));
        else
            LocalPlayer->VisionAngle = angles;
    }
}