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

void SetTheme(Themes theme);
void SetTheme(unsigned int theme);
