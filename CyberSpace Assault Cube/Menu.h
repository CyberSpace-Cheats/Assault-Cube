#pragma once

namespace Menu
{
	void DrawMenu();
	void DrawLine(const ImVec2 from, const ImVec2 to, const ImVec4 color, const int thickness);
	void DrawFov();
	void DrawBox(const ImVec2& vec_start, const ImVec2& vec_end, const ImVec4 color, const float thickness);
}