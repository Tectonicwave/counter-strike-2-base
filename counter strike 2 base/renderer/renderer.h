#pragma once
#include <imgui_internal.h>
#include "../manager.h"

#include "../sdk/maths/color.h"

enum text_render_flags
{
	NO_TFLAG = 0,
	CENTERED_X = 1 << 1,
	CENTERED_Y = 1 << 2,
	CENTERED_ALL = 1 << 3,
	RIGHT_ALIGN = 1 << 4,
	OUTLINE = 1 << 5,
	DROP_SHADOW = 1 << 6
};

class renderer_t
{
public:
	void rect_outline(const sdk::vector2d& min, const sdk::vector2d& max, const sdk::color& col) const;

	void rect(const sdk::vector2d& min, const sdk::vector2d& max, const sdk::color& col) const;

	void line(const sdk::vector2d& min, const sdk::vector2d& max, const sdk::color& col) const;

	void rect_filled(const sdk::vector2d& min, const sdk::vector2d& max, const sdk::color& col) const;

	void text(const sdk::vector2d& pos, const sdk::color& col, int flags, const std::string& text) const;

	ImColor GetU32(const sdk::color& aColor);

	void render_text(std::string strText, sdk::vector2d& vecTextPosition, sdk::color aTextColor, int flags, ImFont* pDrawFont);

	void line(const ImVec2& start, const ImVec2& end, const sdk::color& col, float thickness) const;

};

inline renderer_t* renderer = new renderer_t;
