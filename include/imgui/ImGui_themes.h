#pragma once

enum Themes : unsigned char {
	LIGHT,
	DARK,
	GOLD_LIGHT,
	GOLD_DARK,
	PURPLE_LIGHT,
	PURPLE_DARK,
	GIRLY_PINK,
	NOCTUA_LIGHT,
	NOCTUA_DARK,
	ROSEPINE_LIGHT,
	ROSEPINE_DARK,
	NIMBLE_LIGHT,
	NIMBLE_DARK,
	// Appended, never inserted: WindowControl persists the theme as a number.
	MODERN_LIGHT,
	MODERN_DARK,
	DEFAULT = GOLD_LIGHT,
};

void ThemeGoldDark();
void ThemeGoldLight();
void ThemePurpleDark();
void ThemePurpleLight();
void ThemeGirlyPink();
void ThemeNoctuaLight();
void ThemeNoctuaDark();
void ThemeRosePineLight();
void ThemeRosePineDark();
// Corporate identity themes: orange accent (#F47B20) on charcoal / near-white.
void ThemeNimbleLight();
void ThemeNimbleDark();
// Modern themes. Defined in imgui/modern/ImGui_modern_theme.h, which builds
// them from a semantic palette instead of writing ImGuiCol slots by hand.
void ThemeModernLight();
void ThemeModernDark();

void SetTheme(Themes theme);
void SetTheme(unsigned int theme);
