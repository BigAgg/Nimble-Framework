#pragma once

#include <imgui.h>

// ---------------------------------------------------------------------------
// Modern theme layer
//
// Kept apart from ImGui_themes.cpp on purpose: the classic themes each write the
// ~60 ImGuiCol slots by hand, which means a new theme is a 200 line copy/paste
// and a fixed slot is fixed in one theme only. Here a theme is a ModernPalette
// (semantic colors) plus ModernMetrics (shape/spacing); ApplyModernTheme() owns
// the single mapping onto ImGui. Adding "Modern Blue" later is one palette
// function.
//
// The look ImGui usually lacks - depth - comes from three things applied
// together: layered surfaces (base -> surface -> elevated), generous padding,
// and accent used sparingly for state instead of on every button. The actual
// shadows and gradients live in ImGui_modern_decor.h, which reads the palette
// set here.
// ---------------------------------------------------------------------------

/// Semantic colors a modern theme is built from. Fill this, never ImGuiCol
/// directly, so every theme reacts to changes in the mapping.
struct ModernPalette {
	// --- Surfaces, ordered by elevation ---
	ImVec4 bgBase;      // window background, the layer everything sits on
	ImVec4 bgSurface;   // panels, child windows, title bar, menu bar
	ImVec4 bgElevated;  // popups, menus, tooltips, cards
	ImVec4 bgOverlay;   // hovered rows, ghost buttons, subtle fills

	// --- Input fields ---
	ImVec4 field;
	ImVec4 fieldHovered;
	ImVec4 fieldActive;

	// --- Content ---
	ImVec4 text;
	ImVec4 textMuted;
	ImVec4 textOnAccent;  // text drawn on top of an accent fill

	// --- Lines ---
	ImVec4 border;        // default 1px outlines, deliberately low contrast
	ImVec4 borderStrong;  // focused frames, table outer borders
	ImVec4 separator;

	// --- Accent (one hue, three states) ---
	ImVec4 accent;
	ImVec4 accentHover;
	ImVec4 accentActive;

	// --- Status colors, used by the decor badges/buttons ---
	ImVec4 success;
	ImVec4 warning;
	ImVec4 danger;
	ImVec4 info;

	// Dark themes need lighter striping and heavier shadows than light ones.
	bool dark = true;
};

/// Shape and spacing of a modern theme. Separate from the palette so a compact
/// mode for data heavy screens is a metrics change, not a new theme.
struct ModernMetrics {
	float density = 1.00f;         // scales padding/spacing: 0.85 compact, 1.2 touch
	float rounding = 6.0f;         // frames, buttons, grabs
	float windowRounding = 10.0f;  // windows, popups, cards
	float borderSize = 1.0f;
	float shadowStrength = 1.0f;   // 0 disables the decor shadows entirely
};

/// Writes palette + metrics into the current ImGuiStyle. Safe to call at any
/// time (not mid-frame); it resets the style first, so themes cannot leak into
/// each other.
void ApplyModernTheme(const ModernPalette &palette, const ModernMetrics &metrics);
void ApplyModernTheme(const ModernPalette &palette);

/// Derives a complete palette from the three decisions a theme actually makes:
/// its base surface, its text color and its accent. Everything else - the four
/// elevation steps, the field states, borders, muted text, the accent hover and
/// active shades - is computed, which is what keeps a light theme from
/// collapsing into one flat sheet of white.
///   surface: the brightest surface of the theme (light) / the window
///            background (dark). The theme's hue lives here.
/// Override single fields afterwards where a theme needs it.
ModernPalette MakeModernPalette(bool dark, const ImVec4 &surface, const ImVec4 &text,
                                const ImVec4 &accent);

/// Palettes. Both carry the Nimble accent (#F47B20) so the corporate identity
/// survives the theme switch.
ModernPalette ModernDarkPalette();
ModernPalette ModernLightPalette();

/// Themes, as expected by SetTheme().
void ThemeModernDark();
void ThemeModernLight();

/// Palette/metrics of the last ApplyModernTheme() call. The decor helpers read
/// these; they default to the dark palette so they stay usable under the
/// classic themes too.
const ModernPalette &CurrentModernPalette();
const ModernMetrics &CurrentModernMetrics();
