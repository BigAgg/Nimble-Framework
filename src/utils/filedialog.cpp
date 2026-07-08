#include "utils/filedialog.h"
#include <nfd.h>
#include <string>
#include <vector>
#include <utility>
#include <Windows.h>

std::string OpenFileDialog(const std::vector<std::string>& filters, std::string delimiter) {
  NFD_Init();
  nfdu8char_t* outPath;
  // Setting up filters
  std::vector<std::pair<std::string, std::string>> filterlist;
  filterlist.reserve(filters.size());
  size_t filterCount = 0;
  for (size_t i = 0; i < filters.size(); i++) {
    const std::string f = filters[i];
    size_t pos = f.find(delimiter);
    if (pos != std::string::npos) {
      filterlist.push_back(std::make_pair(f.substr(0, pos), f.substr(pos + delimiter.length())));
      filterCount++;
    }
  }
  std::vector<nfdu8filteritem_t> filter;
  filter.resize(filterCount);
  // nfdu8filteritem_t filter[filterCount];
  for (size_t i = 0; i < filterCount; i++) {
    filter[i].name = filterlist[i].first.c_str();
    filter[i].spec = filterlist[i].second.c_str();
  }
  nfdopendialogu8args_t args = {0};
  args.filterList = filter.data();
  args.filterCount = filterCount;
  nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
  std::string outStr = "";
  if (result == NFD_OKAY) {
    outStr = std::string(outPath);
    NFD_FreePathU8(outPath);
  }

  NFD_Quit();
  for (char& c : outStr) {
    if (c == '\\')
      c = '/';
  }
  return outStr;
}

std::string SaveFileDialog(const std::vector<std::string>& filters, std::string delimiter) {
  // Initialize NFD and variables
  NFD_Init();
  nfdu8char_t* outPath;
  std::vector<std::pair<std::string, std::string>> filterlist;
  filterlist.reserve(filters.size());
  size_t filterCount = 0;
  for (size_t i = 0; i < filters.size(); i++) {
    const std::string f = filters[i];
    size_t pos = f.find(delimiter);
    if (pos != std::string::npos) {
      filterlist.push_back(std::make_pair(f.substr(0, pos), f.substr(pos + delimiter.length())));
      filterCount++;
    }
  }
  std::vector<nfdu8filteritem_t> filter;
  filter.resize(filterCount);
  // nfdu8filteritem_t filter[filterCount];
  for (size_t i = 0; i < filterCount; i++) {
    filter[i].name = filterlist[i].first.c_str();
    filter[i].spec = filterlist[i].second.c_str();
  }
  nfdsavedialogu8args_t args = {0};
  args.filterList = filter.data();
  args.filterCount = filterCount;
  // Retrieving picked file
  nfdresult_t result = NFD_SaveDialogU8_With(&outPath, &args);
  // Generating the outstring
  std::string outStr = "";
  if (result == NFD_OKAY) {
    outStr = std::string(outPath);
    NFD_FreePathU8(outPath);
  }
  // Closing everything and free the path
  NFD_Quit();
  for (char& c : outStr) {
    if (c == '\\') {
      c = '/';
    }
  }
  return outStr;
}

std::string OpenDirectoryDialog() {
  NFD_Init();
  nfdu8char_t* outPath;
  nfdpickfolderu8args_t args = {0};
  nfdresult_t result = NFD_PickFolderU8_With(&outPath, &args);
  std::string outStr = "";

  if (result == NFD_OKAY) {
    outStr = std::string(outPath);
    NFD_FreePathU8(outPath);
  }

  NFD_Quit();
  for (char& c : outStr) {
    if (c == '\\')
      c = '/';
  }
  return outStr;
}

void OpenPath(const std::string& path) {
  ShellExecute(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}
