// Must precede every imgui include: the ImVec2/ImVec4 operators are declared
// inside imgui.h behind this macro, and imgui_internal.h relies on them.
#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui/modern/ImGui_modern_decor.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <cmath>

namespace ImGui {
namespace {
/// Rounding used by the elevated pieces (cards, buttons). Pulled from the live
/// metrics so a density/rounding change reaches the hand drawn parts too.
float DecorRounding() {
	return CurrentModernMetrics().rounding;
}

/// Shadow color of the active theme. Dark themes need a near black shadow at
/// high alpha, light themes a soft blue grey - a black shadow on white looks
/// like dirt.
ImVec4 ShadowColor() {
	const ModernPalette &p = CurrentModernPalette();
	return p.dark ? ImVec4(0.0f, 0.0f, 0.0f, 0.55f)
	              : ImVec4(0.36f, 0.42f, 0.51f, 0.30f);
}

/// Draws a gradient-filled, bordered, shadowed rect and returns nothing: the
/// shared body of PrimaryButton, GradientButton and friends.
void DrawRaisedRect(ImDrawList *dl, const ImRect &bb, const ImVec4 &color,
                    float hoverT, bool held) {
	const float rounding = DecorRounding();

	// Held = pressed into the surface, so the shadow goes away and the gradient
	// flips. Removing the shadow is what makes the press readable.
	if (!held) {
		DrawShadowRect(dl, bb.Min, bb.Max, rounding, 6.0f + 2.0f * hoverT,
		               ImVec2(0.0f, 2.0f + hoverT), 0.55f + 0.25f * hoverT);
	}

	const ImVec4 lit = ModernShade(color, 0.12f + 0.10f * hoverT);
	const ImVec4 dim = ModernShade(color, -0.12f);

	if (held) {
		FillRectGradient(dl, bb.Min, bb.Max, ColorConvertFloat4ToU32(dim),
		                 ColorConvertFloat4ToU32(color), rounding);
	} else {
		FillRectGradient(dl, bb.Min, bb.Max, ColorConvertFloat4ToU32(lit),
		                 ColorConvertFloat4ToU32(dim), rounding);
		DrawInnerHighlight(dl, bb.Min, bb.Max, rounding, 0.22f);
	}

	dl->AddRect(bb.Min, bb.Max, ModernAlpha(ModernShade(color, -0.30f), 0.90f),
	            rounding, ImDrawFlags_None, 1.0f);
}
}  // namespace

// ---------------------------------------------------------------------------
// Color utilities
// ---------------------------------------------------------------------------

ImU32 ModernAlpha(const ImVec4 &c, float alpha) {
	return ColorConvertFloat4ToU32(ImVec4(c.x, c.y, c.z, alpha));
}

ImVec4 ModernShade(const ImVec4 &c, float amount) {
	const float t = ImClamp(amount, -1.0f, 1.0f);
	const float target = t >= 0.0f ? 1.0f : 0.0f;
	const float k = t >= 0.0f ? t : -t;
	return ImVec4(c.x + (target - c.x) * k,
	              c.y + (target - c.y) * k,
	              c.z + (target - c.z) * k,
	              c.w);
}

// ---------------------------------------------------------------------------
// Draw primitives
// ---------------------------------------------------------------------------

void DrawShadowRect(ImDrawList *dl, const ImVec2 &min, const ImVec2 &max,
                    float rounding, float size, const ImVec2 &offset, float opacity) {
	const float strength = CurrentModernMetrics().shadowStrength * opacity;
	if (strength <= 0.0f || size <= 0.0f)
		return;

	const ImVec4 shadow = ShadowColor();
	// Concentric rings with a quadratic falloff. Six is the point where more
	// rings stop being visible but keep costing vertices.
	const int steps = 6;
	for (int i = steps; i >= 1; --i) {
		const float t = static_cast<float>(i) / static_cast<float>(steps);
		const float grow = size * t;
		const float a = shadow.w * strength * (1.0f - t) * (1.0f - t);
		if (a <= 0.002f)
			continue;
		dl->AddRectFilled(ImVec2(min.x - grow + offset.x, min.y - grow + offset.y),
		                  ImVec2(max.x + grow + offset.x, max.y + grow + offset.y),
		                  ModernAlpha(shadow, a), rounding + grow);
	}
}

void FillRectGradient(ImDrawList *dl, const ImVec2 &min, const ImVec2 &max,
                      ImU32 top, ImU32 bottom, float rounding, ImDrawFlags flags) {
	// Rounded rects cannot be drawn with per-corner colors, so fill normally and
	// re-shade the vertices that were just written.
	const int first = dl->VtxBuffer.Size;
	dl->AddRectFilled(min, max, top, rounding, flags);
	const int last = dl->VtxBuffer.Size;
	if (top != bottom)
		ShadeVertsLinearColorGradientKeepAlpha(dl, first, last, min,
		                                       ImVec2(min.x, max.y), top, bottom);
}

void DrawInnerHighlight(ImDrawList *dl, const ImVec2 &min, const ImVec2 &max,
                        float rounding, float strength) {
	const float inset = ImMax(rounding * 0.5f, 2.0f);
	dl->AddLine(ImVec2(min.x + inset, min.y + 1.0f),
	            ImVec2(max.x - inset, min.y + 1.0f),
	            IM_COL32(255, 255, 255, static_cast<int>(strength * 255.0f)), 1.0f);
}

// ---------------------------------------------------------------------------
// Animation
// ---------------------------------------------------------------------------

float ModernAnim(ImGuiID id, bool state, float speed) {
	ImGuiStorage *storage = GetStateStorage();
	float t = storage->GetFloat(id, state ? 1.0f : 0.0f);
	const float step = GetIO().DeltaTime * speed;
	t = ImClamp(t + (state ? step : -step), 0.0f, 1.0f);
	storage->SetFloat(id, t);
	return t;
}

// ---------------------------------------------------------------------------
// Containers
// ---------------------------------------------------------------------------

bool BeginCard(const char *id, const ImVec2 &size, bool border) {
	const ModernPalette &p = CurrentModernPalette();
	const ImGuiStyle &style = GetStyle();
	const float rounding = style.ChildRounding;

	const ImVec2 pos = GetCursorScreenPos();
	const ImVec2 avail = GetContentRegionAvail();
	const ImVec2 sz(size.x > 0.0f ? size.x : avail.x,
	                size.y > 0.0f ? size.y : avail.y);
	const ImVec2 end(pos.x + sz.x, pos.y + sz.y);

	// Painted into the parent window: child windows render after their parent,
	// so the shadow lands underneath the card content.
	ImDrawList *dl = GetWindowDrawList();
	DrawShadowRect(dl, pos, end, rounding, 10.0f, ImVec2(0.0f, 4.0f), 0.8f);

	// Very shallow gradient - enough to give the surface a direction without
	// looking like a 2005 gradient button.
	const ImVec4 top = ModernShade(p.bgElevated, p.dark ? 0.035f : 0.0f);
	const ImVec4 bottom = ModernShade(p.bgElevated, p.dark ? -0.02f : -0.015f);
	FillRectGradient(dl, pos, end, ColorConvertFloat4ToU32(top),
	                 ColorConvertFloat4ToU32(bottom), rounding);
	if (border)
		dl->AddRect(pos, end, ColorConvertFloat4ToU32(p.border), rounding,
		            ImDrawFlags_None, style.ChildBorderSize);

	PushStyleColor(ImGuiCol_ChildBg, IM_COL32_BLACK_TRANS);
	PushStyleVar(ImGuiStyleVar_ChildRounding, rounding);
	return BeginChild(id, sz, ImGuiChildFlags_AlwaysUseWindowPadding);
}

void EndCard() {
	EndChild();
	PopStyleVar();
	PopStyleColor();
}

void SectionHeader(const char *label) {
	const ModernPalette &p = CurrentModernPalette();
	const ImGuiStyle &style = GetStyle();

	PushStyleColor(ImGuiCol_Text, p.textMuted);
	TextUnformatted(label);
	PopStyleColor();

	// Hairline that fades out to the right: closes the header without the hard
	// full width rule of Separator().
	const ImVec2 min = GetItemRectMin();
	const ImVec2 max = GetItemRectMax();
	const float y = max.y + style.ItemSpacing.y * 0.5f;
	const float right = min.x + GetContentRegionAvail().x;
	ImDrawList *dl = GetWindowDrawList();
	dl->AddRectFilledMultiColor(ImVec2(min.x, y), ImVec2(right, y + 1.0f),
	                            ModernAlpha(p.accent, 0.55f), IM_COL32_BLACK_TRANS,
	                            IM_COL32_BLACK_TRANS, ModernAlpha(p.accent, 0.55f));
	Dummy(ImVec2(0.0f, style.ItemSpacing.y * 0.5f));
}

// ---------------------------------------------------------------------------
// Buttons
// ---------------------------------------------------------------------------

bool GradientButton(const char *label, const ImVec4 &color, const ImVec2 &sizeArg) {
	ImGuiWindow *window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImGuiStyle &style = GetStyle();
	const ImGuiID id = window->GetID(label);
	const ImVec2 labelSize = CalcTextSize(label, nullptr, true);

	const ImVec2 pos = window->DC.CursorPos;
	const ImVec2 size = CalcItemSize(sizeArg, labelSize.x + style.FramePadding.x * 2.0f,
	                                 labelSize.y + style.FramePadding.y * 2.0f);
	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));

	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	bool hovered = false;
	bool held = false;
	const bool pressed = ButtonBehavior(bb, id, &hovered, &held);
	const float t = ModernAnim(id, hovered || held);

	// Sinking by a pixel while held is the cheapest press feedback there is.
	ImRect drawBb = bb;
	if (held) {
		drawBb.Min.y += 1.0f;
		drawBb.Max.y += 1.0f;
	}

	RenderNavCursor(bb, id);
	DrawRaisedRect(window->DrawList, drawBb, color, t, held);

	PushStyleColor(ImGuiCol_Text, CurrentModernPalette().textOnAccent);
	RenderTextClipped(ImVec2(drawBb.Min.x + style.FramePadding.x,
	                         drawBb.Min.y + style.FramePadding.y),
	                  ImVec2(drawBb.Max.x - style.FramePadding.x,
	                         drawBb.Max.y - style.FramePadding.y),
	                  label, nullptr, &labelSize, style.ButtonTextAlign, &bb);
	PopStyleColor();
	return pressed;
}

bool PrimaryButton(const char *label, const ImVec2 &size) {
	return GradientButton(label, CurrentModernPalette().accent, size);
}

bool DangerButton(const char *label, const ImVec2 &size) {
	return GradientButton(label, CurrentModernPalette().danger, size);
}

bool GhostButton(const char *label, const ImVec2 &size) {
	const ModernPalette &p = CurrentModernPalette();
	PushStyleColor(ImGuiCol_Button, IM_COL32_BLACK_TRANS);
	PushStyleColor(ImGuiCol_ButtonHovered, ColorConvertFloat4ToU32(p.bgOverlay));
	PushStyleColor(ImGuiCol_ButtonActive, ColorConvertFloat4ToU32(p.fieldActive));
	PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
	const bool pressed = Button(label, size);
	PopStyleVar();
	PopStyleColor(3);
	return pressed;
}

// ---------------------------------------------------------------------------
// Widgets
// ---------------------------------------------------------------------------

bool ToggleSwitch(const char *label, bool *value) {
	ImGuiWindow *window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImGuiStyle &style = GetStyle();
	const ModernPalette &p = CurrentModernPalette();
	const ImGuiID id = window->GetID(label);
	const ImVec2 labelSize = CalcTextSize(label, nullptr, true);

	const float height = GetFrameHeight();
	const float width = height * 1.85f;
	const ImVec2 pos = window->DC.CursorPos;
	const ImRect track(pos, ImVec2(pos.x + width, pos.y + height));
	const ImRect bb(pos, ImVec2(track.Max.x + (labelSize.x > 0.0f
	                                               ? style.ItemInnerSpacing.x + labelSize.x
	                                               : 0.0f),
	                            track.Max.y));

	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	bool hovered = false;
	bool held = false;
	const bool pressed = ButtonBehavior(bb, id, &hovered, &held);
	if (pressed) {
		*value = !*value;
		MarkItemEdited(id);
	}

	// One animation per state so the knob glides instead of teleporting.
	const float t = ModernAnim(id, *value, 14.0f);
	const float radius = height * 0.5f;

	ImDrawList *dl = window->DrawList;
	const ImVec4 off = hovered ? p.fieldHovered : p.field;
	const ImVec4 on = hovered ? p.accentHover : p.accent;
	const ImVec4 trackColor(off.x + (on.x - off.x) * t,
	                        off.y + (on.y - off.y) * t,
	                        off.z + (on.z - off.z) * t, 1.0f);

	dl->AddRectFilled(track.Min, track.Max, ColorConvertFloat4ToU32(trackColor), radius);
	dl->AddRect(track.Min, track.Max, ColorConvertFloat4ToU32(p.border), radius);

	const float knobX = ImLerp(track.Min.x + radius, track.Max.x - radius, t);
	const ImVec2 knob(knobX, track.Min.y + radius);
	dl->AddCircleFilled(ImVec2(knob.x, knob.y + 1.0f), radius - 2.5f,
	                    ModernAlpha(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), 0.25f));
	dl->AddCircleFilled(knob, radius - 3.0f, IM_COL32_WHITE);

	if (labelSize.x > 0.0f)
		RenderText(ImVec2(track.Max.x + style.ItemInnerSpacing.x,
		                  bb.Min.y + style.FramePadding.y),
		           label);
	return pressed;
}

void Badge(const char *label, const ImVec4 &color) {
	ImGuiWindow *window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	const ImGuiStyle &style = GetStyle();
	const ImVec2 labelSize = CalcTextSize(label, nullptr, true);
	const ImVec2 padding(style.FramePadding.x * 0.7f, style.FramePadding.y * 0.45f);

	const ImVec2 pos = window->DC.CursorPos;
	const ImVec2 size(labelSize.x + padding.x * 2.0f, labelSize.y + padding.y * 2.0f);
	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));

	ItemSize(size, padding.y);
	if (!ItemAdd(bb, 0))
		return;

	// Tinted fill plus colored text: reads as a state, not as a button.
	const float rounding = size.y * 0.5f;
	window->DrawList->AddRectFilled(bb.Min, bb.Max, ModernAlpha(color, 0.16f), rounding);
	window->DrawList->AddRect(bb.Min, bb.Max, ModernAlpha(color, 0.45f), rounding);

	PushStyleColor(ImGuiCol_Text, color);
	RenderText(ImVec2(bb.Min.x + padding.x, bb.Min.y + padding.y), label);
	PopStyleColor();
}

void BadgeSuccess(const char *label) {
	Badge(label, CurrentModernPalette().success);
}

void BadgeWarning(const char *label) {
	Badge(label, CurrentModernPalette().warning);
}

void BadgeDanger(const char *label) {
	Badge(label, CurrentModernPalette().danger);
}

void BadgeInfo(const char *label) {
	Badge(label, CurrentModernPalette().info);
}

void ModernProgressBar(float fraction, const ImVec2 &sizeArg, const char *overlay) {
	ImGuiWindow *window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	const ImGuiStyle &style = GetStyle();
	const ModernPalette &p = CurrentModernPalette();

	const ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = CalcItemSize(sizeArg, CalcItemWidth(), GetFrameHeight() * 0.55f);
	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));

	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, 0))
		return;

	const float rounding = size.y * 0.5f;
	ImDrawList *dl = window->DrawList;
	dl->AddRectFilled(bb.Min, bb.Max, ColorConvertFloat4ToU32(p.field), rounding);

	fraction = ImSaturate(fraction);
	const float fillX = ImLerp(bb.Min.x, bb.Max.x, fraction);
	if (fillX > bb.Min.x + 1.0f) {
		FillRectGradient(dl, bb.Min, ImVec2(fillX, bb.Max.y),
		                 ColorConvertFloat4ToU32(ModernShade(p.accent, 0.15f)),
		                 ColorConvertFloat4ToU32(ModernShade(p.accent, -0.10f)), rounding);
	}
	dl->AddRect(bb.Min, bb.Max, ColorConvertFloat4ToU32(p.border), rounding);

	if (overlay) {
		const ImVec2 overlaySize = CalcTextSize(overlay, nullptr, true);
		RenderTextClipped(bb.Min, bb.Max, overlay, nullptr, &overlaySize,
		                  ImVec2(0.5f, 0.5f), &bb);
	}
}

void ModernSpinner(const char *id, float radius, float thickness) {
	ImGuiWindow *window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	const ImGuiStyle &style = GetStyle();
	const ImVec2 pos = window->DC.CursorPos;
	const ImVec2 size(radius * 2.0f, radius * 2.0f + style.FramePadding.y);
	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));

	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, window->GetID(id)))
		return;

	// Arc length swings between a short and a long tail, which is what makes an
	// indeterminate spinner look alive rather than mechanical.
	const float time = static_cast<float>(GetTime());
	const int segments = 30;
	const float start = time * 3.6f;
	const float span = IM_PI * (1.0f + 0.45f * std::sin(time * 2.2f));

	const ImVec2 center(pos.x + radius, pos.y + radius + style.FramePadding.y * 0.5f);
	ImDrawList *dl = window->DrawList;
	dl->PathClear();
	for (int i = 0; i <= segments; ++i) {
		const float a = start + (static_cast<float>(i) / segments) * span;
		dl->PathLineTo(ImVec2(center.x + std::cos(a) * radius,
		                      center.y + std::sin(a) * radius));
	}
	dl->PathStroke(ColorConvertFloat4ToU32(CurrentModernPalette().accent),
	               ImDrawFlags_None, thickness);
}

}  // namespace ImGui
