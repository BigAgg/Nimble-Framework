#include "utils/stringconverter.h"

#include <string>
#include <utility>
#include <chrono>
#include <algorithm>
#include <Windows.h>

using namespace std::chrono;

std::pair<std::string, std::string> split_at(const std::string &input, const std::string &delimiter) {
  std::size_t pos = input.find(delimiter);
  if (pos == std::string::npos)
    return {input, ""};
  return {
      input.substr(0, pos),
      input.substr(pos + delimiter.length())
  };
}

std::string to_lower (const std::string& s) {
  std::string out = s;
  std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) { return std::tolower(c); });
  return out;
}

std::string get_date (const std::string& fmt) {
  auto now = system_clock::now();
  auto today = floor<days>(now);
  year_month_day ymd{today};
  return std::vformat(fmt, std::make_format_args(ymd));
}

std::string get_user () {
  return to_lower(std::getenv("USERNAME"));
}

static bool s_CanDecodeAsCodepage (const std::vector<unsigned char>& bytes, UINT codePage) {
  if (bytes.empty()) return true;

  int result = MultiByteToWideChar(
      codePage,
      MB_ERR_INVALID_CHARS,
      reinterpret_cast<LPCCH>(bytes.data()),
      static_cast<int>(bytes.size()),
      nullptr,
      0);
  return result > 0;
}
std::string to_utf8 (const std::string& content) {
  std::string out = content;
  std::vector<unsigned char> bytes(content.begin(), content.end());
  if (!s_CanDecodeAsCodepage(bytes, GetACP()))
    return out;
  if (content.empty())
    return out;
  // ANSI -> UTF-16
  int wlen = MultiByteToWideChar(CP_ACP, 0, out.data(), (int)out.size(), nullptr, 0);
  std::wstring wstr(wlen, L'\0');
  MultiByteToWideChar(CP_ACP, 0,
                      out.data(), (int)out.size(),
                      &wstr[0], wlen);

  // UTF-16 -> UTF-8
  int u8len = WideCharToMultiByte(CP_UTF8, 0,
                                  wstr.data(), (int)wstr.size(),
                                  nullptr, 0, nullptr, nullptr);
  std::string u8(u8len, '\0');
  WideCharToMultiByte(CP_UTF8, 0,
                      wstr.data(), (int)wstr.size(),
                      &u8[0], u8len, nullptr, nullptr);
  return u8;
}

std::string from_utf8 (const std::string& utf8) {

  if (utf8.empty()) return {};

  // UTF-8 -> UTF-16
  int wlen = MultiByteToWideChar(
      CP_UTF8, 0,
      utf8.data(), (int)utf8.size(),
      nullptr, 0);

  std::wstring wstr(wlen, L'\0');

  MultiByteToWideChar(
      CP_UTF8, 0,
      utf8.data(), (int)utf8.size(),
      &wstr[0], wlen);

  // UTF-16 -> ANSI
  int alen = WideCharToMultiByte(
      CP_ACP, 0,
      wstr.data(), (int)wstr.size(),
      nullptr, 0, nullptr, nullptr);

  std::string ansi(alen, '\0');

  WideCharToMultiByte(
      CP_ACP, 0,
      wstr.data(), (int)wstr.size(),
      &ansi[0], alen, nullptr, nullptr);

  return ansi;
}

