#pragma once

#include <imgui.h>

#include "imgui/modern/ImGui_modern_theme.h"

// ---------------------------------------------------------------------------
// Depth layer for the modern themes
//
// A style can only ever set flat fills - ImGui paints every widget with one
// solid color. Everything that actually makes a UI look three dimensional
// (drop shadows, vertical gradients, a lit top edge, hover that fades in
// instead of snapping) has to be drawn by hand into the ImDrawList. That is
// what lives here.
//
// Nothing in this file is required: the modern themes look fine without it.
// Use it where it pays off - the primary action of a dialog, the cards on a
// dashboard, a status pill in a table.
//
// All of it reads CurrentModernPalette(), so widgets follow the active theme
// including the light/dark switch, and honour ModernMetrics::shadowStrength.
// ---------------------------------------------------------------------------

namespace ImGui {

// --- Color utilities -------------------------------------------------------

/// Same color, different alpha.
ImU32 ModernAlpha(const ImVec4 &c, float alpha);
/// Moves a color towards white (amount > 0) or black (amount < 0), 0..1.
ImVec4 ModernShade(const ImVec4 &c, float amount);

// --- Draw primitives -------------------------------------------------------

/// Soft drop shadow behind a rounded rect, drawn as concentric rings with a
/// quadratic alpha falloff. Cheap enough for a full screen of cards, and the
/// only way to get real elevation out of ImGui without a blur pass.
///   size:   how far the shadow spreads, in pixels
///   offset: usually (0, size * 0.4) so the light reads as coming from above
void DrawShadowRect(ImDrawList *drawList, const ImVec2 &min, const ImVec2 &max,
                    float rounding, float size, const ImVec2 &offset = ImVec2(0.0f, 2.0f),
                    float opacity = 1.0f);

/// Rounded rect filled with a vertical gradient. This is the second half of
/// "not flat": a button lit slightly stronger at the top reads as a raised
/// surface, the same button flat reads as a colored rectangle.
void FillRectGradient(ImDrawList *drawList, const ImVec2 &min, const ImVec2 &max,
                      ImU32 top, ImU32 bottom, float rounding,
                      ImDrawFlags flags = ImDrawFlags_None);

/// Thin lit line just inside the top edge of a rect. Sells the raised look on
/// buttons and cards for one draw call.
void DrawInnerHighlight(ImDrawList *drawList, const ImVec2 &min, const ImVec2 &max,
                        float rounding, float strength = 0.18f);

// --- Animation -------------------------------------------------------------

/// Per widget 0..1 value that eases towards `state` at `speed` per second.
/// Hover that fades over ~80ms is most of the difference between a UI that
/// feels modern and one that feels like a debug menu.
float ModernAnim(ImGuiID id, bool state, float speed = 12.0f);

// --- Containers ------------------------------------------------------------

/// Elevated panel: shadow, gradient surface, hairline border, child window for
/// the content. Always pair with EndCard(), including when it returns false.
///   size: (0, 0) fills the available region; y > 0 fixes the height
bool BeginCard(const char *id, const ImVec2 &size = ImVec2(0.0f, 0.0f),
               bool border = true);
void EndCard();

/// Title row for a card or a section of a form: label in accent-tinted caps
/// weight plus a hairline that fades out to the right.
void SectionHeader(const char *label);

// --- Widgets ---------------------------------------------------------------

/// The one call-to-action per screen: accent gradient, drop shadow, and it
/// visibly sinks when held. Same signature as ImGui::Button().
bool PrimaryButton(const char *label, const ImVec2 &size = ImVec2(0.0f, 0.0f));

/// Destructive variant of PrimaryButton (delete, cancel subscription, ...).
bool DangerButton(const char *label, const ImVec2 &size = ImVec2(0.0f, 0.0f));

/// PrimaryButton in an arbitrary color, for the status colors of the palette.
bool GradientButton(const char *label, const ImVec4 &color,
                    const ImVec2 &size = ImVec2(0.0f, 0.0f));

/// Borderless button that only shows a surface on hover. For toolbars and
/// anything that would otherwise be a row of competing grey boxes.
bool GhostButton(const char *label, const ImVec2 &size = ImVec2(0.0f, 0.0f));

/// Animated on/off switch. Reads faster than a checkbox at a glance, which is
/// what matters in a settings list.
bool ToggleSwitch(const char *label, bool *value);

/// Small filled pill, e.g. an order state in a table cell. Draws inline and
/// advances the cursor like any other item.
void Badge(const char *label, const ImVec4 &color);
/// Badge in the palette's status colors.
void BadgeSuccess(const char *label);
void BadgeWarning(const char *label);
void BadgeDanger(const char *label);
void BadgeInfo(const char *label);

/// Rounded progress bar with a gradient fill. fraction is 0..1, overlay may be
/// null for no text.
void ModernProgressBar(float fraction, const ImVec2 &size = ImVec2(-1.0f, 0.0f),
                       const char *overlay = nullptr);

/// Indeterminate spinner for pending requests (api::Pending() and friends).
void ModernSpinner(const char *id, float radius = 8.0f, float thickness = 3.0f);

}  // namespace ImGui
