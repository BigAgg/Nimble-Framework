#include "imgui/modern/ImGui_modern_palettes.h"

// Every palette below is three colors and nothing else. If a theme needs a
// tweak, override the single field after the MakeModernPalette() call rather
// than expanding this into per-theme mapping code again.

// --- Blue ------------------------------------------------------------------

ModernPalette ModernBlueLightPalette() {
	return MakeModernPalette(false,
	                         ImVec4(0.992f, 0.996f, 1.000f, 1.00f),   // #FDFEFF
	                         ImVec4(0.063f, 0.082f, 0.110f, 1.00f),   // #10151C
	                         ImVec4(0.149f, 0.388f, 0.922f, 1.00f));  // #2663EB
}

ModernPalette ModernBlueDarkPalette() {
	return MakeModernPalette(true,
	                         ImVec4(0.055f, 0.067f, 0.086f, 1.00f),   // #0E1116
	                         ImVec4(0.910f, 0.922f, 0.941f, 1.00f),   // #E8EBF0
	                         ImVec4(0.231f, 0.510f, 0.965f, 1.00f));  // #3B82F6
}

void ThemeModernBlueLight() {
	ApplyModernTheme(ModernBlueLightPalette());
}

void ThemeModernBlueDark() {
	ApplyModernTheme(ModernBlueDarkPalette());
}

// --- Gold ------------------------------------------------------------------

ModernPalette ModernGoldLightPalette() {
	return MakeModernPalette(false,
	                         ImVec4(1.000f, 0.992f, 0.973f, 1.00f),   // #FFFDF8
	                         ImVec4(0.110f, 0.094f, 0.063f, 1.00f),   // #1C1810
	                         ImVec4(0.945f, 0.769f, 0.059f, 1.00f));  // #F1C40F
}

ModernPalette ModernGoldDarkPalette() {
	return MakeModernPalette(true,
	                         ImVec4(0.059f, 0.059f, 0.051f, 1.00f),   // #0F0F0D
	                         ImVec4(0.949f, 0.941f, 0.910f, 1.00f),   // #F2F0E8
	                         ImVec4(0.945f, 0.769f, 0.059f, 1.00f));  // #F1C40F
}

void ThemeModernGoldLight() {
	ApplyModernTheme(ModernGoldLightPalette());
}

void ThemeModernGoldDark() {
	ApplyModernTheme(ModernGoldDarkPalette());
}

// --- Purple ----------------------------------------------------------------

ModernPalette ModernPurpleLightPalette() {
	return MakeModernPalette(false,
	                         ImVec4(0.992f, 0.988f, 1.000f, 1.00f),   // #FDFCFF
	                         ImVec4(0.090f, 0.075f, 0.125f, 1.00f),   // #171320
	                         ImVec4(0.608f, 0.349f, 0.714f, 1.00f));  // #9B59B6
}

ModernPalette ModernPurpleDarkPalette() {
	return MakeModernPalette(true,
	                         ImVec4(0.063f, 0.055f, 0.086f, 1.00f),   // #100E16
	                         ImVec4(0.925f, 0.918f, 0.961f, 1.00f),   // #ECEAF5
	                         ImVec4(0.663f, 0.482f, 0.839f, 1.00f));  // #A97BD6
}

void ThemeModernPurpleLight() {
	ApplyModernTheme(ModernPurpleLightPalette());
}

void ThemeModernPurpleDark() {
	ApplyModernTheme(ModernPurpleDarkPalette());
}

// --- Girly pink ------------------------------------------------------------

ModernPalette ModernGirlyPinkPalette() {
	ModernPalette p = MakeModernPalette(false,
	                                    ImVec4(1.000f, 0.980f, 0.992f, 1.00f),   // #FFFAFD
	                                    ImVec4(0.141f, 0.090f, 0.133f, 1.00f),   // #241722
	                                    ImVec4(0.961f, 0.361f, 0.659f, 1.00f));  // #F55CA8
	// The one theme that is allowed to be loud: keep the selection tint pink
	// rather than letting the darkened accent take over.
	p.info = ImVec4(0.545f, 0.361f, 0.788f, 1.00f);
	return p;
}

void ThemeModernGirlyPink() {
	ApplyModernTheme(ModernGirlyPinkPalette());
}

// --- Noctua ----------------------------------------------------------------

ModernPalette ModernNoctuaLightPalette() {
	return MakeModernPalette(false,
	                         ImVec4(0.984f, 0.969f, 0.937f, 1.00f),   // #FBF7EF
	                         ImVec4(0.129f, 0.102f, 0.071f, 1.00f),   // #211A12
	                         ImVec4(0.541f, 0.384f, 0.204f, 1.00f));  // #8A6234
}

ModernPalette ModernNoctuaDarkPalette() {
	return MakeModernPalette(true,
	                         ImVec4(0.078f, 0.063f, 0.043f, 1.00f),   // #14100B
	                         ImVec4(0.929f, 0.894f, 0.831f, 1.00f),   // #EDE4D4
	                         ImVec4(0.761f, 0.604f, 0.400f, 1.00f));  // #C29A66
}

void ThemeModernNoctuaLight() {
	ApplyModernTheme(ModernNoctuaLightPalette());
}

void ThemeModernNoctuaDark() {
	ApplyModernTheme(ModernNoctuaDarkPalette());
}

// --- Rose Pine -------------------------------------------------------------

ModernPalette ModernRosePineLightPalette() {
	ModernPalette p = MakeModernPalette(false,
	                                    ImVec4(1.000f, 0.980f, 0.953f, 1.00f),   // #FFFAF3 surface
	                                    ImVec4(0.341f, 0.322f, 0.475f, 1.00f),   // #575279 text
	                                    ImVec4(0.565f, 0.478f, 0.663f, 1.00f));  // #907AA9 iris
	// Rose Pine's text is a muted purple, which makes the derived borders too
	// soft. Take them from the darker pine instead.
	p.border = ImVec4(0.157f, 0.412f, 0.514f, 0.280f);
	p.borderStrong = ImVec4(0.157f, 0.412f, 0.514f, 0.480f);
	p.danger = ImVec4(0.706f, 0.388f, 0.478f, 1.00f);  // rose #B4637A
	return p;
}

ModernPalette ModernRosePineDarkPalette() {
	ModernPalette p = MakeModernPalette(true,
	                                    ImVec4(0.098f, 0.090f, 0.141f, 1.00f),   // #191724 base
	                                    ImVec4(0.878f, 0.871f, 0.957f, 1.00f),   // #E0DEF4 text
	                                    ImVec4(0.769f, 0.655f, 0.906f, 1.00f));  // #C4A7E7 iris
	p.danger = ImVec4(0.922f, 0.435f, 0.573f, 1.00f);  // rose #EB6F92
	p.info = ImVec4(0.192f, 0.455f, 0.561f, 1.00f);    // pine #31748F
	return p;
}

void ThemeModernRosePineLight() {
	ApplyModernTheme(ModernRosePineLightPalette());
}

void ThemeModernRosePineDark() {
	ApplyModernTheme(ModernRosePineDarkPalette());
}

// --- Nimble ----------------------------------------------------------------

ModernPalette ModernNimbleLightPalette() {
	return MakeModernPalette(false,
	                         ImVec4(1.000f, 0.988f, 0.976f, 1.00f),   // #FFFCF9 warm white
	                         ImVec4(0.141f, 0.153f, 0.173f, 1.00f),   // #24272C charcoal
	                         ImVec4(0.957f, 0.482f, 0.125f, 1.00f));  // #F47B20
}

ModernPalette ModernNimbleDarkPalette() {
	return MakeModernPalette(true,
	                         ImVec4(0.090f, 0.082f, 0.075f, 1.00f),   // #171513 warm charcoal
	                         ImVec4(0.929f, 0.918f, 0.902f, 1.00f),   // #EDEAE6
	                         ImVec4(0.957f, 0.482f, 0.125f, 1.00f));  // #F47B20
}

void ThemeModernNimbleLight() {
	ApplyModernTheme(ModernNimbleLightPalette());
}

void ThemeModernNimbleDark() {
	ApplyModernTheme(ModernNimbleDarkPalette());
}
