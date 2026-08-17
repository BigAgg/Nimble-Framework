#include "imgui/ImGui_helpers.h"
#include <chrono>
#include <imgui_internal.h>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <string>
#include <imgui.h>
#include <iomanip>
#include <sstream>

#define GET_DAY(timePoint) int(timePoint.tm_mday)
#define GET_MONTH_UNSCALED(timePoint) timePoint.tm_mon
#define GET_MONTH(timePoint) int(GET_MONTH_UNSCALED(timePoint) + 1)
#define GET_YEAR(timePoint) int(timePoint.tm_year + 1900)

#define SET_DAY(timePoint, day) timePoint.tm_mday = day
#define SET_MONTH(timePoint, month) timePoint.tm_mon = month - 1
#define SET_YEAR(timePoint, year) timePoint.tm_year = year - 1900

namespace ImGui {
  static const std::vector<std::string> MONTHS =
  {
      "Januar",
      "Februar",
      "März",
      "April",
      "Mai",
      "Juni",
      "Juli",
      "August",
      "September",
      "Oktober",
      "November",
      "Dezember" };

  static const std::vector<std::string> DAYS =
  {
      "Mo",
      "Di",
      "Mi",
      "Do",
      "Fr",
      "Sa",
      "So" };

  // Implements Zeller's Congruence to determine the day of week [1, 7](Mon-Sun) from the given parameters
  inline static int DayOfWeek(int dayOfMonth, int month, int year) noexcept {
    if ((month == 1) || (month == 2)) {
      month += 12;
      year -= 1;
    }

    int h = (dayOfMonth + static_cast<int>(std::floor((13 * (month + 1)) / 5.0)) + year + static_cast<int>(std::floor(year / 4.0)) - static_cast<int>(std::floor(year / 100.0)) + static_cast<int>(std::floor(year / 400.0))) % 7;

    return static_cast<int>(std::floor(((h + 5) % 7) + 1));
  }

  constexpr static bool IsLeapYear(int year) noexcept {
    if ((year % 400) == 0)
      return true;

    if ((year % 4 == 0) && ((year % 100) != 0))
      return true;

    return false;
  }

  inline static int NumDaysInMonth(int month, int year) {
    if (month == 2)
      return IsLeapYear(year) ? 29 : 28;

    // Month index paired to the number of days in that month excluding February
    static const std::unordered_map<int, int> monthDayMap =
    {
        {1, 31},
        {3, 31},
        {4, 30},
        {5, 31},
        {6, 30},
        {7, 31},
        {8, 31},
        {9, 30},
        {10, 31},
        {11, 30},
        {12, 31} };

    return monthDayMap.at(month);
  }

  // Returns the number of calendar weeks spanned by month in the specified year
  inline static int NumWeeksInMonth(int month, int year) {
    int days = NumDaysInMonth(month, year);
    int firstDay = DayOfWeek(1, month, year);

    return static_cast<int>(std::ceil((days + firstDay - 1) / 7.0));
  }

  // Returns a vector containing dates as they would appear on the calendar for a given week. Populates 0 if there is no day.
  inline static std::vector<int> CalendarWeek(int week, int startDay, int daysInMonth) {
    std::vector<int> res(7, 0);
    int startOfWeek = 7 * (week - 1) + 2 - startDay;

    if (startOfWeek >= 1)
      res[0] = startOfWeek;

    for (int i = 1; i < 7; ++i) {
      int day = startOfWeek + i;
      if ((day >= 1) && (day <= daysInMonth))
        res[i] = day;
    }

    return res;
  }

  constexpr static tm EncodeTimePoint(int dayOfMonth, int month, int year) noexcept {
    tm res{};
    res.tm_isdst = -1;
    SET_DAY(res, dayOfMonth);
    SET_MONTH(res, month);
    SET_YEAR(res, year);

    return res;
  }

  inline static std::string TimePointToLongString(const tm& timePoint) noexcept {
    std::string day = std::to_string(GET_DAY(timePoint));
    std::string month = MONTHS[GET_MONTH_UNSCALED(timePoint)];
    std::string year = std::to_string(GET_YEAR(timePoint));

    return std::string(day + " " + month + " " + year);
  }

  // Local time, not UTC: a calendar shows the day the user is having, and east of
  // Greenwich after 22:00 those are not the same day.
  inline static tm Today() noexcept {
    const std::time_t currentTime = std::time(nullptr);

    tm res{};
#if defined(_WIN32)
    localtime_s(&res, &currentTime);
#else
    localtime_r(&currentTime, &res);
#endif
    return res;
  }

  inline static tm PreviousMonth(const tm& timePoint) noexcept {
    int month = GET_MONTH(timePoint);
    int year = GET_YEAR(timePoint);

    if (month == 1) {
      int newDay = std::min(GET_DAY(timePoint), NumDaysInMonth(12, --year));
      return EncodeTimePoint(newDay, 12, year);
    }

    int newDay = std::min(GET_DAY(timePoint), NumDaysInMonth(--month, year));
    return EncodeTimePoint(newDay, month, year);
  }

  inline static tm NextMonth(const tm& timePoint) noexcept {
    int month = GET_MONTH(timePoint);
    int year = GET_YEAR(timePoint);

    if (month == 12) {
      int newDay = std::min(GET_DAY(timePoint), NumDaysInMonth(1, ++year));
      return EncodeTimePoint(newDay, 1, year);
    }

    int newDay = std::min(GET_DAY(timePoint), NumDaysInMonth(++month, year));
    return EncodeTimePoint(newDay, month, year);
  }

  constexpr static bool IsMinDate(const tm& timePoint) noexcept {
    return (GET_MONTH(timePoint) == 1) && (GET_YEAR(timePoint) == IMGUI_DATEPICKER_YEAR_MIN);
  }

  constexpr static bool IsMaxDate(const tm& timePoint) noexcept {
    return (GET_MONTH(timePoint) == 12) && (GET_YEAR(timePoint) == IMGUI_DATEPICKER_YEAR_MAX);
  }

  static bool ComboBox(const std::string& label, const std::vector<std::string>& items, int& v, ImFont* altFont) {
    bool res = false;

    if (altFont)
      ImGui::PushFont(altFont);
    if (ImGui::BeginCombo(label.c_str(), items[v].c_str())) {
      for (int i = 0; i < static_cast<int>(items.size()); ++i) {
        bool selected = (items[v] == items[i]);
        if (ImGui::Selectable(items[i].c_str(), &selected)) {
          v = i;
          res = true;
        }

        if (selected)
          ImGui::SetItemDefaultFocus();
      }

      ImGui::EndCombo();
    }

    if (altFont)
      ImGui::PopFont();
    return res;
  }

  // Everything the calendar needs to lay itself out, derived from the current
  // style and font instead of the constants this widget used to carry.
  //
  // The old version hardcoded a 274.5 x 301.5 popup, 30 px columns and a 20 px
  // corner radius. Those numbers describe one theme at one font size; change
  // either and the popup clips its last week row or floats in empty space. None of
  // them are free parameters - they all follow from the glyph size and the padding
  // the theme already declares.
  struct DatePickerMetrics {
    float cellWidth = 0.0f;     // one day column, text plus frame padding
    float cellHeight = 0.0f;    // one day button
    float tableWidth = 0.0f;
    float tableHeight = 0.0f;
    ImVec2 popupSize{};
    float fieldWidth = 0.0f;    // the closed combo showing the date
  };

  static DatePickerMetrics CalcDatePickerMetrics(const char* sample) {
    const ImGuiStyle& style = GetStyle();
    DatePickerMetrics m;

    // Widest thing a cell has to hold: a two digit day or a weekday abbreviation.
    float widest = CalcTextSize("00").x;
    for (const auto& day : DAYS)
      widest = ImMax(widest, CalcTextSize(day.c_str()).x);

    m.cellWidth = widest + style.FramePadding.x * 2.0f;
    m.cellHeight = GetFrameHeight();

    // Seven columns, each with cell padding on both sides, plus the outer border
    // lines the table draws.
    const float columnWidth = m.cellWidth + style.CellPadding.x * 2.0f;
    m.tableWidth = columnWidth * 7.0f + 2.0f;

    // Header row plus six week rows - six is the most any month can span, and
    // sizing for the worst case keeps the popup from resizing as months change.
    const float rowHeight = m.cellHeight + style.CellPadding.y * 2.0f;
    m.tableHeight = rowHeight * 7.0f + 2.0f;

    // Above the table sit the month/year row and the arrow row, each one frame
    // high, separated by the theme's item spacing.
    const float headerRows = GetFrameHeight() * 2.0f + style.ItemSpacing.y * 3.0f;

    m.popupSize = ImVec2(
      m.tableWidth + style.WindowPadding.x * 2.0f,
      m.tableHeight + headerRows + style.WindowPadding.y * 2.0f);

    // The closed field only has to fit the formatted date and the combo arrow.
    m.fieldWidth = CalcTextSize(sample).x + style.FramePadding.x * 2.0f
      + GetFrameHeight() + style.ItemInnerSpacing.x;

    return m;
  }

  bool DatePickerEx(const std::string& label, std::string& date, ImFont* altFont, bool clampToBorder, float itemSpacing, float width, const char* format) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
      return false;

    bool hiddenLabel = label.substr(0, 2) == "##";
    std::string myLabel = (hiddenLabel) ? label.substr(2) : label;

    const ImGuiID id = window->GetID(label.c_str());

    // One calendar position per widget instead of one shared by all of them.
    //
    // This used to be a function-level static, which meant two date fields in the
    // same window - a Beleg has Datum and Leistungsdatum - fought over a single
    // tm: opening one picker moved the other, and an empty field reset whatever
    // its neighbour was showing.
    static std::unordered_map<ImGuiID, tm> states;

    tm& v = states[id];

    // Reparsed every frame so the calendar follows the string, which the caller
    // may have changed since. An unparsable or empty value falls back to today
    // rather than to whatever was left in the struct.
    {
      tm parsed{};
      parsed.tm_isdst = -1;
      std::istringstream ss(date);
      ss >> std::get_time(&parsed, format);
      if (ss.fail() || date.empty())
        v = Today();
      else
        v = parsed;
    }

    bool res = false;

    if (!hiddenLabel) {
      Text("%s", label.c_str());
      SameLine((itemSpacing == 0.0f) ? 0.0f : GetCursorPos().x + itemSpacing);
    }

    // Rendered once so the field is measured against the format actually in use -
    // "31.12.2026" and "2026-12-31" are not the same width.
    char sample[32];
    {
      tm probe = EncodeTimePoint(30, 12, 2026);
      if (std::strftime(sample, sizeof(sample), format, &probe) == 0)
        std::snprintf(sample, sizeof(sample), "00.00.0000");
    }

    const DatePickerMetrics m = CalcDatePickerMetrics(sample);

    if (clampToBorder)
      SetNextItemWidth(GetContentRegionAvail().x);
    else
      SetNextItemWidth(width > 0.0f ? width : m.fieldWidth);

    SetNextWindowSize(m.popupSize);

    if (BeginCombo(std::string("##" + myLabel).c_str(), date.c_str())) {
      int monthIdx = GET_MONTH_UNSCALED(v);
      int year = GET_YEAR(v);

      PushItemWidth((GetContentRegionAvail().x * 0.5f));

      if (ComboBox("##CmbMonth_" + myLabel, MONTHS, monthIdx, altFont)) {
        SET_MONTH(v, monthIdx + 1);
        res = true;
      }

      PopItemWidth();
      SameLine();
      PushItemWidth(GetContentRegionAvail().x);

      if (InputInt(std::string("##IntYear_" + myLabel).c_str(), &year, 0, 0)) {
        SET_YEAR(v, std::min(std::max(IMGUI_DATEPICKER_YEAR_MIN, year), IMGUI_DATEPICKER_YEAR_MAX));
        res = true;
      }

      PopItemWidth();

      const ImGuiStyle& style = GetStyle();
      const float contentWidth = GetContentRegionAvail().x;
      const float arrowSize = GetFrameHeight();
      const float arrowButtonWidth = arrowSize * 2.0f + style.ItemSpacing.x;
      // Proportional to the frame rather than a fixed 5 px smaller: on a large
      // font the old bullet was a dot, on a small one it swallowed the arrows.
      const float bulletSize = ImMax(arrowSize * 0.55f, 6.0f);
      const float bulletButtonWidth = bulletSize + style.ItemSpacing.x;
      const float combinedWidth = arrowButtonWidth + bulletButtonWidth;
      const float offset = ImMax((contentWidth - combinedWidth) * 0.5f, 0.0f);

      SetCursorPosX(GetCursorPosX() + offset);
      // Half the height makes a circle at any size; a constant 20 only did at one.
      PushStyleVar(ImGuiStyleVar_FrameRounding, arrowSize * 0.5f);
      PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
      PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
      BeginDisabled(IsMinDate(v));

      if (ArrowButtonEx(std::string("##ArrowLeft_" + myLabel).c_str(), ImGuiDir_Left, ImVec2(arrowSize, arrowSize))) {
        v = PreviousMonth(v);
        res = true;
      }

      EndDisabled();
      PopStyleColor(2);
      SameLine();
      PushStyleColor(ImGuiCol_Button, GetStyleColorVec4(ImGuiCol_Text));
      // Centred against the arrows, whatever the two heights happen to be.
      SetCursorPosY(GetCursorPosY() + (arrowSize - bulletSize) * 0.5f);

      if (ButtonEx(std::string("##ArrowMid_" + myLabel).c_str(), ImVec2(bulletSize, bulletSize))) {
        v = Today();
        res = true;
        CloseCurrentPopup();
      }
      SetItemTooltip("Heute");

      PopStyleColor();
      SameLine();
      PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
      PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
      BeginDisabled(IsMaxDate(v));

      if (ArrowButtonEx(std::string("##ArrowRight_" + myLabel).c_str(), ImGuiDir_Right, ImVec2(arrowSize, arrowSize))) {
        v = NextMonth(v);
        res = true;
      }

      EndDisabled();
      PopStyleColor(2);
      PopStyleVar();

      constexpr ImGuiTableFlags TABLE_FLAGS = ImGuiTableFlags_BordersOuter | ImGuiTableFlags_SizingFixedFit |
        ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoHostExtendY;

      if (BeginTable(std::string("##Table_" + myLabel).c_str(), 7, TABLE_FLAGS, GetContentRegionAvail())) {
        for (const auto& day : DAYS)
          TableSetupColumn(day.c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHeaderWidth, m.cellWidth);

        PushStyleColor(ImGuiCol_HeaderHovered, GetStyleColorVec4(ImGuiCol_TableHeaderBg));
        PushStyleColor(ImGuiCol_HeaderActive, GetStyleColorVec4(ImGuiCol_TableHeaderBg));
        if (altFont)
          PushFont(altFont);
        TableHeadersRow();
        PopStyleColor(2);
        if (altFont)
          PopFont();

        TableNextRow();
        TableSetColumnIndex(0);

        int month = monthIdx + 1;
        int firstDayOfMonth = DayOfWeek(1, month, year);
        int numDaysInMonth = NumDaysInMonth(month, year);
        int numWeeksInMonth = NumWeeksInMonth(month, year);

        const tm heute = Today();
        const bool sameMonth = (GET_MONTH(heute) == month) && (GET_YEAR(heute) == year);

        for (int i = 1; i <= numWeeksInMonth; ++i) {
          for (const auto& day : CalendarWeek(i, firstDayOfMonth, numDaysInMonth)) {
            if (day != 0) {
              PushStyleVar(ImGuiStyleVar_FrameRounding, m.cellHeight * 0.5f);

              const bool selected = day == GET_DAY(v);
              if (!selected) {
                PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
              }
              // Today gets an outline when it is not the selected day, so the
              // calendar answers "where am I" without a second glance.
              const bool istHeute = sameMonth && (GET_DAY(heute) == day);
              if (istHeute && !selected)
                PushStyleColor(ImGuiCol_Border, GetStyleColorVec4(ImGuiCol_TextDisabled));

              if (Button(std::to_string(day).c_str(), ImVec2(GetContentRegionAvail().x, m.cellHeight))) {
                v = EncodeTimePoint(day, month, year);
                res = true;
                CloseCurrentPopup();
              }

              if (istHeute && !selected)
                PopStyleColor();
              if (!selected)
                PopStyleColor(2);

              PopStyleVar();
            }

            if (day != numDaysInMonth)
              TableNextColumn();
          }
        }

        EndTable();
      }

      EndCombo();
    }
    if (res) {
      char buffer[32];
      if (std::strftime(buffer, sizeof(buffer), format, &v) > 0)
        date = std::string(buffer);
    }
    return res;
  }

  bool DatePicker(const std::string& label, std::string& date, bool clampToBorder, float itemSpacing, float width, const char* format) {
    return DatePickerEx(label, date, nullptr, clampToBorder, itemSpacing, width, format);
  }
} // namespace ImGui
