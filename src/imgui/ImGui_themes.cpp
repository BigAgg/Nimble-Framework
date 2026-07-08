#include "imgui/ImGui_themes.h"
#include <imgui.h>

void SetTheme(Themes theme) {
	switch (theme) {
	case Themes::LIGHT:
		ImGui::StyleColorsLight();
		break;
	case Themes::DARK:
		ImGui::StyleColorsDark();
		break;
	case Themes::GOLD_DARK:
		ThemeGoldDark();
		break;
	case Themes::PURPLE_LIGHT:
		ThemePurpleLight();
		break;
	case Themes::PURPLE_DARK:
		ThemePurpleDark();
		break;
    case Themes::GIRLY_PINK:
	    ThemeGirlyPink();
	    break;
	case Themes::NOCTUA_LIGHT:
		ThemeNoctuaLight();
		break;
	case Themes::NOCTUA_DARK:
		ThemeNoctuaDark();
		break;
    case Themes::ROSEPINE_LIGHT:
        ThemeRosePineLight();
        break;
    case Themes::ROSEPINE_DARK:
        ThemeRosePineDark();
        break;
	case Themes::GOLD_LIGHT:
	default:
		ThemeGoldLight();
		break;
	}
}

void SetTheme(unsigned int theme) {
	SetTheme((Themes)theme);
}

void ThemeGirlyPink()
{
    // Start from a light baseline; we’ll override most of it.
    ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors    = style.Colors;

    // --- Palette (soft light + pink) ---
    const ImVec4 bg            = ImVec4(0.98f, 0.96f, 0.98f, 1.00f); // very light pink-lilac
    const ImVec4 bgAlt         = ImVec4(0.96f, 0.93f, 0.96f, 1.00f);
    const ImVec4 panel         = ImVec4(0.93f, 0.88f, 0.93f, 1.00f);

    const ImVec4 text          = ImVec4(0.16f, 0.12f, 0.16f, 1.00f);
    const ImVec4 textDisabled  = ImVec4(0.55f, 0.50f, 0.56f, 1.00f);

    // Pink accent family (base / hover / active)
    const ImVec4 pinkBase      = ImVec4(0.96f, 0.36f, 0.66f, 1.00f); // ~#F55CA8
    const ImVec4 pinkHover     = ImVec4(0.98f, 0.48f, 0.74f, 1.00f);
    const ImVec4 pinkActive    = ImVec4(0.88f, 0.22f, 0.54f, 1.00f);

    // Optional “sparkle” highlight (used sparingly)
    const ImVec4 candy         = ImVec4(1.00f, 0.78f, 0.90f, 1.00f);

    // Borders / separators (warm, soft)
    const ImVec4 border        = ImVec4(0.82f, 0.72f, 0.82f, 0.90f);
    const ImVec4 separator     = ImVec4(0.80f, 0.70f, 0.80f, 0.85f);

    // --- Text ---
    colors[ImGuiCol_Text]                 = text;
    colors[ImGuiCol_TextDisabled]         = textDisabled;

    // --- Backgrounds ---
    colors[ImGuiCol_WindowBg]             = bg;
    colors[ImGuiCol_ChildBg]              = bgAlt;
    colors[ImGuiCol_PopupBg]              = ImVec4(0.99f, 0.97f, 0.99f, 0.98f);

    // --- Borders ---
    colors[ImGuiCol_Border]               = border;
    colors[ImGuiCol_BorderShadow]         = ImVec4(0, 0, 0, 0);

    // --- Frames (inputs, combos, etc.) ---
    colors[ImGuiCol_FrameBg]              = panel;
    colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.97f, 0.90f, 0.96f, 1.00f);
    colors[ImGuiCol_FrameBgActive]        = ImVec4(0.95f, 0.87f, 0.94f, 1.00f);

    // --- Title bars ---
    colors[ImGuiCol_TitleBg]              = bgAlt;
    colors[ImGuiCol_TitleBgActive]        = ImVec4(0.99f, 0.96f, 0.99f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]     = bgAlt;

    // --- Menu / scrollbar ---
    colors[ImGuiCol_MenuBarBg]            = bgAlt;

    colors[ImGuiCol_ScrollbarBg]          = bgAlt;
    colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.82f, 0.74f, 0.82f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.78f, 0.66f, 0.78f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.74f, 0.60f, 0.74f, 1.00f);

    // --- Checkmark / sliders ---
    colors[ImGuiCol_CheckMark]            = pinkActive;
    colors[ImGuiCol_SliderGrab]           = pinkBase;
    colors[ImGuiCol_SliderGrabActive]     = pinkActive;

    // --- Buttons (pink accent) ---
    colors[ImGuiCol_Button]               = ImVec4(pinkBase.x,  pinkBase.y,  pinkBase.z, 0.85f);
    colors[ImGuiCol_ButtonHovered]        = ImVec4(pinkHover.x, pinkHover.y, pinkHover.z, 0.95f);
    colors[ImGuiCol_ButtonActive]         = ImVec4(pinkActive.x,pinkActive.y,pinkActive.z, 1.00f);

    // --- Headers (selectable, tree nodes, collapsing) ---
    colors[ImGuiCol_Header]               = ImVec4(pinkBase.x,  pinkBase.y,  pinkBase.z, 0.35f);
    colors[ImGuiCol_HeaderHovered]        = ImVec4(pinkHover.x, pinkHover.y, pinkHover.z, 0.55f);
    colors[ImGuiCol_HeaderActive]         = ImVec4(pinkActive.x,pinkActive.y,pinkActive.z, 0.65f);

    // --- Separators ---
    colors[ImGuiCol_Separator]            = separator;
    colors[ImGuiCol_SeparatorHovered]     = ImVec4(pinkHover.x, pinkHover.y, pinkHover.z, 0.70f);
    colors[ImGuiCol_SeparatorActive]      = ImVec4(pinkActive.x,pinkActive.y,pinkActive.z, 0.90f);

    // --- Tabs (cute but readable) ---
    colors[ImGuiCol_Tab]                  = ImVec4(0.95f, 0.90f, 0.95f, 1.00f);
    colors[ImGuiCol_TabHovered]           = ImVec4(pinkHover.x, pinkHover.y, pinkHover.z, 0.80f);
    colors[ImGuiCol_TabActive]            = ImVec4(pinkBase.x,  pinkBase.y,  pinkBase.z, 0.90f);
    colors[ImGuiCol_TabUnfocused]         = ImVec4(0.97f, 0.94f, 0.97f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]   = ImVec4(0.96f, 0.92f, 0.96f, 1.00f);

    // --- Resize grips ---
    colors[ImGuiCol_ResizeGrip]           = ImVec4(pinkBase.x,  pinkBase.y,  pinkBase.z, 0.25f);
    colors[ImGuiCol_ResizeGripHovered]    = ImVec4(pinkHover.x, pinkHover.y, pinkHover.z, 0.65f);
    colors[ImGuiCol_ResizeGripActive]     = ImVec4(pinkActive.x,pinkActive.y,pinkActive.z, 0.90f);

    // --- Selection / nav highlight ---
    colors[ImGuiCol_TextSelectedBg]       = ImVec4(candy.x, candy.y, candy.z, 0.60f);
    colors[ImGuiCol_DragDropTarget]       = pinkHover;
    colors[ImGuiCol_NavHighlight]         = pinkBase;

    // --- Plots ---
    colors[ImGuiCol_PlotLines]            = ImVec4(0.55f, 0.25f, 0.55f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]     = pinkHover;
    colors[ImGuiCol_PlotHistogram]        = ImVec4(pinkBase.x, pinkBase.y, pinkBase.z, 0.85f);
    colors[ImGuiCol_PlotHistogramHovered] = pinkHover;

    // --- Modal dim ---
    colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.20f, 0.10f, 0.18f, 0.25f);

    // --- Style shaping (soft + “cute”) ---
    style.WindowRounding    = 10.0f;
    style.FrameRounding     = 8.0f;
    style.PopupRounding     = 10.0f;
    style.ScrollbarRounding = 10.0f;
    style.GrabRounding      = 8.0f;
    style.TabRounding       = 8.0f;

    style.WindowBorderSize  = 1.0f;
    style.FrameBorderSize   = 1.0f;
    style.TabBorderSize     = 0.0f;

    // Optional: slightly more padding for a softer look
    style.WindowPadding     = ImVec2(10.0f, 10.0f);
    style.FramePadding      = ImVec2(8.0f, 5.0f);
    style.ItemSpacing       = ImVec2(8.0f, 7.0f);
}

void ThemeNoctuaLight(){
    ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // --- Base palette (cream / sand) ---
    ImVec4 bg = ImVec4(0.96f, 0.93f, 0.88f, 1.00f); // warm cream
    ImVec4 bgAlt = ImVec4(0.92f, 0.88f, 0.82f, 1.00f);
    ImVec4 panel = ImVec4(0.88f, 0.83f, 0.76f, 1.00f);

    ImVec4 text = ImVec4(0.22f, 0.17f, 0.12f, 1.00f);
    ImVec4 textDisabled = ImVec4(0.55f, 0.48f, 0.40f, 1.00f);

    ImVec4 accentBase = ImVec4(0.60f, 0.45f, 0.30f, 1.00f);
    ImVec4 accentHover = ImVec4(0.70f, 0.52f, 0.34f, 1.00f);
    ImVec4 accentActive = ImVec4(0.50f, 0.38f, 0.25f, 1.00f);

    ImVec4 border = ImVec4(0.75f, 0.68f, 0.58f, 0.90f);

    colors[ImGuiCol_Text] = text;
    colors[ImGuiCol_TextDisabled] = textDisabled;

    colors[ImGuiCol_WindowBg] = bg;
    colors[ImGuiCol_ChildBg] = bgAlt;
    colors[ImGuiCol_PopupBg] = ImVec4(0.98f, 0.95f, 0.90f, 0.98f);

    colors[ImGuiCol_Border] = border;
    colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

    colors[ImGuiCol_FrameBg] = panel;
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.87f, 0.80f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.84f, 0.76f, 1.00f);

    colors[ImGuiCol_TitleBg] = bgAlt;
    colors[ImGuiCol_TitleBgActive] = bg;

    colors[ImGuiCol_CheckMark] = accentActive;
    colors[ImGuiCol_SliderGrab] = accentBase;
    colors[ImGuiCol_SliderGrabActive] = accentActive;

    colors[ImGuiCol_Button] = ImVec4(accentBase.x, accentBase.y, accentBase.z, 0.85f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(accentHover.x, accentHover.y, accentHover.z, 0.95f);
    colors[ImGuiCol_ButtonActive] = accentActive;

    colors[ImGuiCol_Header] = ImVec4(accentBase.x, accentBase.y, accentBase.z, 0.55f);
    colors[ImGuiCol_HeaderHovered] = accentHover;
    colors[ImGuiCol_HeaderActive] = accentActive;

    colors[ImGuiCol_Tab] = bgAlt;
    colors[ImGuiCol_TabHovered] = accentHover;
    colors[ImGuiCol_TabActive] = accentBase;

    colors[ImGuiCol_Separator] = border;
    colors[ImGuiCol_SeparatorHovered] = accentHover;
    colors[ImGuiCol_SeparatorActive] = accentActive;

    colors[ImGuiCol_TextSelectedBg] = ImVec4(accentBase.x, accentBase.y, accentBase.z, 0.35f);
    colors[ImGuiCol_NavHighlight] = accentBase;

    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.15f, 0.10f, 0.25f);

    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;

    style.FrameBorderSize = 1.0f;
    style.WindowBorderSize = 1.0f;
}

void ThemeNoctuaDark(){
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // --- Base palette (dark brown) ---
    ImVec4 bg = ImVec4(0.15f, 0.12f, 0.09f, 1.00f); // deep brown
    ImVec4 bgAlt = ImVec4(0.18f, 0.14f, 0.10f, 1.00f);
    ImVec4 panel = ImVec4(0.22f, 0.17f, 0.12f, 1.00f);

    ImVec4 text = ImVec4(0.92f, 0.88f, 0.80f, 1.00f); // warm beige
    ImVec4 textDisabled = ImVec4(0.60f, 0.55f, 0.48f, 1.00f);

    // Accent family (sand / copper)
    ImVec4 accentBase = ImVec4(0.76f, 0.60f, 0.40f, 1.00f);
    ImVec4 accentHover = ImVec4(0.85f, 0.68f, 0.46f, 1.00f);
    ImVec4 accentActive = ImVec4(0.65f, 0.50f, 0.32f, 1.00f);

    ImVec4 border = ImVec4(0.35f, 0.28f, 0.20f, 0.80f);

    // Text
    colors[ImGuiCol_Text] = text;
    colors[ImGuiCol_TextDisabled] = textDisabled;

    // Backgrounds
    colors[ImGuiCol_WindowBg] = bg;
    colors[ImGuiCol_ChildBg] = bgAlt;
    colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.15f, 0.11f, 0.98f);

    // Borders
    colors[ImGuiCol_Border] = border;
    colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

    // Frames
    colors[ImGuiCol_FrameBg] = panel;
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.20f, 0.14f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.23f, 0.16f, 1.00f);

    // Title bars
    colors[ImGuiCol_TitleBg] = bgAlt;
    colors[ImGuiCol_TitleBgActive] = panel;
    colors[ImGuiCol_TitleBgCollapsed] = bgAlt;

    // Scrollbars
    colors[ImGuiCol_ScrollbarBg] = bgAlt;
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.32f, 0.25f, 0.18f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.32f, 0.22f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.48f, 0.38f, 0.26f, 1.00f);

    // Accent elements
    colors[ImGuiCol_CheckMark] = accentBase;
    colors[ImGuiCol_SliderGrab] = accentBase;
    colors[ImGuiCol_SliderGrabActive] = accentActive;

    colors[ImGuiCol_Button] = ImVec4(accentBase.x, accentBase.y, accentBase.z, 0.85f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(accentHover.x, accentHover.y, accentHover.z, 0.95f);
    colors[ImGuiCol_ButtonActive] = accentActive;

    colors[ImGuiCol_Header] = ImVec4(accentBase.x, accentBase.y, accentBase.z, 0.50f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(accentHover.x, accentHover.y, accentHover.z, 0.80f);
    colors[ImGuiCol_HeaderActive] = accentActive;

    colors[ImGuiCol_Tab] = bgAlt;
    colors[ImGuiCol_TabHovered] = ImVec4(accentHover.x, accentHover.y, accentHover.z, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(accentBase.x, accentBase.y, accentBase.z, 0.90f);

    colors[ImGuiCol_Separator] = border;
    colors[ImGuiCol_SeparatorHovered] = accentHover;
    colors[ImGuiCol_SeparatorActive] = accentActive;

    colors[ImGuiCol_TextSelectedBg] = ImVec4(accentBase.x, accentBase.y, accentBase.z, 0.35f);
    colors[ImGuiCol_NavHighlight] = accentBase;

    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.65f);

    // Soft industrial look
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;

    style.FrameBorderSize = 1.0f;
    style.WindowBorderSize = 1.0f;
}

void ThemeRosePineLight(){
    ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* c = style.Colors;

    ImVec4 base = ImVec4(0.98f, 0.96f, 0.93f, 1.00f); // #faf4ed
    ImVec4 surface = ImVec4(1.00f, 0.98f, 0.95f, 1.00f); // #fffaf3
    ImVec4 overlay = ImVec4(0.95f, 0.91f, 0.88f, 1.00f); // #f2e9e1

    //ImVec4 text = ImVec4(0.34f, 0.32f, 0.47f, 1.00f); // #575279
    ImVec4 text = ImVec4(0.25f, 0.22f, 0.32f, 1.00f);
    ImVec4 muted = ImVec4(0.60f, 0.58f, 0.65f, 1.00f); // #9893a5

    ImVec4 pine = ImVec4(0.16f, 0.41f, 0.51f, 1.00f); // #286983
    ImVec4 rose = ImVec4(0.71f, 0.39f, 0.48f, 1.00f); // #b4637a
    ImVec4 iris = ImVec4(0.56f, 0.48f, 0.66f, 1.00f); // #907aa9

    c[ImGuiCol_Text] = text;
    c[ImGuiCol_TextDisabled] = muted;

    c[ImGuiCol_WindowBg] = base;
    c[ImGuiCol_ChildBg] = surface;
    c[ImGuiCol_PopupBg] = surface;

    c[ImGuiCol_Border] = overlay;
    c[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

    c[ImGuiCol_FrameBg] = overlay;
    c[ImGuiCol_FrameBgHovered] = surface;
    c[ImGuiCol_FrameBgActive] = surface;

    c[ImGuiCol_TitleBg] = surface;
    c[ImGuiCol_TitleBgActive] = overlay;

    c[ImGuiCol_CheckMark] = rose;
    c[ImGuiCol_SliderGrab] = iris;
    c[ImGuiCol_SliderGrabActive] = rose;

    c[ImGuiCol_Button] = ImVec4(pine.x, pine.y, pine.z, 0.85f);
    c[ImGuiCol_ButtonHovered] = pine;
    c[ImGuiCol_ButtonActive] = rose;

    c[ImGuiCol_Header] = ImVec4(iris.x, iris.y, iris.z, 0.45f);
    c[ImGuiCol_HeaderHovered] = iris;
    c[ImGuiCol_HeaderActive] = rose;

    c[ImGuiCol_Tab] = surface;
    c[ImGuiCol_TabHovered] = iris;
    c[ImGuiCol_TabActive] = overlay;

    c[ImGuiCol_Separator] = overlay;
    c[ImGuiCol_SeparatorHovered] = iris;
    c[ImGuiCol_SeparatorActive] = rose;

    c[ImGuiCol_TextSelectedBg] = ImVec4(iris.x, iris.y, iris.z, 0.35f);
    c[ImGuiCol_NavHighlight] = rose;

    c[ImGuiCol_ModalWindowDimBg] = ImVec4(0.30f, 0.25f, 0.20f, 0.20f);

    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;

    style.FrameBorderSize = 1.0f;
    style.WindowBorderSize = 1.0f;
}

void ThemeRosePineDark(){
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* c = style.Colors;

    // --- Base palette ---
    ImVec4 base = ImVec4(0.10f, 0.09f, 0.14f, 1.00f); // #191724
    ImVec4 surface = ImVec4(0.12f, 0.11f, 0.18f, 1.00f); // #1f1d2e
    ImVec4 overlay = ImVec4(0.15f, 0.14f, 0.23f, 1.00f); // #26233a

    ImVec4 text = ImVec4(0.88f, 0.87f, 0.96f, 1.00f); // #e0def4
    ImVec4 muted = ImVec4(0.43f, 0.42f, 0.53f, 1.00f); // #6e6a86

    ImVec4 pine = ImVec4(0.19f, 0.45f, 0.56f, 1.00f); // #31748f
    ImVec4 rose = ImVec4(0.92f, 0.44f, 0.57f, 1.00f); // #eb6f92
    ImVec4 iris = ImVec4(0.77f, 0.65f, 0.91f, 1.00f); // #c4a7e7

    // Text
    c[ImGuiCol_Text] = text;
    c[ImGuiCol_TextDisabled] = muted;

    // Backgrounds
    c[ImGuiCol_WindowBg] = base;
    c[ImGuiCol_ChildBg] = surface;
    c[ImGuiCol_PopupBg] = surface;

    // Borders
    c[ImGuiCol_Border] = overlay;
    c[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

    // Frames
    c[ImGuiCol_FrameBg] = surface;
    c[ImGuiCol_FrameBgHovered] = overlay;
    c[ImGuiCol_FrameBgActive] = overlay;

    // Title
    c[ImGuiCol_TitleBg] = surface;
    c[ImGuiCol_TitleBgActive] = overlay;

    // Scrollbar
    c[ImGuiCol_ScrollbarBg] = surface;
    c[ImGuiCol_ScrollbarGrab] = overlay;
    c[ImGuiCol_ScrollbarGrabHovered] = iris;
    c[ImGuiCol_ScrollbarGrabActive] = rose;

    // Accent usage (subtle)
    c[ImGuiCol_CheckMark] = iris;
    c[ImGuiCol_SliderGrab] = iris;
    c[ImGuiCol_SliderGrabActive] = rose;

    c[ImGuiCol_Button] = ImVec4(pine.x, pine.y, pine.z, 0.80f);
    c[ImGuiCol_ButtonHovered] = pine;
    c[ImGuiCol_ButtonActive] = rose;

    c[ImGuiCol_Header] = ImVec4(iris.x, iris.y, iris.z, 0.40f);
    c[ImGuiCol_HeaderHovered] = ImVec4(iris.x, iris.y, iris.z, 0.70f);
    c[ImGuiCol_HeaderActive] = iris;

    c[ImGuiCol_Tab] = surface;
    c[ImGuiCol_TabHovered] = iris;
    c[ImGuiCol_TabActive] = overlay;

    c[ImGuiCol_Separator] = overlay;
    c[ImGuiCol_SeparatorHovered] = iris;
    c[ImGuiCol_SeparatorActive] = rose;

    c[ImGuiCol_TextSelectedBg] = ImVec4(iris.x, iris.y, iris.z, 0.35f);
    c[ImGuiCol_NavHighlight] = rose;

    c[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.60f);

    // Styling (Rose Pine is soft)
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;

    // --- Stronger frame contrast ---
    ImVec4 frameBase = ImVec4(0.18f, 0.17f, 0.26f, 1.00f); // darker than overlay
    ImVec4 frameHover = ImVec4(0.22f, 0.21f, 0.32f, 1.00f);
    ImVec4 frameActive = ImVec4(0.27f, 0.25f, 0.38f, 1.00f); // clear focus bump

    c[ImGuiCol_FrameBg] = frameBase;
    c[ImGuiCol_FrameBgHovered] = frameHover;
    c[ImGuiCol_FrameBgActive] = frameActive;

    // Slightly stronger border for inputs
    c[ImGuiCol_Border] = ImVec4(0.20f, 0.19f, 0.30f, 1.00f);

    // Optional: colored focus feedback (subtle iris glow)
    c[ImGuiCol_NavHighlight] = ImVec4(0.77f, 0.65f, 0.91f, 1.00f);

    // Make focus feel intentional
    style.FrameBorderSize = 1.0f;
    style.FrameRounding = 5.0f;
}

void ThemeGoldDark() {
	ImGui::StyleColorsDark();
  ImGuiStyle& style = ImGui::GetStyle();
  ImVec4* colors = style.Colors;

  // --- Base palette ---
  ImVec4 bg = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);  // Main background
  ImVec4 bgDark = ImVec4(0.04f, 0.04f, 0.05f, 1.00f);
  ImVec4 bgLight = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);

  ImVec4 text = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
  ImVec4 textDisabled = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);

  // Yellow-gold accent (base / hover / active)
  ImVec4 goldBase = ImVec4(0.95f, 0.76f, 0.06f, 1.00f);  // ~#F1C40F
  ImVec4 goldHover = ImVec4(0.99f, 0.82f, 0.20f, 1.00f);
  ImVec4 goldActive = ImVec4(0.89f, 0.67f, 0.00f, 1.00f);

  // Text
  colors[ImGuiCol_Text] = text;
  colors[ImGuiCol_TextDisabled] = textDisabled;

  // Backgrounds
  colors[ImGuiCol_WindowBg] = bg;
  colors[ImGuiCol_ChildBg] = bgDark;
  colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.09f, 0.98f);

  // Borders
  colors[ImGuiCol_Border] = ImVec4(0.35f, 0.30f, 0.10f, 0.70f); // subtle gold tint
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

  // Frames (input boxes, combo, etc.)
  colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.22f, 0.24f, 1.00f);

  // Title bars
  colors[ImGuiCol_TitleBg] = bgDark;
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = bgDark;

  // Menus & bars
  colors[ImGuiCol_MenuBarBg] = bgDark;
  colors[ImGuiCol_ScrollbarBg] = bgDark;
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.32f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.38f, 0.38f, 0.40f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.46f, 0.46f, 0.48f, 1.00f);

  // Checkboxes, radio buttons
  colors[ImGuiCol_CheckMark] = goldBase;

  // Sliders
  colors[ImGuiCol_SliderGrab] = goldBase;
  colors[ImGuiCol_SliderGrabActive] = goldActive;

  // Buttons
  colors[ImGuiCol_Button] = ImVec4(goldBase.x, goldBase.y, goldBase.z, 0.80f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(goldHover.x, goldHover.y, goldHover.z, 0.90f);
  colors[ImGuiCol_ButtonActive] = ImVec4(goldActive.x, goldActive.y, goldActive.z, 1.00f);

  // Headers (tree nodes, collapsing headers, selectable)
  colors[ImGuiCol_Header] = ImVec4(goldBase.x, goldBase.y, goldBase.z, 0.55f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(goldHover.x, goldHover.y, goldHover.z, 0.80f);
  colors[ImGuiCol_HeaderActive] = ImVec4(goldActive.x, goldActive.y, goldActive.z, 0.90f);

  // Separator
  colors[ImGuiCol_Separator] = ImVec4(0.45f, 0.36f, 0.10f, 0.80f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(goldHover.x, goldHover.y, goldHover.z, 0.78f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(goldActive.x, goldActive.y, goldActive.z, 1.00f);

  // Tabs
  colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
  colors[ImGuiCol_TabHovered] = ImVec4(goldHover.x, goldHover.y, goldHover.z, 0.85f);
  colors[ImGuiCol_TabActive] = ImVec4(goldBase.x, goldBase.y, goldBase.z, 0.95f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);

  // Resize grips
  colors[ImGuiCol_ResizeGrip] = ImVec4(goldBase.x, goldBase.y, goldBase.z, 0.20f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(goldHover.x, goldHover.y, goldHover.z, 0.70f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(goldActive.x, goldActive.y, goldActive.z, 0.95f);

  // Selection
  colors[ImGuiCol_TextSelectedBg] = ImVec4(goldBase.x, goldBase.y, goldBase.z, 0.35f);

  // Misc
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
  colors[ImGuiCol_DragDropTarget] = goldHover;
  colors[ImGuiCol_NavHighlight] = goldBase;

  // --- Styling tweaks ---
  style.WindowRounding = 6.0f;
  style.FrameRounding = 4.0f;
  style.PopupRounding = 4.0f;
  style.ScrollbarRounding = 6.0f;
  style.GrabRounding = 4.0f;
  style.TabRounding = 4.0f;

  style.WindowBorderSize = 1.0f;
  style.FrameBorderSize = 1.0f;
}

void ThemeGoldLight() {
  ImGui::StyleColorsLight();
  ImGuiStyle& style = ImGui::GetStyle();
  ImVec4* colors = style.Colors;

  // --- Base palette (light) ---
  ImVec4 bg = ImVec4(0.96f, 0.97f, 0.98f, 1.00f);  // Window background
  ImVec4 bgAlt = ImVec4(0.92f, 0.93f, 0.95f, 1.00f);  // Child/menu
  ImVec4 bgFrame = ImVec4(0.88f, 0.89f, 0.91f, 1.00f);  // Frames / inputs

  ImVec4 text = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
  ImVec4 textDisabled = ImVec4(0.55f, 0.55f, 0.60f, 1.00f);

  // Same yellow-gold accent family as the dark theme
  ImVec4 goldBase = ImVec4(0.95f, 0.76f, 0.06f, 1.00f);  // ~#F1C40F
  ImVec4 goldHover = ImVec4(0.99f, 0.82f, 0.20f, 1.00f);
  ImVec4 goldActive = ImVec4(0.89f, 0.67f, 0.00f, 1.00f);

  // Text
  colors[ImGuiCol_Text] = text;
  colors[ImGuiCol_TextDisabled] = textDisabled;

  // Backgrounds
  colors[ImGuiCol_WindowBg] = bg;
  colors[ImGuiCol_ChildBg] = bgAlt;
  colors[ImGuiCol_PopupBg] = ImVec4(0.98f, 0.98f, 0.99f, 0.98f);

  // Borders
  colors[ImGuiCol_Border] = ImVec4(0.75f, 0.70f, 0.45f, 0.80f); // subtle warm tint
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

  // Frames (input boxes, combo, etc.)
  colors[ImGuiCol_FrameBg] = bgFrame;
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.94f, 0.94f, 0.95f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.91f, 0.91f, 0.92f, 1.00f);

  // Title bars
  colors[ImGuiCol_TitleBg] = bgAlt;
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.98f, 0.98f, 0.99f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = bgAlt;

  // Menus & bars
  colors[ImGuiCol_MenuBarBg] = bgAlt;
  colors[ImGuiCol_ScrollbarBg] = bgAlt;
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.72f, 0.72f, 0.74f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.64f, 0.64f, 0.66f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);

  // Checkboxes, radio buttons
  colors[ImGuiCol_CheckMark] = goldActive;

  // Sliders
  colors[ImGuiCol_SliderGrab] = goldBase;
  colors[ImGuiCol_SliderGrabActive] = goldActive;

  // Buttons (gold accent pops nicely on light bg)
  colors[ImGuiCol_Button] = ImVec4(goldBase.x, goldBase.y, goldBase.z, 0.85f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(goldHover.x, goldHover.y, goldHover.z, 0.95f);
  colors[ImGuiCol_ButtonActive] = ImVec4(goldActive.x, goldActive.y, goldActive.z, 1.00f);

  // Headers (tree nodes, collapsing headers, selectable)
  colors[ImGuiCol_Header] = ImVec4(goldBase.x, goldBase.y, goldBase.z, 0.55f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(goldHover.x, goldHover.y, goldHover.z, 0.80f);
  colors[ImGuiCol_HeaderActive] = ImVec4(goldActive.x, goldActive.y, goldActive.z, 0.90f);

  // Separator
  colors[ImGuiCol_Separator] = ImVec4(0.80f, 0.76f, 0.50f, 0.80f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(goldHover.x, goldHover.y, goldHover.z, 0.90f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(goldActive.x, goldActive.y, goldActive.z, 1.00f);

  // Tabs
  colors[ImGuiCol_Tab] = ImVec4(0.92f, 0.93f, 0.95f, 1.00f);
  colors[ImGuiCol_TabHovered] = ImVec4(goldHover.x, goldHover.y, goldHover.z, 0.90f);
  colors[ImGuiCol_TabActive] = ImVec4(goldBase.x, goldBase.y, goldBase.z, 0.98f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.94f, 0.95f, 0.96f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.96f, 0.96f, 0.97f, 1.00f);

  // Resize grips
  colors[ImGuiCol_ResizeGrip] = ImVec4(goldBase.x, goldBase.y, goldBase.z, 0.30f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(goldHover.x, goldHover.y, goldHover.z, 0.85f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(goldActive.x, goldActive.y, goldActive.z, 1.00f);

  // Selection & navigation
  colors[ImGuiCol_TextSelectedBg] = ImVec4(goldBase.x, goldBase.y, goldBase.z, 0.35f);
  colors[ImGuiCol_DragDropTarget] = goldHover;
  colors[ImGuiCol_NavHighlight] = goldBase;

  // Modals
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.35f);

  // --- Styling tweaks (same “shape” as dark theme) ---
  style.WindowRounding = 6.0f;
  style.FrameRounding = 4.0f;
  style.PopupRounding = 4.0f;
  style.ScrollbarRounding = 6.0f;
  style.GrabRounding = 4.0f;
  style.TabRounding = 4.0f;

  style.WindowBorderSize = 1.0f;
  style.FrameBorderSize = 1.0f;
}

void ThemePurpleDark() {
  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  ImVec4* colors = style.Colors;

  // --- Base palette (dark) ---
  ImVec4 bg = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
  ImVec4 bgDark = ImVec4(0.05f, 0.05f, 0.07f, 1.00f);
  ImVec4 bgLight = ImVec4(0.14f, 0.14f, 0.18f, 1.00f);

  ImVec4 text = ImVec4(0.95f, 0.95f, 0.98f, 1.00f);
  ImVec4 textDisabled = ImVec4(0.55f, 0.55f, 0.60f, 1.00f);

  // Purple accent family (~#9B59B6, #AF7AC5)
  ImVec4 purpleBase = ImVec4(0.61f, 0.35f, 0.71f, 1.00f);
  ImVec4 purpleHover = ImVec4(0.69f, 0.45f, 0.80f, 1.00f);
  ImVec4 purpleActive = ImVec4(0.52f, 0.23f, 0.64f, 1.00f);

  // Text
  colors[ImGuiCol_Text] = text;
  colors[ImGuiCol_TextDisabled] = textDisabled;

  // Backgrounds
  colors[ImGuiCol_WindowBg] = bg;
  colors[ImGuiCol_ChildBg] = bgDark;
  colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.14f, 0.98f);

  // Borders
  colors[ImGuiCol_Border] = ImVec4(0.35f, 0.25f, 0.45f, 0.70f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

  // Frames
  colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.13f, 0.17f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.18f, 0.23f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.22f, 0.27f, 1.00f);

  // Title bars
  colors[ImGuiCol_TitleBg] = bgDark;
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.10f, 0.18f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = bgDark;

  // Menus & bars
  colors[ImGuiCol_MenuBarBg] = bgDark;
  colors[ImGuiCol_ScrollbarBg] = bgDark;
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.38f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.37f, 0.37f, 0.45f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.45f, 0.45f, 0.55f, 1.00f);

  // Checkboxes, radio buttons
  colors[ImGuiCol_CheckMark] = purpleBase;

  // Sliders
  colors[ImGuiCol_SliderGrab] = purpleBase;
  colors[ImGuiCol_SliderGrabActive] = purpleActive;

  // Buttons
  colors[ImGuiCol_Button] = ImVec4(purpleBase.x, purpleBase.y, purpleBase.z, 0.85f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(purpleHover.x, purpleHover.y, purpleHover.z, 0.95f);
  colors[ImGuiCol_ButtonActive] = ImVec4(purpleActive.x, purpleActive.y, purpleActive.z, 1.00f);

  // Headers
  colors[ImGuiCol_Header] = ImVec4(purpleBase.x, purpleBase.y, purpleBase.z, 0.65f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(purpleHover.x, purpleHover.y, purpleHover.z, 0.90f);
  colors[ImGuiCol_HeaderActive] = ImVec4(purpleActive.x, purpleActive.y, purpleActive.z, 1.00f);

  // Separator
  colors[ImGuiCol_Separator] = ImVec4(0.40f, 0.30f, 0.55f, 0.80f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(purpleHover.x, purpleHover.y, purpleHover.z, 0.85f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(purpleActive.x, purpleActive.y, purpleActive.z, 1.00f);

  // Tabs
  colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.10f, 0.18f, 1.00f);
  colors[ImGuiCol_TabHovered] = ImVec4(purpleHover.x, purpleHover.y, purpleHover.z, 0.85f);
  colors[ImGuiCol_TabActive] = ImVec4(purpleBase.x, purpleBase.y, purpleBase.z, 0.95f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.10f, 0.10f, 0.13f, 1.00f);

  // Resize grips
  colors[ImGuiCol_ResizeGrip] = ImVec4(purpleBase.x, purpleBase.y, purpleBase.z, 0.25f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(purpleHover.x, purpleHover.y, purpleHover.z, 0.70f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(purpleActive.x, purpleActive.y, purpleActive.z, 0.95f);

  // Selection & nav
  colors[ImGuiCol_TextSelectedBg] = ImVec4(purpleBase.x, purpleBase.y, purpleBase.z, 0.35f);
  colors[ImGuiCol_DragDropTarget] = purpleHover;
  colors[ImGuiCol_NavHighlight] = purpleBase;

  // Modals
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);

  // --- Styling ---
  style.WindowRounding = 6.0f;
  style.FrameRounding = 4.0f;
  style.PopupRounding = 4.0f;
  style.ScrollbarRounding = 6.0f;
  style.GrabRounding = 4.0f;
  style.TabRounding = 4.0f;

  style.WindowBorderSize = 1.0f;
  style.FrameBorderSize = 1.0f;
}

void ThemePurpleLight() {
  ImGui::StyleColorsLight();

  ImGuiStyle& style = ImGui::GetStyle();
  ImVec4* colors = style.Colors;

  // --- Base palette (light) ---
  ImVec4 bg = ImVec4(0.97f, 0.97f, 0.99f, 1.00f);
  ImVec4 bgAlt = ImVec4(0.94f, 0.94f, 0.98f, 1.00f);
  ImVec4 bgFrame = ImVec4(0.89f, 0.89f, 0.94f, 1.00f);

  ImVec4 text = ImVec4(0.11f, 0.11f, 0.16f, 1.00f);
  ImVec4 textDisabled = ImVec4(0.55f, 0.55f, 0.62f, 1.00f);

  // Same purple accent family
  ImVec4 purpleBase = ImVec4(0.61f, 0.35f, 0.71f, 1.00f);
  ImVec4 purpleHover = ImVec4(0.69f, 0.45f, 0.80f, 1.00f);
  ImVec4 purpleActive = ImVec4(0.52f, 0.23f, 0.64f, 1.00f);

  // Text
  colors[ImGuiCol_Text] = text;
  colors[ImGuiCol_TextDisabled] = textDisabled;

  // Backgrounds
  colors[ImGuiCol_WindowBg] = bg;
  colors[ImGuiCol_ChildBg] = bgAlt;
  colors[ImGuiCol_PopupBg] = ImVec4(0.99f, 0.99f, 1.00f, 0.98f);

  // Borders
  colors[ImGuiCol_Border] = ImVec4(0.44f, 0.40f, 0.58f, 0.90f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

  // Frames
  colors[ImGuiCol_FrameBg] = bgFrame;
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.93f, 0.93f, 0.98f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.90f, 0.96f, 1.00f);

  // Title bars
  colors[ImGuiCol_TitleBg] = bgAlt;
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.98f, 0.98f, 1.00f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = bgAlt;

  // Menus & bars
  colors[ImGuiCol_MenuBarBg] = bgAlt;
  colors[ImGuiCol_ScrollbarBg] = bgAlt;
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.75f, 0.75f, 0.83f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.67f, 0.67f, 0.78f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.60f, 0.72f, 1.00f);

  // Checkboxes, radio buttons
  colors[ImGuiCol_CheckMark] = purpleActive;

  // Sliders
  colors[ImGuiCol_SliderGrab] = purpleBase;
  colors[ImGuiCol_SliderGrabActive] = purpleActive;

  // Buttons
  colors[ImGuiCol_Button] = ImVec4(purpleBase.x, purpleBase.y, purpleBase.z, 0.90f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(purpleHover.x, purpleHover.y, purpleHover.z, 0.98f);
  colors[ImGuiCol_ButtonActive] = ImVec4(purpleActive.x, purpleActive.y, purpleActive.z, 1.00f);

  // Headers
  colors[ImGuiCol_Header] = ImVec4(purpleBase.x, purpleBase.y, purpleBase.z, 0.60f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(purpleHover.x, purpleHover.y, purpleHover.z, 0.90f);
  colors[ImGuiCol_HeaderActive] = ImVec4(purpleActive.x, purpleActive.y, purpleActive.z, 1.00f);

  // Separator
  colors[ImGuiCol_Separator] = ImVec4(0.78f, 0.75f, 0.90f, 0.90f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(purpleHover.x, purpleHover.y, purpleHover.z, 0.95f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(purpleActive.x, purpleActive.y, purpleActive.z, 1.00f);

  // Tabs
  colors[ImGuiCol_Tab] = ImVec4(0.93f, 0.93f, 0.98f, 1.00f);
  colors[ImGuiCol_TabHovered] = ImVec4(purpleHover.x, purpleHover.y, purpleHover.z, 0.95f);
  colors[ImGuiCol_TabActive] = ImVec4(purpleBase.x, purpleBase.y, purpleBase.z, 0.98f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.95f, 0.95f, 0.99f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.97f, 0.97f, 1.00f, 1.00f);

  // Resize grips
  colors[ImGuiCol_ResizeGrip] = ImVec4(purpleBase.x, purpleBase.y, purpleBase.z, 0.30f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(purpleHover.x, purpleHover.y, purpleHover.z, 0.85f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(purpleActive.x, purpleActive.y, purpleActive.z, 1.00f);

  // Selection & nav
  colors[ImGuiCol_TextSelectedBg] = ImVec4(purpleBase.x, purpleBase.y, purpleBase.z, 0.35f);
  colors[ImGuiCol_DragDropTarget] = purpleHover;
  colors[ImGuiCol_NavHighlight] = purpleBase;

  // Modals
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.10f, 0.10f, 0.15f, 0.35f);

  // --- Styling (same shapes as dark) ---
  style.WindowRounding = 6.0f;
  style.FrameRounding = 4.0f;
  style.PopupRounding = 4.0f;
  style.ScrollbarRounding = 6.0f;
  style.GrabRounding = 4.0f;
  style.TabRounding = 4.0f;

  style.WindowBorderSize = 1.0f;
  style.FrameBorderSize = 1.0f;
}

