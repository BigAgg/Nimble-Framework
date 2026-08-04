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

void SetTheme(Themes theme);
void SetTheme(unsigned int theme);
