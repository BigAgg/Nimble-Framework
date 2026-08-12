#pragma once

#include "imgui/modern/ImGui_modern_theme.h"

// ---------------------------------------------------------------------------
// The classic color schemes, rebuilt on the modern pipeline
//
// Same identities as the themes in ImGui_themes.cpp - blue, gold, purple, pink,
// Noctua, Rose Pine, Nimble - but expressed as a surface, a text color and an
// accent, with MakeModernPalette() deriving the rest. That is the whole file:
// every theme gets the modern shape, elevation and depth helpers for free, and
// a fix to the mapping fixes all of them at once.
//
// SetTheme() routes to these. The original flat Theme*() functions in
// ImGui_themes.cpp still exist and can be called directly if the old look is
// wanted for a comparison.
// ---------------------------------------------------------------------------

// Blue - the neutral default.
ModernPalette ModernBlueLightPalette();
ModernPalette ModernBlueDarkPalette();
void ThemeModernBlueLight();
void ThemeModernBlueDark();

// Gold. The accent is darkened automatically on the light variant; #F1C40F as
// an outline on white is barely there.
ModernPalette ModernGoldLightPalette();
ModernPalette ModernGoldDarkPalette();
void ThemeModernGoldLight();
void ThemeModernGoldDark();

// Purple.
ModernPalette ModernPurpleLightPalette();
ModernPalette ModernPurpleDarkPalette();
void ThemeModernPurpleLight();
void ThemeModernPurpleDark();

// Girly pink - light only, like the original.
ModernPalette ModernGirlyPinkPalette();
void ThemeModernGirlyPink();

// Noctua - cream and copper / deep brown and sand.
ModernPalette ModernNoctuaLightPalette();
ModernPalette ModernNoctuaDarkPalette();
void ThemeModernNoctuaLight();
void ThemeModernNoctuaDark();

// Rose Pine, using the upstream base/text/iris values.
ModernPalette ModernRosePineLightPalette();
ModernPalette ModernRosePineDarkPalette();
void ThemeModernRosePineLight();
void ThemeModernRosePineDark();

// Nimble corporate identity: warm-tinted surfaces, #F47B20 accent. Same accent
// as the neutral modern theme, on a warmer grey.
ModernPalette ModernNimbleLightPalette();
ModernPalette ModernNimbleDarkPalette();
void ThemeModernNimbleLight();
void ThemeModernNimbleDark();
