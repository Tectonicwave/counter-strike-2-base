#define IMGUI_DEFINE_MATH_OPERATORS

#include "menu.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_internal.h"
#include "../Imgui/imgui_freetype.h"
#include "config.h"
#include "draw_manager.h"
#include "../resources/font_namespace.h"

// Standard library and necessary includes
#include <string>   // For std::string
#include <cmath>    // For mathematical functions like std::round
#include <d3d11.h>  // For Direct3D 11 interface
#include <tchar.h>  // For TCHAR and string-handling macros

bool info_bar = true;

DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_DisplayHex;
float tab_size = 0.f;
float arrow_roll = 0.f;
bool tab_opening = true;
int rotation_start_index;
static bool Dcheckbox = false;
static bool Headcheckbox = false;
static bool Healthcheckbox = false;
static bool Namecheckbox = false;
static bool Distancecheckbox = false;
static bool weaponcheckbox = false;
static bool Filterteams = false;
static bool Filterteams_map = false;
static bool Bonecheckbox = false;
static float boxtk = 1.f;
static float hptk = 1.f;
static float hdtk = 1.f;
static float bonetk = 1.f;
static bool Flogs[6] = { false, false, false, false,false };
const char* Flogss[6] = { "Head", "Health Bar", "Player Name", "Distance","Weapon" };



//this is cool 
void ImRotateStart()
{
    rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

ImVec2 ImRotationCenter()
{
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

    const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}

void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
{
    float s = sin(rad), c = cos(rad);
    center = ImRotate(center, s, c) - center;

    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

void Particles()
{
    ImVec2 screen_size = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };

    static ImVec2 partile_pos[100];
    static ImVec2 partile_target_pos[100];
    static float partile_speed[100];
    static float partile_radius[100];


    for (int i = 1; i < 50; i++)
    {
        if (partile_pos[i].x == 0 || partile_pos[i].y == 0)
        {
            partile_pos[i].x = rand() % (int)screen_size.x + 1;
            partile_pos[i].y = 15.f;
            partile_speed[i] = 1 + rand() % 25;
            partile_radius[i] = rand() % 4;

            partile_target_pos[i].x = rand() % (int)screen_size.x;
            partile_target_pos[i].y = screen_size.y * 2;
        }

        partile_pos[i] = ImLerp(partile_pos[i], partile_target_pos[i], ImGui::GetIO().DeltaTime * (partile_speed[i] / 60));

        if (partile_pos[i].y > screen_size.y)
        {
            partile_pos[i].x = 0;
            partile_pos[i].y = 0;
        }

        ImGui::GetWindowDrawList()->AddCircleFilled(partile_pos[i], partile_radius[i], ImColor(71, 226, 67, 255 / 2));
    }

}

#include "../resources/font_namespace.h"
#include "imgui_settings.h"

void menu_t::menu()
{
    ImGuiStyle* style = &ImGui::GetStyle();

    if (!window_opened)
        return;

    ImGuiStyle* s = &ImGui::GetStyle();

    s->WindowPadding = ImVec2(0, 0), s->WindowBorderSize = 0;
    s->ItemSpacing = ImVec2(20, 20);

    s->ScrollbarSize = 8.f;

    ImGui::GetBackgroundDrawList()->AddImage(image::bg, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));

    ImGui::SetNextWindowSize(ImVec2(c::bg::size) + ImVec2(tab_size, 0));

    ImGui::Begin("IMGUI MENU", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
    {
        const ImVec2& pos = ImGui::GetWindowPos();
        const auto& p = ImGui::GetWindowPos();
        const ImVec2 spacing = ImGui::GetStyle().ItemSpacing;

        ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(c::bg::size) + ImVec2(tab_size, 0), ImGui::GetColorU32(c::bg::background), c::bg::rounding);
        ImGui::GetBackgroundDrawList()->AddRect(pos, pos + ImVec2(c::bg::size) + ImVec2(tab_size, 0), ImGui::GetColorU32(c::bg::outline_background), c::bg::rounding);

        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(c::accent_text_color));

        ImGui::PushFont(font::tahoma_bold2); ImGui::RenderTextClipped(pos + ImVec2(60, 0) + spacing, pos + spacing + ImVec2(60, 60) + ImVec2(tab_size + (spacing.x / 2) - 30, 0), "UC-Menu", NULL, NULL, ImVec2(0.5f, 0.5f), NULL); ImGui::PopFont();

        ImGui::RenderTextClipped(pos + ImVec2(60 + spacing.x, c::bg::size.y - 60 * 2), pos + spacing + ImVec2(60, c::bg::size.y) + ImVec2(tab_size, 0), "Lifetime", NULL, NULL, ImVec2(0.0f, 0.43f), NULL);
        ImGui::RenderTextClipped(pos + ImVec2(60 + spacing.x, c::bg::size.y - 60 * 2), pos + spacing + ImVec2(60, c::bg::size.y) + ImVec2(tab_size, 0), "hefan2429", NULL, NULL, ImVec2(0.0f, 0.57f), NULL);

        ImGui::PushFont(font::tahoma_bold2); ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 255, 255, 255)); ImGui::RenderTextClipped(pos + ImVec2(0, 0) + spacing, pos + spacing + ImVec2(60, 40) + ImVec2(tab_size + (spacing.x / 2) + 199, 0), "Hello, UC-Menu", NULL, NULL, ImVec2(1.f, 0.5f), NULL); ImGui::PopFont(); ImGui::PopStyleColor();

        ImGui::GetBackgroundDrawList()->AddImage(image::logo, pos + ImVec2(10, 10), pos + ImVec2(10, 10), ImVec2(100, 100), ImVec2(100, 100), ImColor(255, 255, 255, 255));


        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(90, 93, 100, 255)); ImGui::RenderTextClipped(pos + ImVec2(30, 20) + spacing, pos + spacing + ImVec2(60, 80) + ImVec2(tab_size + (spacing.x / 2) + 108, -20), "Welcome Back!", NULL, NULL, ImVec2(1.f, 0.5f), NULL); ImGui::PopStyleColor();

        static char Search[64] = { "" };
        ImGui::SetCursorPos(ImVec2(385 + tab_size, -20) + (s->ItemSpacing * 2));
        ImGui::BeginChild("", " ", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 60));
        ImGui::PushFont(font::tab_icon);
        ImGui::Text("I"); ImGui::SameLine(); ImGui::Text("H"); ImGui::SameLine(); ImGui::Text("G");
        // ImGui::GetWindowDrawList()->AddText(pos + ImVec2(600, 36), ImColor(90, 93, 100, 255), "I");
        // ImGui::GetWindowDrawList()->AddText(pos + ImVec2(635, 36), ImColor(90, 93, 100, 255), "H");
        // ImGui::GetWindowDrawList()->AddText(pos + ImVec2(670, 36), ImColor(90, 93, 100, 255), "G");
        ImGui::PopFont(); ImGui::SameLine(); ImGui::SetNextItemWidth(180); ImGui::InputTextWithHint("Search function", "Search...", Search, 64, NULL);
        ImGui::EndChild();
        ImGui::PopStyleColor(1);


        const char* nametab_array1[6] = { "E", "D", "A", "B", "C","F" };

        const char* nametab_array[6] = { "Legit", "Visuals", "Skins", "Configs", "Settings","Chat" };
        const char* nametab_hint_array[6] = { "Aimbot,Rcs,Trigger", "Overlay,Chams,World", "All Game Skins", "Save your settings", "Element settings","Contact online users" };


        static int tabs = 0;

        ImGui::SetCursorPos(ImVec2(spacing.x + (60 - 22) / 2, 60 + (spacing.y * 2) + 22));
        ImGui::BeginGroup();
        {
            for (int i = 0; i < sizeof(nametab_array1) / sizeof(nametab_array1[0]); i++)
                if (ImGui::Tab(i == tabs, nametab_array1[i], nametab_array[i], nametab_hint_array[i], ImVec2(35 + tab_size, 35))) tabs = i;
        }
        ImGui::EndGroup();

        ImGui::SetCursorPos(ImVec2(8, 9) + spacing);

        ImRotateStart();
        if (ImGui::CustomButton(1, image::roll, ImVec2(35, 35), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::accent_color))) tab_opening = !tab_opening;
        ImRotateEnd(1.57f * arrow_roll);

        ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, -20 + spacing.y) + spacing, pos + spacing + ImVec2(60, c::bg::size.y - 60 - spacing.y * 3) + ImVec2(tab_size, 0), ImGui::GetColorU32(c::child::background), c::child::rounding);
        ImGui::GetBackgroundDrawList()->AddRect(pos + ImVec2(0, -20 + spacing.y) + spacing, pos + spacing + ImVec2(60, c::bg::size.y - 60 - spacing.y * 3) + ImVec2(tab_size, 0), ImGui::GetColorU32(c::child::outline_background), c::child::rounding);

        ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, c::bg::size.y - 60 - spacing.y * 2) + spacing, pos + spacing + ImVec2(60, c::bg::size.y - spacing.y * 2) + ImVec2(tab_size, 0), ImGui::GetColorU32(c::child::background), c::child::rounding);
        ImGui::GetBackgroundDrawList()->AddRect(pos + ImVec2(0, c::bg::size.y - 60 - spacing.y * 2) + spacing, pos + spacing + ImVec2(60, c::bg::size.y - spacing.y * 2) + ImVec2(tab_size, 0), ImGui::GetColorU32(c::child::outline_background), c::child::rounding);

        ImGui::GetWindowDrawList()->AddImage(image::logo, pos + ImVec2(0, c::bg::size.y - 60 - spacing.y * 2) + spacing + ImVec2(12, 12), pos + spacing + ImVec2(60, c::bg::size.y - spacing.y * 2) - ImVec2(12, 12), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));

        ImGui::GetWindowDrawList()->AddCircleFilled(pos + ImVec2(63, c::bg::size.y - (spacing.y * 2) + 3), 10.f, ImGui::GetColorU32(c::child::background), 100.f);
        ImGui::GetWindowDrawList()->AddCircleFilled(pos + ImVec2(63, c::bg::size.y - (spacing.y * 2) + 3), 6.f, ImColor(0, 255, 0, 255), 100.f);

        Particles();

        static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;

        tab_alpha = ImClamp(tab_alpha + (4.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);
        tab_add = ImClamp(tab_add + (std::round(350.f) * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);

        if (tab_alpha == 0.f && tab_add == 0.f) active_tab = tabs;

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * s->Alpha);
        if (tabs == 1)
        {


            ImGui::SetCursorPos(ImVec2(60 + tab_size, 60) + (s->ItemSpacing * 2));
            ImGui::BeginGroup();
            {
                ImGui::BeginChild("D", "MAIN", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 420));
                {

                    ImVec2 p = ImGui::GetWindowPos();
                    static float weapon_color[4] = { 174 / 255.f, 197 / 255.f, 255 / 255.f, 1.f };
                    ImGui::Checkbox("2D Box", &Dcheckbox);
                    /* ImGui::Checkbox("Player Head", &Headcheckbox);
                     ImGui::Checkbox("Health Bar", &Healthcheckbox);
                     ImGui::Checkbox("Player Name", &Namecheckbox);
                     ImGui::Checkbox("Distance", &Distancecheckbox);
                     ImGui::Checkbox("Weapon", &weaponcheckbox); */
                     //  ImGui::Checkbox("Bone", &Bonecheckbox);

                    ImGui::CheckPicker("Bone", "Bone Color", &Bonecheckbox, weapon_color);


                    ImGui::CheckPicker("Bone##2", "Bone Color##2", &Bonecheckbox, weapon_color);

                    ImGui::MultiCombo("Flogs", Flogs, Flogss, 5);

                    static int fov = 0;
                    ImGui::SliderInt("ESP Distance", &fov, 0, 3000);

                    static float chance_hit = 0.5f;
                    ImGui::SliderFloat("Delay", &chance_hit, 0.5f, 5.f, "%.1f");

                    static char input[64] = { "" };
                    ImGui::InputTextWithHint("Exact Value", "Value..", input, 64, NULL);

                    static int select = 0;
                    const char* items[3]{ "Default", "Static", "Module" };
                    ImGui::Combo("Mode", &select, items, IM_ARRAYSIZE(items), 3);

                    static bool multi_num[4] = { true, true, true, false };
                    const char* multi_items[4] = { "Body", "Neck", "Spin", "Legs" };
                    ImGui::MultiCombo("Body Overlay", multi_num, multi_items, 4);

                    static int key = 0;
                    ImGui::Keybind("Overlay keybinds", &key, true);

                    ImGui::KeybindPicker("             Aimbot", &key, true, "Aimbotbinds Color##2", weapon_color);

                    ImGui::ColorEdit4("test Color", weapon_color, picker_flags);

                    if (ImGui::Button("Click Me", ImVec2(ImGui::GetContentRegionMax().x - s->WindowPadding.x, 25)));

                }
                ImGui::EndChild();
            }
            ImGui::EndGroup();
            ImGui::SameLine();

            ImGui::BeginGroup();
            {

                ImGui::BeginChild(" ", "", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 420));
                {
                    ImVec2 pos = ImGui::GetWindowPos();
                    ImDrawList* draw = ImGui::GetWindowDrawList();
                    if (Flogs[0])
                    {
                        //   ImGui::SetCursorPos(ImVec2(80, 45));
                        ImGui::SetCursorPos(ImVec2(60, 65));
                        ImVec2 pos1 = ImGui::GetCursorScreenPos();
                        draw->AddCircle(ImVec2(pos1.x + 88, pos1.y - 20), hdtk + 1, ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 255)), 12, 0);
                        draw->AddCircleFilled(ImVec2(pos1.x + 88, pos1.y - 20), hdtk, ImGui::ColorConvertFloat4ToU32(ImVec4(255, 255, 255, 255)), 12);

                        draw->AddShadowCircle(ImVec2(pos1.x + 88, pos1.y - 20), hdtk, ImGui::ColorConvertFloat4ToU32(ImVec4(71, 226, 67, 255 / 2)), 50.f, ImVec2(0, 0), 0, 12);

                        //  draw->AddRect(ImVec2(pos1.x + 100, pos1.y - 20), ImVec2(pos1.x + 40, pos1.y + 30), ImColor(255,255,255), 0.0f, 0.0f, hdtk);
                    }
                    if (Dcheckbox)
                    {
                        ImGui::SetCursorPos(ImVec2(50, 15));
                        ImVec2 pos1 = ImGui::GetCursorScreenPos();
                        draw->AddRect(ImVec2(pos1.x, pos1.y), ImVec2(pos1.x + 200, pos1.y + 360), ImColor(255, 255, 255, 255), 0.0f, 0.0f, boxtk);
                        draw->AddShadowRect(ImVec2(pos1.x, pos1.y), ImVec2(pos1.x + 200, pos1.y + 360), ImColor(255, 255, 255, 255), 14.0f, ImVec2(0, 0), 0, 0);
                    }
                    if (Flogs[1])
                    {
                        ImGui::SetCursorPos(ImVec2(42, 15));
                        ImVec2 pos1 = ImGui::GetCursorScreenPos();
                        draw->AddRectFilled(ImVec2(pos1.x, pos1.y), ImVec2(pos1.x + hptk, pos1.y + 360), ImColor(24, 248, 24, 255), 0.0f);
                        draw->AddShadowRect(ImVec2(pos1.x, pos1.y), ImVec2(pos1.x + hptk, pos1.y + 360), ImColor(24, 248, 24, 255), 14.0f, ImVec2(0, 0), 0, 0);
                    }
                    if (Flogs[2])
                    {
                        ImGui::SetCursorPos(ImVec2(112, 0));
                        ImVec2 pos1 = ImGui::GetCursorScreenPos();
                        draw->AddText(ImVec2(pos1.x, pos1.y), ImColor(255, 255, 255, 255), "PLAYER NAME");
                    }
                    if (Flogs[3])
                    {
                        ImGui::SetCursorPos(ImVec2(254, 12));
                        ImVec2 pos1 = ImGui::GetCursorScreenPos();
                        draw->AddText(ImVec2(pos1.x, pos1.y), ImColor(255, 255, 255, 255), "384M");
                    }
                    if (Flogs[4])
                    {
                        ImGui::SetCursorPos(ImVec2(254, 23));
                        ImVec2 pos1 = ImGui::GetCursorScreenPos();
                        draw->AddText(ImVec2(pos1.x, pos1.y), ImColor(255, 255, 255, 255), "M416");
                    }
                    if (Bonecheckbox)
                    {
                        ImGui::SetCursorPos(ImVec2(26, 74));
                        ImVec2 pos1 = ImGui::GetCursorScreenPos();
                        ImVec2 child_size = ImGui::GetWindowSize();
                        ImDrawList* draw_list = ImGui::GetWindowDrawList();
                        //����
                        ImVec2 neck(122 + pos1.x, pos1.y);
                        //���ֱ�
                        ImVec2 epaule_droite(70 + pos1.x, 40 + pos1.y);
                        ImVec2 coude_droite(45 + pos1.x, 100 + pos1.y);
                        //���ֱ�
                        ImVec2 epaule_gauche(170 + pos1.x, 20 + pos1.y);
                        ImVec2 coude_gauche(210 + pos1.x, 100 + pos1.y);
                        //������
                        ImVec2 bassin(121 + pos1.x, 110 + pos1.y);
                        //����
                        ImVec2 anche_droite(80 + pos1.x, 207 + pos1.y);
                        ImVec2 genoux_droite(80 + pos1.x, 258 + pos1.y);
                        //����
                        ImVec2 anche_gauche(175 + pos1.x, 210 + pos1.y);
                        ImVec2 genoux_gauche(175 + pos1.x, 261 + pos1.y);
                        //����ͷ������������
                        draw_list->AddLine(neck, bassin, ImColor(255, 255, 255, 255), bonetk);
                        //�������ֱ�
                        draw_list->AddLine(neck, epaule_droite, ImColor(255, 255, 255, 255), bonetk);
                        draw_list->AddLine(epaule_droite, coude_droite, ImColor(255, 255, 255, 255), bonetk);
                        //�������ֱ�
                        draw_list->AddLine(neck, epaule_gauche, ImColor(255, 255, 255, 255), bonetk);
                        draw_list->AddLine(epaule_gauche, coude_gauche, ImColor(255, 255, 255, 255), bonetk);
                        //��������
                        draw_list->AddLine(bassin, anche_droite, ImColor(255, 255, 255, 255), bonetk);
                        draw_list->AddLine(anche_droite, genoux_droite, ImColor(255, 255, 255, 255), bonetk);
                        //��������
                        draw_list->AddLine(bassin, anche_gauche, ImColor(255, 255, 255, 255), bonetk);
                        draw_list->AddLine(anche_gauche, genoux_gauche, ImColor(255, 255, 255, 255), bonetk);
                    }
                }
                ImGui::EndChild();
                ImGui::BeginChild("C", "Others", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 80));
                {
                    ImGui::SliderFloat("2D box thick", &boxtk, 1.f, 5.f, "%.1f");
                    ImGui::SliderFloat("Head Size", &hdtk, 1.f, 10.f, "%.1f");
                    ImGui::SliderFloat("Health Size", &hptk, 1.f, 5.f, "%.1f");
                    ImGui::SliderFloat("Bone thick", &bonetk, 1.f, 5.f, "%.1f");
                }
                ImGui::EndChild();
                // ImGui::SetCursorPos(ImVec2(260,540));
                ImGui::SetCursorPos(ImVec2(60 + tab_size, 500) + (s->ItemSpacing * 2));
                ImGui::BeginChild("A", "Team", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) / 2, 80));
                {
                    ImGui::Checkbox("Filter teams", &Filterteams);
                    ImGui::Checkbox("Filter teams on map", &Filterteams_map);
                }
                ImGui::EndChild();
            }
            ImGui::EndGroup();
        }
        if (tabs == 5)
        {
            ImGui::SetCursorPos(ImVec2(60 + tab_size, 60) + (s->ItemSpacing * 2));
            ImGui::BeginGroup();
            {
                ImGui::BeginChild("D", "Online Chat -> Online users:48", ImVec2((c::bg::size.x - 60 - s->ItemSpacing.x * 4) + 18, 520));
                {

                }
                ImGui::EndChild();
            }
            ImGui::EndGroup();
        }

        ImGui::PopStyleVar();
    }
    ImGui::End();
}