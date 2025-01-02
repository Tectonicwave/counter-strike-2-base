#define IMGUI_DEFINE_MATH_OPERATORS

#include "menu.h"
#include "../Imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../Imgui/imgui_freetype.h"
#include "../Imgui/imgui_edited.hpp"
#include "config.h"

#include <d3d11.h>
#include <tchar.h>

bool info_bar = true;

DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_DisplayHex;
static float tab_alpha = 0.f; 
static float tab_add; 
static int active_tab = 0;

void menu_t::menu()
{
    ImGuiStyle* style = &ImGui::GetStyle();

    if (!window_opened)
        return;

    static float color[4] = { 112 / 255.f, 109 / 255.f, 214 / 255.f, 1.f };
    c::accent_color = { color[0], color[1], color[2], 1.f };

    static float background[4] = { 21 / 255.f, 21 / 255.f, 21 / 255.f, 1.f };
    c::bg::background = { background[0], background[1], background[2], background[3] };

    static float border[4] = { 23 / 255.f, 24 / 255.f, 25 / 255.f, 1.f };
    c::bg::border = { border[0], border[1], border[2], border[3] };

    static float child[4] = { 23 / 255.f, 24 / 255.f, 25 / 255.f, 1.f };
    c::child::background = { child[0], child[1], child[2], child[3] };

    static float widget[4] = { 28 / 255.f, 28 / 255.f, 35 / 255.f, 1.f };
    c::widget::background = { widget[0], widget[1], widget[2], widget[3] };

    static float selectable[4] = { 37 / 255.f, 37 / 255.f, 47 / 255.f, 1.f };
    c::widget::selectable = { selectable[0], selectable[1], selectable[2], selectable[3] };

    static float popup[4] = { 21 / 255.f, 21 / 255.f, 22 / 255.f, 1.f };
    c::widget::popup = { popup[0], popup[1], popup[2], popup[3] };

    static float text_active[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
    c::text::text_active = { text_active[0], text_active[1], text_active[2], text_active[3] };

    static float text_hovered[4] = { 89 / 255.f, 95 / 255.f, 105 / 255.f, 1.f };
    c::text::text_hov = { text_hovered[0], text_hovered[1], text_hovered[2], text_hovered[3] };

    static float text_default[4] = { 50 / 255.f, 54 / 255.f, 59 / 255.f, 1.f };
    c::text::text = { text_default[0], text_default[1], text_default[2], text_default[3] };

    style->WindowPadding = ImVec2(0, 0);
    style->ItemSpacing = ImVec2(20, 0);
    style->WindowBorderSize = 0;
    style->ScrollbarSize = 9.f;

    // ImGui::GetBackgroundDrawList()->AddImage(image::background_preview, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));

    ImGui::SetNextWindowSize(c::bg::size);

    ImGui::Begin("RAGNAREK", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);
    {
        const ImVec2& pos = ImGui::GetWindowPos();
        const ImVec2& region = ImGui::GetContentRegionMax();
        const ImVec2& spacing = style->ItemSpacing;

        ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(c::bg::size), ImGui::GetColorU32(c::bg::background), c::bg::rounding + 1);
        ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(100, c::bg::size.y), ImGui::GetColorU32(c::bg::border), c::bg::rounding, ImDrawFlags_RoundCornersLeft);

       // ImGui::GetBackgroundDrawList()->AddImage(image::logo, pos + (ImVec2(100, 100) - ImVec2(38, 43)) / 2, pos + (ImVec2(100, 100) + ImVec2(38, 43)) / 2, ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::accent_color));

        ImGui::GetBackgroundDrawList()->AddLine(pos + ImVec2(0, 100), pos + ImVec2(100, 100), ImGui::GetColorU32(c::widget::background), 1.f);

        static int page = 0;

        ImGui::SetCursorPos(ImVec2((100 - 47) / 2, 100 + (47 / 2)));
        ImGui::BeginGroup();
        {
            if (edited::Tab(0 == page, 1, "c", ImVec2(47, 47))) page = 0;

            if (edited::Tab(1 == page, 2, "a", ImVec2(47, 47))) page = 1;

            if (edited::Tab(2 == page, 3, "b", ImVec2(47, 47))) page = 2;

            if (edited::Tab(3 == page, 4, "o", ImVec2(47, 47))) page = 3;

            if (edited::Tab(4 == page, 5, "v", ImVec2(47, 47))) page = 4;

            if (edited::Tab(5 == page, 6, "f", ImVec2(47, 47))) page = 5;

            if (edited::Tab(6 == page, 7, "e", ImVec2(47, 47))) page = 6;
        }
        ImGui::EndGroup();

        ImGui::SetCursorPos(ImVec2(100 + spacing.x, 0));

        tab_alpha = ImClamp(tab_alpha + (4.f * ImGui::GetIO().DeltaTime * (page == active_tab ? 1.f : -1.f)), 0.f, 1.f);
        if (tab_alpha == 0.f && tab_add == 0.f) active_tab = page;

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * style->Alpha);

        ImGui::BeginChild("CONTAINER", ImVec2(c::bg::size) - ImVec2(100 + spacing.x, 0));
        {
            if (active_tab == 0)
            {
                ImGui::BeginGroup();
                {
                    edited::BeginChild("Weapons", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {
                        static int select = 0;
                        const char* items[3]{ "AWP", "AK47", "M4A1" };
                        edited::Combo("Select Weapon", &select, items, IM_ARRAYSIZE(items), 3);

                        static bool enable_cfg = true;
                        edited::Checkbox("Enable Config", &enable_cfg);
                    }
                    edited::EndChild();

                    edited::BeginChild("Additions", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {
                        static int select = 0;
                        const char* items[3]{ "Low", "Normal", "High" };
                        edited::Combo("History", &select, items, IM_ARRAYSIZE(items), 3);

                        static bool delay_shot = true;
                        edited::Checkbox("Delay Shot", &delay_shot);

                        static bool duck_peek = false;
                        edited::Checkbox("Duck Peek Assist", &duck_peek);

                        static bool peek_assist = false;
                        edited::CheckboxClicked("Quick Peek Assist", &peek_assist);

                        static bool speed_fire = true;
                        edited::Checkbox("Speed Up Fire Rate", &speed_fire);

                        static bool Magic_bullet = false;
                        edited::CheckboxClicked("Magic Bullet", &Magic_bullet);
                    }
                    edited::EndChild();

                    edited::BeginChild("Anti Aim", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {
                        static bool Enabled = true;
                        edited::Checkbox("Enabled", &Enabled);

                        static int select0 = 0;
                        const char* items0[2]{ "Disabled", "Enabled" };
                        edited::ComboClicked("Pitch", &select0, items0, IM_ARRAYSIZE(items0), 2);

                        static int select1 = 0;
                        const char* items1[2]{ "Disabled", "Enabled" };
                        edited::ComboClicked("Yaw", &select1, items1, IM_ARRAYSIZE(items1), 2);

                        static bool slow_walk = false;
                        edited::Checkbox("Slow Walk", &slow_walk);

                        static bool freestanding = false;
                        static float color[4] = { 124 / 255.f, 103 / 255.f, 255 / 255.f, 0.5f };
                        edited::CheckboxPicker("Freestanding", &freestanding, color, picker_flags);
                    }
                    edited::EndChild();
                }
                ImGui::EndGroup();

                ImGui::SameLine();

                ImGui::BeginGroup();
                {
                    edited::BeginChild("General", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {
                        static bool enabled = true;
                        edited::CheckboxClicked("Enabled", &enabled);

                        static bool silent = false;
                        edited::CheckboxClicked("Silent Aimbot", &silent);

                        static bool auto_fire = true;
                        edited::Checkbox("Automatic Fire", &auto_fire);

                        static bool penetrate_walls = true;
                        edited::Checkbox("Penetrate Walls", &penetrate_walls);

                        static int field = 90;
                        edited::SliderInt("Field Of View", &field, -180, 180);

                        static float r0 = -100, r1 = 100;
                        edited::RangeSliderFloat("Hit Chance", &r0, &r1, -100, 100, "%.1f, %.1f");

                        static float r2 = 0, r3 = 10;
                        edited::RangeSliderFloat("Damage", &r2, &r3, 0, 10, "%.1f, %.1f");
                    }
                    edited::EndChild();

                    edited::BeginChild("Selection", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {
                        static int select0 = 0;
                        const char* items0[2]{ "Hit Chance", "Default" };
                        edited::Combo("Target", &select0, items0, IM_ARRAYSIZE(items0), 2);

                        static bool multi_num1[5] = { false, true, true, true, false };
                        const char* multi_items1[5] = { "Head", "Chest", "Stomatch", "Body", "Legs" };
                        edited::MultiComboClicked("Hitboxes", multi_num1, multi_items1, 5);

                        static int select1 = 0;
                        const char* items1[2]{ "Select", "Defect" };
                        edited::ComboClicked("Multipint", &select1, items1, IM_ARRAYSIZE(items1), 2);

                        static bool auto_stop = true;
                        edited::CheckboxClicked("Auto Stop", &auto_stop);

                        static bool auto_scope = true;
                        edited::Checkbox("Auto Scope", &auto_scope);
                    }
                    edited::EndChild();

                    edited::BeginChild("Extrended", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {
                        static int select1 = 0;
                        const char* items1[2]{ "Automatic", "Yourself" };
                        edited::Combo("Mode", &select1, items1, IM_ARRAYSIZE(items1), 2);

                        static int key = 0;
                        edited::Keybind("Click on me to bind", &key);

                    }
                    edited::EndChild();
                }
                ImGui::EndGroup();

            }
            else if (active_tab == 2)
            {
                ImGui::BeginGroup();
                {
                    edited::BeginChild("Players", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {
                        static bool enabled = true;
                        edited::Checkbox("Enabled", &enabled);

                        static bool teammates = false;
                        edited::Checkbox("Teammates", &teammates);

                        static bool behind = false;
                        edited::Checkbox("Behind Walls", &behind);

                        static bool tracers = true;
                        edited::CheckboxClicked("Bullet Tracers", &tracers);

                        static bool offscreen = false;
                        edited::CheckboxClicked("Offscreen ESP", &offscreen);

                        static bool sounds = false;
                        static float color_sound[4] = { 124 / 255.f, 103 / 255.f, 255 / 255.f, 0.5f };
                        edited::CheckboxPicker("Sounds", &sounds, color_sound, picker_flags);

                        static bool radar = false;
                        static float color_radar1[4] = { 124 / 255.f, 103 / 255.f, 255 / 255.f, 1.0f };
                        static float color_radar2[4] = { 124 / 255.f, 103 / 255.f, 255 / 255.f, 0.5f };
                        edited::CheckboxDoublePicker("Radar", &radar, color_radar1, color_radar2, picker_flags);

                        static char input[45] = { "" };

                        ImGui::InputTextEx("v", "Enter your text here", input, 45, ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 35), NULL);

                    }
                    edited::EndChild();

                    edited::BeginChild("Models", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {
                        static int enemies = 0;
                        edited::Keybind("Enemies", &enemies);

                        static int teammates = 0;
                        edited::Keybind("Teammates", &teammates);

                        static int players = 0;
                        edited::Keybind("Local Player", &players);

                        static int ragdolls = 0;
                        edited::Keybind("Ragdolls", &ragdolls);

                    }
                    edited::EndChild();

                    edited::BeginChild("World", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {
                        static bool bomb = true;
                        edited::CheckboxClicked("Bomb", &bomb);

                        static bool weapons = false;
                        edited::CheckboxClicked("Weapons", &weapons);

                        static float r0 = -9000, r1 = 9000;
                        edited::RangeSliderFloat("The Radius Of Vision", &r0, &r1, -10000, 10000, "%.1f, %.1f");
                    }
                    edited::EndChild();
                }
                ImGui::EndGroup();

                ImGui::SameLine();

                ImGui::BeginGroup();
                {
                    edited::BeginChild("ESP PREVIEW", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {
                        /*
                        edited::esp_preview(image::preview_model,
                            &esp_preview::nickname, esp_preview::nick_color,
                            &esp_preview::weapon, esp_preview::weapon_color,
                            &esp_preview::hp, esp_preview::hp_color,
                            &esp_preview::zoom, esp_preview::zoom_color,
                            &esp_preview::bomb, esp_preview::bomb_color,
                            &esp_preview::c4, esp_preview::c4_color,
                            &esp_preview::money, esp_preview::money_color,
                            &esp_preview::hit, esp_preview::hit_color,
                            &esp_preview::box, esp_preview::box_color,
                            &esp_preview::HP_line, esp_preview::hp_line_color);*/
                    }
                    edited::EndChild();

                    edited::BeginChild("ESP MANAGE ELEMENTS", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {

                        edited::CheckboxPicker("Show Nickname", &esp_preview::nickname, esp_preview::nick_color, picker_flags);

                        edited::CheckboxPicker("Show Zoomed", &esp_preview::zoom, esp_preview::zoom_color, picker_flags);

                        edited::CheckboxPicker("Show Weapon", &esp_preview::weapon, esp_preview::weapon_color, picker_flags);

                        edited::CheckboxPicker("Show Money", &esp_preview::money, esp_preview::money_color, picker_flags);

                        edited::CheckboxPicker("Show Bomb", &esp_preview::bomb, esp_preview::bomb_color, picker_flags);

                        edited::CheckboxPicker("Show Box", &esp_preview::box, esp_preview::box_color, picker_flags);

                        edited::CheckboxPicker("Show Hit", &esp_preview::hit, esp_preview::hit_color, picker_flags);

                        edited::CheckboxDoublePicker("Show HP", &esp_preview::HP_line, esp_preview::hp_color, esp_preview::hp_line_color, picker_flags);

                        edited::CheckboxPicker("Show C4", &esp_preview::c4, esp_preview::c4_color, picker_flags);

                    }
                    edited::EndChild();
                }
                ImGui::EndGroup();
            }
            else if (active_tab == 6)
            {
                ImGui::BeginGroup();
                {
                    edited::BeginChild("GUI", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {
                        edited::ColorEdit4("Accent Color", color, picker_flags | ImGuiColorEditFlags_NoAlpha);
                    }
                    edited::EndChild();

                    edited::BeginChild("Styles", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {
                        edited::ColorEdit4("Background", background, picker_flags);

                        edited::ColorEdit4("Border", border, picker_flags);

                        edited::ColorEdit4("Child", child, picker_flags);

                    }
                    edited::EndChild();

                    edited::BeginChild("Others", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {
                        edited::ColorEdit4("Color Element's", widget, picker_flags);

                        edited::ColorEdit4("Color Selectable", selectable, picker_flags);

                        edited::ColorEdit4("Color Popup's", popup, picker_flags);
                    }
                    edited::EndChild();
                }
                ImGui::EndGroup();

                ImGui::SameLine();

                ImGui::BeginGroup();
                {
                    edited::BeginChild("Miscellaneous", ImVec2(c::bg::size.x - (100 + spacing.x * 3), 0) / 2);
                    {
                        edited::ColorEdit4("Text Active", text_active, picker_flags);

                        edited::ColorEdit4("Text Hovered", text_hovered, picker_flags);

                        edited::ColorEdit4("Text Default", text_default, picker_flags);
                    }
                    edited::EndChild();

                }
                ImGui::EndGroup();
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + spacing.x);
        }
        ImGui::EndChild();

        ImGui::PopStyleVar();
    }
    ImGui::End();

}