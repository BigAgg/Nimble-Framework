#include "imgui/modern/ImGui_modern_theme.h"

#include <imgui.h>

namespace {
/// Same color at a different alpha. Used constantly below, since most of the
/// "soft" look is one hue at several opacities rather than many hues.
ImVec4 Alpha(const ImVec4 &c, float a) {
	return ImVec4(c.x, c.y, c.z, a);
}

/// Composites a translucent color over an opaque one and returns the opaque
/// result. Tables need this: alpha in a table border bleeds where cell borders
/// overlap, so the border colors have to be pre-flattened onto their surface.
ImVec4 Flatten(const ImVec4 &over, const ImVec4 &onto) {
	const float a = over.w;
	return ImVec4(onto.x + (over.x - onto.x) * a,
	              onto.y + (over.y - onto.y) * a,
	              onto.z + (over.z - onto.z) * a,
	              1.0f);
}

/// Towards white for amount > 0, towards black for amount < 0, range -1..1.
ImVec4 Shade(const ImVec4 &c, float amount) {
	const float t = amount < -1.0f ? -1.0f : (amount > 1.0f ? 1.0f : amount);
	const float target = t >= 0.0f ? 1.0f : 0.0f;
	const float k = t >= 0.0f ? t : -t;
	return ImVec4(c.x + (target - c.x) * k,
	              c.y + (target - c.y) * k,
	              c.z + (target - c.z) * k,
	              c.w);
}

/// Linear blend, t = 0 returns a, t = 1 returns b.
ImVec4 Mix(const ImVec4 &a, const ImVec4 &b, float t) {
	return ImVec4(a.x + (b.x - a.x) * t,
	              a.y + (b.y - a.y) * t,
	              a.z + (b.z - a.z) * t,
	              a.w + (b.w - a.w) * t);
}

/// Perceived brightness, used to decide black-or-white text on a fill and to
/// keep bright accents (gold, pink) readable on a light background.
float Luminance(const ImVec4 &c) {
	return 0.2126f * c.x + 0.7152f * c.y + 0.0722f * c.z;
}

// Live theme state. Defaults are overwritten by the first ApplyModernTheme().
ModernPalette g_palette = ModernDarkPalette();
ModernMetrics g_metrics = ModernMetrics();
}  // namespace

const ModernPalette &CurrentModernPalette() {
	return g_palette;
}

const ModernMetrics &CurrentModernMetrics() {
	return g_metrics;
}

void ApplyModernTheme(const ModernPalette &p) {
	ApplyModernTheme(p, ModernMetrics());
}

void ApplyModernTheme(const ModernPalette &p, const ModernMetrics &m) {
	// Reset first: a switch from a classic theme would otherwise leave its
	// paddings and roundings behind, because those are never fully rewritten.
	ImGui::GetStyle() = ImGuiStyle();
	if (p.dark)
		ImGui::StyleColorsDark();
	else
		ImGui::StyleColorsLight();

	ImGuiStyle &style = ImGui::GetStyle();
	ImVec4 *c = style.Colors;

	// -----------------------------------------------------------------------
	// Shape. Roomier than stock ImGui: the flat look is as much about cramped
	// 3px padding as it is about the colors.
	// -----------------------------------------------------------------------
	const float d = m.density;

	style.WindowPadding = ImVec2(14.0f * d, 12.0f * d);
	style.FramePadding = ImVec2(11.0f * d, 6.0f * d);
	style.ItemSpacing = ImVec2(10.0f * d, 8.0f * d);
	style.ItemInnerSpacing = ImVec2(8.0f * d, 6.0f * d);
	style.CellPadding = ImVec2(9.0f * d, 6.0f * d);
	style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
	style.IndentSpacing = 22.0f * d;
	style.ScrollbarSize = 12.0f * d;
	style.GrabMinSize = 12.0f * d;
	style.SeparatorTextPadding = ImVec2(18.0f * d, 4.0f * d);

	style.WindowRounding = m.windowRounding;
	style.ChildRounding = m.rounding + 2.0f;
	style.PopupRounding = m.windowRounding;
	style.FrameRounding = m.rounding;
	style.GrabRounding = m.rounding;
	style.TabRounding = m.rounding + 1.0f;
	style.ScrollbarRounding = 12.0f;

	style.WindowBorderSize = m.borderSize;
	style.ChildBorderSize = m.borderSize;
	style.PopupBorderSize = m.borderSize;
	style.FrameBorderSize = m.borderSize;
	style.TabBorderSize = 0.0f;
	style.TabBarBorderSize = 2.0f;
	style.TabBarOverlineSize = 2.0f;  // accent bar over the selected tab
	style.SeparatorTextBorderSize = 1.0f;
	style.DockingSeparatorSize = 2.0f;
	style.TreeLinesSize = 1.0f;
	style.TreeLinesRounding = 4.0f;

	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_None;  // the arrow is pure noise
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.5f);
	style.DisabledAlpha = 0.45f;

	// Rounded corners are the whole point here, so pay for smooth ones.
	style.AntiAliasedLines = true;
	style.AntiAliasedLinesUseTex = true;
	style.AntiAliasedFill = true;
	style.CircleTessellationMaxError = 0.10f;

	// Tooltips that appear on a twitch feel cheap; give the pointer a moment.
	style.HoverStationaryDelay = 0.20f;
	style.HoverDelayShort = 0.12f;

	// -----------------------------------------------------------------------
	// Colors
	// -----------------------------------------------------------------------
	c[ImGuiCol_Text] = p.text;
	c[ImGuiCol_TextDisabled] = p.textMuted;
	c[ImGuiCol_TextSelectedBg] = Alpha(p.accent, 0.32f);
	c[ImGuiCol_TextLink] = p.accent;
	c[ImGuiCol_InputTextCursor] = p.accent;

	// Surfaces
	c[ImGuiCol_WindowBg] = p.bgBase;
	c[ImGuiCol_ChildBg] = p.bgSurface;
	c[ImGuiCol_PopupBg] = p.bgElevated;
	c[ImGuiCol_MenuBarBg] = p.bgSurface;

	// Borders. Low contrast on purpose: depth should read as a shift in surface
	// brightness, with the outline only hinting at the edge.
	c[ImGuiCol_Border] = p.border;
	c[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

	// Frames
	c[ImGuiCol_FrameBg] = p.field;
	c[ImGuiCol_FrameBgHovered] = p.fieldHovered;
	c[ImGuiCol_FrameBgActive] = p.fieldActive;

	// Title bars. Chrome moves away from the content: brighter on dark themes,
	// darker on light ones. Lifting a light title bar towards white would put it
	// at the same value as the window it sits on.
	c[ImGuiCol_TitleBg] = p.bgSurface;
	c[ImGuiCol_TitleBgActive] = p.dark ? p.bgElevated : p.bgOverlay;
	c[ImGuiCol_TitleBgCollapsed] = Alpha(p.bgSurface, 0.85f);

	// Scrollbars: invisible track, grab only visible when it matters.
	c[ImGuiCol_ScrollbarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	c[ImGuiCol_ScrollbarGrab] = Alpha(p.textMuted, 0.35f);
	c[ImGuiCol_ScrollbarGrabHovered] = Alpha(p.textMuted, 0.55f);
	c[ImGuiCol_ScrollbarGrabActive] = Alpha(p.accent, 0.85f);

	// Controls
	c[ImGuiCol_CheckMark] = p.accent;
	c[ImGuiCol_SliderGrab] = p.accent;
	c[ImGuiCol_SliderGrabActive] = p.accentActive;

	// Buttons stay neutral. A screen where every button screams accent has no
	// hierarchy left; the one primary action uses ImGui::PrimaryButton() from
	// ImGui_modern_decor.h.
	c[ImGuiCol_Button] = p.bgOverlay;
	c[ImGuiCol_ButtonHovered] = p.fieldHovered;
	c[ImGuiCol_ButtonActive] = p.fieldActive;

	// Headers (Selectable, TreeNode, MenuItem, CollapsingHeader)
	c[ImGuiCol_Header] = Alpha(p.accent, p.dark ? 0.22f : 0.18f);
	c[ImGuiCol_HeaderHovered] = Alpha(p.accent, p.dark ? 0.32f : 0.26f);
	c[ImGuiCol_HeaderActive] = Alpha(p.accent, p.dark ? 0.45f : 0.38f);

	// Separators
	c[ImGuiCol_Separator] = p.separator;
	c[ImGuiCol_SeparatorHovered] = Alpha(p.accent, 0.70f);
	c[ImGuiCol_SeparatorActive] = p.accentActive;

	// Tabs. Selection is carried by the overline plus a raised surface, not by
	// filling the whole tab with accent.
	c[ImGuiCol_Tab] = p.bgBase;
	c[ImGuiCol_TabHovered] = p.bgOverlay;
	c[ImGuiCol_TabSelected] = p.bgElevated;
	c[ImGuiCol_TabSelectedOverline] = p.accent;
	c[ImGuiCol_TabDimmed] = p.bgBase;
	c[ImGuiCol_TabDimmedSelected] = p.bgSurface;
	c[ImGuiCol_TabDimmedSelectedOverline] = Alpha(p.accent, 0.40f);

	// Docking
	c[ImGuiCol_DockingPreview] = Alpha(p.accent, 0.35f);
	c[ImGuiCol_DockingEmptyBg] = p.bgBase;

	// Resize grips: only show up once the pointer is near.
	c[ImGuiCol_ResizeGrip] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	c[ImGuiCol_ResizeGripHovered] = Alpha(p.accent, 0.55f);
	c[ImGuiCol_ResizeGripActive] = Alpha(p.accent, 0.90f);

	// Plots
	c[ImGuiCol_PlotLines] = p.accent;
	c[ImGuiCol_PlotLinesHovered] = p.accentHover;
	c[ImGuiCol_PlotHistogram] = p.accent;
	c[ImGuiCol_PlotHistogramHovered] = p.accentHover;

	// Tables. Header sits one layer above the rows, striping stays nearly
	// invisible - just enough to follow a line across a wide table.
	c[ImGuiCol_TableHeaderBg] = p.dark ? p.bgElevated : p.bgOverlay;
	c[ImGuiCol_TableBorderStrong] = Flatten(p.borderStrong, p.bgSurface);
	c[ImGuiCol_TableBorderLight] = Flatten(p.border, p.bgSurface);
	c[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	c[ImGuiCol_TableRowBgAlt] = p.dark ? ImVec4(1.0f, 1.0f, 1.0f, 0.030f)
	                                   : ImVec4(0.0f, 0.0f, 0.0f, 0.024f);

	// Trees, drag/drop, navigation
	c[ImGuiCol_TreeLines] = p.border;
	c[ImGuiCol_DragDropTarget] = p.accentHover;
	c[ImGuiCol_NavCursor] = p.accent;
	c[ImGuiCol_NavWindowingHighlight] = Alpha(p.accent, 0.70f);
	c[ImGuiCol_NavWindowingDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.45f);

	// Modal dim: strong enough that the dialog clearly floats above the app.
	c[ImGuiCol_ModalWindowDimBg] = p.dark ? ImVec4(0.02f, 0.02f, 0.03f, 0.72f)
	                                      : ImVec4(0.10f, 0.11f, 0.13f, 0.42f);

	g_palette = p;
	g_metrics = m;
}

ModernPalette MakeModernPalette(bool dark, const ImVec4 &surface, const ImVec4 &text,
                                const ImVec4 &accent) {
	ModernPalette p;
	p.dark = dark;
	p.text = text;

	if (dark) {
		// Dark: the window is the darkest layer, everything above it gets
		// lighter. Steps of ~4% read as elevation without turning grey.
		p.bgBase = surface;
		p.bgSurface = Shade(surface, 0.045f);
		p.bgElevated = Shade(surface, 0.090f);
		p.bgOverlay = Shade(surface, 0.140f);

		p.field = Shade(surface, 0.060f);
		p.fieldHovered = Shade(surface, 0.100f);
		p.fieldActive = Shade(surface, 0.150f);

		p.border = ImVec4(1.0f, 1.0f, 1.0f, 0.080f);
		p.borderStrong = ImVec4(1.0f, 1.0f, 1.0f, 0.180f);
		p.separator = ImVec4(1.0f, 1.0f, 1.0f, 0.090f);

		p.textMuted = Mix(text, surface, 0.45f);
	} else {
		// Light: the opposite direction, and the part that is easy to get wrong.
		// White windows with white fields and 12% borders melt into one surface,
		// so the window sits a step *below* pure white and the layers that need
		// to stand out (popups, focused fields) go back up to it.
		p.bgBase = Shade(surface, -0.035f);
		p.bgSurface = Shade(surface, -0.075f);
		p.bgElevated = surface;
		p.bgOverlay = Shade(surface, -0.150f);

		p.field = Shade(surface, -0.100f);
		p.fieldHovered = Shade(surface, -0.130f);
		p.fieldActive = surface;

		// Borders carry most of the structure on light themes, so they are far
		// stronger here than the near-invisible hairlines of the dark ones.
		p.border = ImVec4(text.x, text.y, text.z, 0.230f);
		p.borderStrong = ImVec4(text.x, text.y, text.z, 0.420f);
		p.separator = ImVec4(text.x, text.y, text.z, 0.160f);

		// Less washed out than the dark side: muted text on a bright surface
		// loses contrast much faster, and themes whose text is already soft
		// (Rose Pine) drop below readable at 0.42.
		p.textMuted = Mix(text, surface, 0.36f);
	}

	// A bright accent (gold, pink) is unreadable as text or a 1px outline on a
	// light background, so darken it until it carries enough weight.
	ImVec4 a = accent;
	if (!dark) {
		while (Luminance(a) > 0.58f)
			a = Shade(a, -0.08f);
	}
	p.accent = a;
	p.accentHover = Shade(a, dark ? 0.14f : 0.10f);
	p.accentActive = Shade(a, -0.14f);
	// Black text on a light fill, white on a dark one - decided per accent so
	// PrimaryButton stays readable in every theme.
	p.textOnAccent = Luminance(a) > 0.55f ? Shade(a, -0.85f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	if (dark) {
		p.success = ImVec4(0.290f, 0.760f, 0.470f, 1.00f);
		p.warning = ImVec4(0.960f, 0.760f, 0.250f, 1.00f);
		p.danger = ImVec4(0.937f, 0.375f, 0.375f, 1.00f);
		p.info = ImVec4(0.380f, 0.640f, 0.960f, 1.00f);
	} else {
		p.success = ImVec4(0.106f, 0.588f, 0.318f, 1.00f);
		p.warning = ImVec4(0.741f, 0.541f, 0.055f, 1.00f);
		p.danger = ImVec4(0.816f, 0.208f, 0.208f, 1.00f);
		p.info = ImVec4(0.129f, 0.427f, 0.812f, 1.00f);
	}
	return p;
}

/// Neutral charcoal, orange accent. The reference dark theme.
ModernPalette ModernDarkPalette() {
	return MakeModernPalette(true,
	                         ImVec4(0.075f, 0.082f, 0.098f, 1.00f),   // #131519
	                         ImVec4(0.914f, 0.925f, 0.941f, 1.00f),   // #E9ECF0
	                         ImVec4(0.957f, 0.482f, 0.125f, 1.00f));  // #F47B20
}

/// Near-white surfaces with visible structure, same orange accent darkened a
/// step so it holds up against white.
ModernPalette ModernLightPalette() {
	return MakeModernPalette(false,
	                         ImVec4(0.996f, 0.996f, 1.000f, 1.00f),   // #FEFEFF
	                         ImVec4(0.106f, 0.122f, 0.145f, 1.00f),   // #1B1F25
	                         ImVec4(0.957f, 0.482f, 0.125f, 1.00f));  // #F47B20
}

void ThemeModernDark() {
	ApplyModernTheme(ModernDarkPalette());
}

void ThemeModernLight() {
	ApplyModernTheme(ModernLightPalette());
}
