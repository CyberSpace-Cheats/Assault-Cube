#include "includes.h"
#include "Hack.h"
#include "Misc.h"
#include "Aimbot.h"
#include "ESP.h"
#include "Menu.h"

extern bool b_aimbot;
extern bool b_drawline;
extern bool b_espteamcheck;
extern bool b_boxesp;

extern bool b_drawfov;

extern int fov_aimbot;

void Hack::Init()
{
	gameModule = reinterpret_cast<uintptr_t>(GetModuleHandleA("ac_client.exe"));
}

void Hack::Loop()
{
    uintptr_t tLocalPlayer = *reinterpret_cast<uintptr_t*>(gameModule + dwLocalPlayer);

    float viewMatrix[16];

    memcpy(&viewMatrix, (PBYTE*)(dwviewMatrix), sizeof(viewMatrix));

    Player* LocalPlayer = reinterpret_cast<Player*>(tLocalPlayer);

    uintptr_t tEntList = *reinterpret_cast<uintptr_t*>(gameModule + dwEntityList);

    int playerCount = *reinterpret_cast<int*>(dwPlayerCount);

    int players_alive = 0;

    if (!IsBadReadPtr(LocalPlayer, sizeof(LocalPlayer)))
    {
        Misc::InfiniteAmmo();
        Misc::InfHealth(LocalPlayer);
        Misc::PapersWall();
        Misc::RapidFire();

        if (b_drawfov)
            Menu::DrawFov();

        if (playerCount > 0)
        {
            for (int i = 0; i < playerCount; i++)
            {
                uintptr_t _ent = *reinterpret_cast<uintptr_t*>(tEntList + (i * 4));

                if (_ent > 0)
                {
                    Player* ent = reinterpret_cast<Player*>(_ent);

                    if (!IsBadReadPtr(ent, sizeof(ent)))
                    {
                        if (ent->Health > 0)
                        {
                            if (b_drawline)
                                ESP::espLine(LocalPlayer, ent, viewMatrix, b_espteamcheck);
                            
                            if (b_boxesp)
                                ESP::drawBox(LocalPlayer, ent, viewMatrix, b_espteamcheck);

                            players_alive++;
                            entListVector.push_back(ent);
                        }
                    }
                }

            }

            if ( b_aimbot && ( entListVector.size() - players_alive ) == 0 )
            {
                Aimbot::Aimbot(LocalPlayer, entListVector);
            }

            entListVector.clear();
        }

    }
}

bool Hack::Patch(uintptr_t baseAddress, int len, const char* bytes)
{
    auto ret = false;
    DWORD cur = 0;
    int nop = 0x90;
    VirtualProtect((LPVOID)baseAddress, len, PAGE_EXECUTE_READWRITE, &cur);

    if (bytes[0] == 0)
    {
        if (memset((void*)baseAddress, nop, len))
            ret = true;
    }

    if (bytes[0] != 0)
    {
        if (memcpy((void*)baseAddress, bytes, len))
            ret = true;
    }

    VirtualProtect((LPVOID)baseAddress, len, cur, &cur);
    return ret;
}

float Hack::GetDistance(Vec3 mypos, Vec3 enemy)
{
    float dx, dy, dz;
    dx = enemy.x - mypos.x;
    dy = enemy.y - mypos.y;
    dz = enemy.z - mypos.z;
    float distance = sqrt(pow(dx, 2) + pow(dz, 2) + pow(dy, 2));
    return distance;
}

float Hack::GetDistance2D(Vec2 mypos, Vec2 enemy)
{
    float dx, dy;
    dx = enemy.x - mypos.x;
    dy = enemy.y - mypos.y;
    float distance = sqrt(pow(dx, 2) + pow(dy, 2));
    return distance;
}

Vec2 Hack::CalcAngles(Vec3 src, Vec3 dst)
{
    Vec2 angles;

    angles.x = -atan2f(dst.x - src.x, dst.y - src.y) / (float)M_PI * 180.0f + 180.0f;
    angles.y = asinf((dst.z - src.z) / GetDistance(src, dst)) * 180 / (float)M_PI;

    return angles;
}

Player* Hack::GetBestTarget(Player* LocalPlayer, std::vector<Player*> entlist)
{
    Player* target = nullptr;
    float oldDistance = FLT_MAX;
    float newDistance = 0;

    for (auto ent : entlist)
    {
        Vec2 _angles = CalcAngles( LocalPlayer->HeadPos, ent->HeadPos );
        newDistance = GetDistance2D( LocalPlayer->VisionAngle, _angles );
        if (( newDistance < oldDistance ) && ( (newDistance < fov_aimbot && newDistance > -fov_aimbot) ))
        {
            oldDistance = newDistance;
            target = ent;
        }
    }
    return target;
}

bool Hack::WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight)
{
    Vec4 clipCoords;
    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
    clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
    clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

    if (clipCoords.w < 0.1f)
        return false;

    Vec3 NDC;
    NDC.x = clipCoords.x / clipCoords.w;
    NDC.y = clipCoords.y / clipCoords.w;
    NDC.z = clipCoords.z / clipCoords.w;

    screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
    screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
    return true;
}