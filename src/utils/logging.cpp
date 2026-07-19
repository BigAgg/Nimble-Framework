#include "utils/logging.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <cstdio>
#include <streambuf>
#include <string_view>
#ifdef _WIN32
#include <crtdbg.h>
#include <cstdio>
#endif

static std::string lastWarning = "";
static std::vector<std::string> warnings;
static std::string lastError = "";
static std::vector<std::string> errors;
static std::vector<std::string> infos;

// Outsource to utils???
namespace strings {
	static std::string GetTimestamp() {
		const auto now = std::chrono::system_clock::now();
		std::time_t time = std::chrono::system_clock::to_time_t(now);
		std::tm tm{};

#ifdef _WIN32
		localtime_s(&tm, &time);
#else
		localtime_r(&time, &tm);
#endif

		std::stringstream ss;
		ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
		return ss.str();
	}
}

namespace logging {
	static std::fstream logfile;
	static std::string logfilePath;
	static std::string logfileName;
	static std::streambuf* oldOutBuf;
	static std::streambuf* oldCerrBuf;

	namespace fs = std::filesystem;
	
	void log(const std::string &type, const std::string &msg, const std::source_location& location){
		const std::string filename = std::filesystem::path(location.file_name()).filename().string();
		if (type == "[ERROR]") {
			std::string fullMessage = strings::formatString(
				"[%s:%u %s] %s",
				filename.c_str(),
				static_cast<unsigned>(location.line()),
				location.function_name(),
				msg.c_str()
			);
			std::cerr << strings::GetTimestamp() << "\t" << type << "\t" << fullMessage << "\n";
			lastError = fullMessage;
			errors.push_back(lastError);
			infos.push_back("[ERROR] " + fullMessage);
			std::cout << strings::GetTimestamp() << "\t" << type << "\t" << fullMessage << "\n";
			logfile.flush();
		}
		else if (type == "[WARNING]") {
			std::string fullMessage = strings::formatString(
				"[%s] %s",
				location.function_name(),
				msg.c_str()
			);
			lastWarning = fullMessage;
			warnings.push_back(lastWarning);
			infos.push_back("[WARNING] " + msg);
			std::cout << strings::GetTimestamp() << "\t" << type << "\t" << fullMessage << "\n";
		}
		else if (type == "[INFO]") {
			std::string fullMessage = strings::formatString(
				"[%s] %s",
				location.function_name(),
				msg.c_str()
			);
			infos.push_back("[INFO] " + fullMessage);
			std::cout << strings::GetTimestamp() << "\t" << type << "\t" << fullMessage << "\n";
		}
		logfile.flush();
	}
	void startlogging(const std::string& path, const std::string& filename) {
		if (!fs::exists(path))
			fs::create_directories(path);
		std::ofstream file(path + "/" + filename, std::ios::app);
		file.close();
		//freopen((path + "/" + filename).c_str(), "w", stdout);
		//freopen((path + "/" + filename).c_str(), "a", stderr);
		if (fs::is_directory(path))
			fs::create_directories(path);
		logfilePath = path;
		logfileName = filename;
		logfile.open(path + "/" + filename, std::ios::out);
		oldOutBuf = std::cout.rdbuf();
		oldCerrBuf = std::cerr.rdbuf();
		std::cout.rdbuf(logfile.rdbuf());
		std::cerr.rdbuf(logfile.rdbuf());
	}
	void stoplogging() {
		logfile.flush();
		logfile.close();
		std::cout.rdbuf(oldOutBuf);
		std::cerr.rdbuf(oldCerrBuf);
	}
	void backuplog(const std::string& path, bool crash) {
		// Check if the logfile exists
		if (!fs::exists(logfilePath + "/" + logfileName)) {
			return;
		}
		// Create Directories
		if (!fs::exists(path)) {
			fs::create_directories(path);
		}
		// Creating outfile string
		std::string outFile;
		std::string timestamp = strings::GetTimestamp();
		for (char& c : timestamp) {
			if (c == ':')
				c = '.';
		}
		if (crash)
			outFile = path + "/crash_" + timestamp + ".txt";
		else
			outFile = path + "/" + logfileName + timestamp + ".txt";
		LOG_INFO("%s", outFile.c_str());
		// Copying logfile to desired Directory
		std::cout.flush();
		std::cerr.flush();
		logfile.flush();
		std::ofstream out(outFile, std::ios::binary);
		std::ifstream in(logfilePath + "/" + logfileName, std::ios::binary);
		out << in.rdbuf();
		out.close();
		in.close();
	}
	void deletelog(const std::string& path) {
		std::remove(path.c_str());
	}

	std::string GetLastError() {
		return lastError;
	}
	std::string GetLastWarning() {
		return lastWarning;
	}
	std::vector<std::string> GetErrors() {
		return errors;
	}
	std::vector<std::string> GetWarnings() {
		return warnings;
	}
	std::vector<std::string> GetAllMessages(){
		return infos;
	}
}
