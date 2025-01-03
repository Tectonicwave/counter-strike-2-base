#include "renderer.h"
#include "draw_manager.h"

void renderer_t::rect_outline(const sdk::vector2d& min, const sdk::vector2d& max, const sdk::color& col) const {
	draw->draw_list_active->AddRect(ImVec2(min.x, min.y), ImVec2(max.x, max.y), col.to_u32(), 0.0f, 0, 1.0f
	);
}

void renderer_t::rect(const sdk::vector2d& min, const sdk::vector2d& max, const sdk::color& col) const {
	draw->draw_list_active->AddRect(ImVec2(min.x, min.y), ImVec2(max.x, max.y), col.to_u32()
	);
}

void renderer_t::line(const sdk::vector2d& min, const sdk::vector2d& max, const sdk::color& col) const {
    draw->draw_list_active->AddLine(ImVec2(min.x, min.y), ImVec2(max.x, max.y), col.to_u32()
    );
}

void renderer_t::render_line(const sdk::vector2d& min, const sdk::vector2d& max, sdk::color aColor, float flThickness)
{
    draw->draw_list_active->AddLine(ImVec2(min.x, min.y), ImVec2(max.x, max.y), aColor.to_u32(), flThickness);
}

void renderer_t::rect_filled(const sdk::vector2d& min, const sdk::vector2d& max, const sdk::color& col) const {
    draw->draw_list_active->AddRectFilled(ImVec2(min.x, min.y), ImVec2(max.x, max.y), col.to_u32()
    );
}

void renderer_t::text(const sdk::vector2d& pos, const sdk::color& col, int flags, const std::string& text) const {
	draw->draw_list_active->AddText(ImVec2(pos.x, pos.y), col.to_u32(), text.c_str());
}

//yes i know there is col.to_u32()
ImColor renderer_t::GetU32(const sdk::color& aColor)
{
	// Convert float r, g, b, a (in range 0.0-1.0) to 0-255 integer range for ImColor
	return ImColor(
		static_cast<int>(aColor.r * 255), // Red
		static_cast<int>(aColor.g * 255), // Green
		static_cast<int>(aColor.b * 255), // Blue
		static_cast<int>(aColor.a * 255)  // Alpha
	);
}

void renderer_t::render_text(std::string strText, sdk::vector2d& vecTextPosition, sdk::color aTextColor, int flags, ImFont* pDrawFont)
{
    // Get the text size using ImVec2
    ImVec2 ImTextSize = pDrawFont->CalcTextSizeA(pDrawFont->FontSize, FLT_MAX, 0.0f, strText.c_str());
    if (!pDrawFont->ContainerAtlas)
        return;

    ImDrawList* OldDrawList = draw->draw_list_active;

    draw->draw_list_active->PushTextureID(pDrawFont->ContainerAtlas->TexID);

    // Handle alignment flags using sdk::vector2d
    if (flags & RIGHT_ALIGN)
        vecTextPosition.x -= ImTextSize.x;
    if (flags & CENTERED_X)
        vecTextPosition.x -= ImTextSize.x / 2.0f;
    if (flags & CENTERED_Y)
        vecTextPosition.y -= ImTextSize.y / 2.0f;

    // Handle outline
    if (flags & OUTLINE)
    {
        draw->draw_list_active->AddText(pDrawFont, pDrawFont->FontSize,
            ImVec2(vecTextPosition.x + 1, vecTextPosition.y + 1), GetU32(sdk::color(30, 30, 36, aTextColor.a)), strText.c_str());
        draw->draw_list_active->AddText(pDrawFont, pDrawFont->FontSize,
            ImVec2(vecTextPosition.x - 1, vecTextPosition.y - 1), GetU32(sdk::color(30, 30, 36, aTextColor.a)), strText.c_str());
        draw->draw_list_active->AddText(pDrawFont, pDrawFont->FontSize,
            ImVec2(vecTextPosition.x + 1, vecTextPosition.y - 1), GetU32(sdk::color(30, 30, 36, aTextColor.a)), strText.c_str());
        draw->draw_list_active->AddText(pDrawFont, pDrawFont->FontSize,
            ImVec2(vecTextPosition.x - 1, vecTextPosition.y + 1), GetU32(sdk::color(30, 30, 36, aTextColor.a)), strText.c_str());
    }

    // Handle drop shadow
    if (flags & DROP_SHADOW)
    {
        draw->draw_list_active->AddText(pDrawFont, pDrawFont->FontSize,
            ImVec2(vecTextPosition.x + 1, vecTextPosition.y + 1), GetU32(sdk::color(0, 0, 0, aTextColor.a)), strText.c_str());
    }

    // Render main text
    draw->draw_list_active->AddText(pDrawFont, pDrawFont->FontSize,
        ImVec2(vecTextPosition.x, vecTextPosition.y), GetU32(aTextColor), strText.c_str());

    draw->draw_list_active->PopTextureID();
}


void renderer_t::line(const ImVec2& start, const ImVec2& end, const sdk::color& col, float thickness = 1.0f) const {
	draw->draw_list_active->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), col.to_u32(), thickness
	);
}