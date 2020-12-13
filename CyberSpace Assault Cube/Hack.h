#pragma once

struct Vec4
{
    float x, y, z, w;
};

struct Vec3
{
    float x, y, z;

    Vec3 operator+(Vec3 d)
    {
        return { x + d.x, y + d.y, z + d.z };
    }

    Vec3 operator-(Vec3 d)
    {
        return { x - d.x, y - d.y, z - d.z };
    }
};

struct Vec2
{
    float x, y;

    Vec2 operator+(Vec2 d)
    {
        return { x + d.x, y + d.y };
    }

    Vec2 operator-(Vec2 d)
    {
        return { x - d.x, y - d.y };
    }
};

class Player
{
public:
    char pad_0000[4]; //0x0000
    Vec3 HeadPos; //0x0004
    Vec3 Velocity; //0x0010
    char pad_001C[24]; //0x001C
    Vec3 Position; //0x0034
    Vec2 VisionAngle; //0x0040
    char pad_0048[176]; //0x0048
    int32_t Health; //0x00F8
    int32_t Armor; //0x00FC
    char pad_0100[292]; //0x0100
    char Name[16]; //0x0224
    char pad_0234[248]; //0x0234
    int32_t TeamNum; //0x032C
}; //Size: 0x0330

static std::vector<Player*> entListVector{ 0 };

namespace Hack
{
    static uintptr_t gameModule = 0;

    static uintptr_t dwLocalPlayer = 0x109B74;
    static uintptr_t dwEntityList = 0x10F4F8;
    static uintptr_t dwPlayerCount = 0x50F500;
    static uintptr_t dwdecAmmo = 0x4637E9;
    static uintptr_t dwRapidFire = 0x4637E4;
    static uintptr_t dwNoRecoil = 0x463786;
    static uintptr_t dwviewMatrix = 0x501AE8;

	void Init();
	void Loop();
    bool Patch(uintptr_t baseAddress, int len, const char* bytes);
    float GetDistance(Vec3 mypos, Vec3 enemy);
    float GetDistance2D(Vec2 mypos, Vec2 enemy);
    Player* GetBestTarget(Player* LocalPlayer, std::vector<Player*> entlist);
    Vec2 CalcAngles(Vec3 src, Vec3 dst);
    bool WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight);
};