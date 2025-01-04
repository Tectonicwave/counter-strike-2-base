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

void renderer_t::render_text(const std::string_view strText, sdk::vector2d& vecTextPosition, sdk::color aTextColor, const int flags, const ImFont* pDrawFont)
{
    // Validate the font and its texture atlas. If any part is invalid, we can't render the text.
    if (!pDrawFont || !pDrawFont->ContainerAtlas || !pDrawFont->ContainerAtlas->TexID)
        return;

    // Push the font's texture ID to the draw list so it can be used for rendering.
    draw->draw_list_active->PushTextureID(pDrawFont->ContainerAtlas->TexID);

    // Calculate the size of the text using the font's size and the input string.
    // strText.data() provides a pointer to the underlying character data of the string view.
    ImVec2 ImTextSize = pDrawFont->CalcTextSizeA(pDrawFont->FontSize, FLT_MAX, 0.0f, strText.data());

    // If the calculated text size is invalid (width or height <= 0), we skip rendering.
    if (ImTextSize.x <= 0.0f || ImTextSize.y <= 0.0f)
        return;

    // Handle alignment flags.
    // Adjust the text position based on the specified alignment (right-aligned, centered, etc.).
    if (flags & RIGHT_ALIGN)
        vecTextPosition.x -= ImTextSize.x;
    if (flags & CENTERED_X)
        vecTextPosition.x -= ImTextSize.x / 2.0f;
    if (flags & CENTERED_Y)
        vecTextPosition.y -= ImTextSize.y / 2.0f;

    // Render text outline if the OUTLINE flag is set.
    // The outline is drawn by rendering the text multiple times with slight offsets.
    if (flags & OUTLINE)
    {
        sdk::color outlineColor(30, 30, 36, aTextColor.a);
        draw->draw_list_active->AddText(pDrawFont, pDrawFont->FontSize, ImVec2(vecTextPosition.x + 1, vecTextPosition.y + 1), GetU32(outlineColor), strText.data());
        draw->draw_list_active->AddText(pDrawFont, pDrawFont->FontSize, ImVec2(vecTextPosition.x - 1, vecTextPosition.y - 1), GetU32(outlineColor), strText.data());
        draw->draw_list_active->AddText(pDrawFont, pDrawFont->FontSize, ImVec2(vecTextPosition.x + 1, vecTextPosition.y - 1), GetU32(outlineColor), strText.data());
        draw->draw_list_active->AddText(pDrawFont, pDrawFont->FontSize, ImVec2(vecTextPosition.x - 1, vecTextPosition.y + 1), GetU32(outlineColor), strText.data());
    }

    // Render drop shadow if the DROP_SHADOW flag is set.
    // The shadow is drawn as a single offset text with a darker color.
    if (flags & DROP_SHADOW)
    {
        sdk::color shadowColor(0, 0, 0, aTextColor.a);
        draw->draw_list_active->AddText(pDrawFont, pDrawFont->FontSize, ImVec2(vecTextPosition.x + 1, vecTextPosition.y + 1), GetU32(shadowColor), strText.data());
    }

    // Render the main text using the provided font and color.
    draw->draw_list_active->AddText(pDrawFont, pDrawFont->FontSize, ImVec2(vecTextPosition.x, vecTextPosition.y), GetU32(aTextColor), strText.data());

    // Restore the original texture ID for the draw list.
    draw->draw_list_active->PopTextureID();
}

/*
Why we changed from std::string to const std::string_view:
1. *Stability and Safety:
   - `CalcTextSizeA` expects `const char*` pointers (`text_begin` and `text_end`) for input. Using `std::string` could lead to temporary or invalid memory being passed, particularly in cases of implicit conversions or when using temporary string objects.
   - `std::string_view` guarantees that the memory being referenced remains valid for the duration of the function, preventing crashes or undefined behavior.

2. *Compatibility with `CalcTextSizeA`:
   - The implementation of `CalcTextSizeA` relies on precise control over the `text_end` pointer. `std::string_view` allows us to directly provide both `text_begin` and `text_end` as valid pointers without additional conversions or operations.

3. *UTF-8 Safety:
   - The decoding logic within `CalcTextSizeA` requires stable, valid UTF-8 encoded memory. `std::string_view` ensures this stability while avoiding overhead.
*/

void renderer_t::line(const ImVec2& start, const ImVec2& end, const sdk::color& col, float thickness = 1.0f) const {
	draw->draw_list_active->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), col.to_u32(), thickness
	);
}