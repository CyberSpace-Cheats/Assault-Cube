#include "includes.h"
#include "Hack.h"
#include "Menu.h"

#define ABS( x ) ( ( x < 0 ) ? ( -x ) : ( x ) )
#define DEG2RAD( x ) ( ( float )( x ) * ( M_PI / 180.f ) )

extern bool b_menu;

extern int windowWidth;
extern int windowHeight;

auto windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

int index = 0;

bool b_rapidfire = false;
bool b_infiniteammo = false;
bool b_infinitehealth = false;
bool b_paperwall = false;
bool b_aimbot = false;
bool b_aimbotteamcheck = false;
bool b_drawline = false;
bool b_espteamcheck = false;
bool b_boxesp = false;
bool b_healthbar = false;

bool b_drawfov = false;

int fov_aimbot = 5;

float smooth_aimbot = 0;

void Menu::DrawMenu()
{    
    if (b_menu)
    {
        ImGui::Begin("CyberSpace - AC", nullptr, windowFlags);
        ImVec2 size = ImGui::GetWindowSize();
        ImGui::SameLine((size.x / 2) - (ImGui::CalcTextSize("CyberSpace").x / 2));
        {
            ImGui::Text("CyberSpace");

            static float x = 65, y = 122;

            ImGui::BeginChild(("first penis"), ImVec2(x, y), true);
            {
                if (ImGui::Button("Aimbot", ImVec2(55, 20)))
                    index = 1;

                if (ImGui::Button("ESP", ImVec2(55, 20)))
                    index = 2;

                if (ImGui::Button("Misc", ImVec2(55, 20)))
                    index = 0;
            }
            ImGui::EndChild();

            switch (index)
            {
            case 0:
                y = 155;
                ImGui::SameLine(78.f);
                ImGui::BeginChild(("double penis"), ImVec2(380, y), true);
                {
                    ImGui::Checkbox("Rapid Fire", &b_rapidfire);
                    ImGui::Checkbox("Infinite Ammo", &b_infiniteammo);
                    ImGui::Checkbox("Infinite Health", &b_infinitehealth);
                    ImGui::Checkbox("Papers Wall", &b_paperwall);
                    ImGui::Separator();
                    if (ImGui::Button("UnHook"))
                    {
                        b_menu = false;
                        Detours::UnHook();
                        Detours::ReleaseMouseHook();
                        SendLog("CyberSpace", "Unloaded...", 3);
                    }
                }
                ImGui::EndChild();
                break;
            case 1:
                y = 155;
                ImGui::SameLine(78.f);
                ImGui::BeginChild(("triple penis"), ImVec2(380, y), true);
                {
                    ImGui::Checkbox("Aimbot", &b_aimbot);
                    ImGui::Checkbox("Draw Fov", &b_drawfov);
                    ImGui::SliderInt("Fov Aimbot", &fov_aimbot, 0, 25);
                    ImGui::SliderFloat("Smooth Aimbot", &smooth_aimbot, 0, 15);
                    ImGui::Checkbox("Team Check", &b_aimbotteamcheck);
                }
                ImGui::EndChild();
                break;
            case 2:
                y = 155;
                ImGui::SameLine(78.f);
                ImGui::BeginChild(("quatro penis"), ImVec2(380, y), true);
                {
                    ImGui::Checkbox("Line ESP", &b_drawline);
                    ImGui::Checkbox("Box ESP", &b_boxesp);
                    ImGui::Checkbox("Health Bar", &b_healthbar);
                    ImGui::Checkbox("Team Check", &b_espteamcheck);
                }
                ImGui::EndChild();
                break;
            }
        }
        ImGui::End();
    }
}

void Menu::DrawLine(const ImVec2 from, const ImVec2 to, const ImVec4 color, const int thickness)
{
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Line", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration))
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        draw_list->AddLine(from, to, IM_COL32(color.x, color.y, color.z, color.w), thickness);
    }
    ImGui::End();
}

void Menu::DrawFov()
{
    float radius = std::tanf(DEG2RAD(fov_aimbot) / 2) / std::tanf(DEG2RAD(90) / 2) * windowWidth;
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Fov", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration))
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        draw_list->AddCircle(ImVec2(windowWidth/2, windowHeight/2), radius, IM_COL32(255, 255, 255, 255), 0, 1);
    }
    ImGui::End();
}

void Menu::DrawBox(const ImVec2& vec_start, const ImVec2& vec_end, const ImVec4 color, const float thickness)
{
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Box", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration))
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        draw_list->AddRect(vec_start, ImVec2(vec_start.x + vec_end.x, vec_start.y + vec_end.y), ImGui::GetColorU32(color), 0, 0, thickness);
    }
    ImGui::End();
}