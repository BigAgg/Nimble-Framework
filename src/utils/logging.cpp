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
static bool loggingstarted = false;

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

	/// Reduces what source_location gives us to just the function name.
	///
	/// MSVC returns the full decorated signature from __FUNCSIG__, so a log line
	/// about a slow request arrives as three hundred characters of template
	/// parameters with the message hidden at the end. GCC and Clang are shorter but
	/// still print the return type and the whole parameter list.
	///
	/// The name sits between the last space before the argument list and the '('
	/// that opens it. Scanning from the '(' backwards also skips over return types
	/// that contain spaces, which is what makes "class std::basic_string<...>
	/// __cdecl net::HttpClient::Send" collapse to "net::HttpClient::Send".
	///
	/// A lambda comes out as the function it was written in - GCC spells the whole
	/// "ns::f()::<lambda(int)>::operator()" out, and the enclosing name is what a
	/// reader needs to find the line anyway.
	static std::string kurzerName(const char* signatur) {
		const std::string_view voll{ signatur };

		// Find the '(' that starts the parameter list, skipping any that appear
		// inside template arguments of the return type.
		std::size_t tiefe = 0;
		std::size_t klammer = std::string_view::npos;
		for (std::size_t i = 0; i < voll.size(); ++i) {
			if (voll[i] == '<')
				++tiefe;
			else if (voll[i] == '>' && tiefe > 0)
				--tiefe;
			else if (voll[i] == '(' && tiefe == 0) {
				klammer = i;
				break;
			}
		}
		if (klammer == std::string_view::npos)
			return std::string{ voll };	// Not a signature we recognise; leave it.

		// Walk back to the start of the qualified name, stopping at whatever
		// separates it from the return type and calling convention.
		std::size_t start = klammer;
		tiefe = 0;
		while (start > 0) {
			const char c = voll[start - 1];
			if (c == '>')
				++tiefe;
			else if (c == '<' && tiefe > 0)
				--tiefe;
			else if (tiefe == 0 && (c == ' ' || c == '*' || c == '&'))
				break;
			--start;
		}

		std::string_view name = voll.substr(start, klammer - start);
		return name.empty() ? std::string{ voll } : std::string{ name };
	}

	void log(const std::string& type, const std::string& msg, const std::source_location& location) {
		if (!loggingstarted)
			return;
		const std::string filename = std::filesystem::path(location.file_name()).filename().string();
		const std::string funktion = kurzerName(location.function_name());
		if (type == "[ERROR]") {
			std::string fullMessage = strings::formatString(
				"[%s:%u %s] %s",
				filename.c_str(),
				static_cast<unsigned>(location.line()),
				funktion.c_str(),
				msg.c_str()
			);
			std::cerr << strings::GetTimestamp() << "\t" << type << "\t" << fullMessage << "\n";
			lastError = fullMessage;
			errors.push_back(lastError);
			infos.push_back("[ERROR] " + fullMessage);
			logfile.flush();
		}
		else if (type == "[WARNING]") {
			std::string fullMessage = strings::formatString(
				"[%s] %s",
				funktion.c_str(),
				msg.c_str()
			);
			lastWarning = fullMessage;
			warnings.push_back(lastWarning);
			infos.push_back("[WARNING] " + msg);
			std::cout << strings::GetTimestamp() << "\t" << type << "\t" << fullMessage << "\n";
			logfile.flush();
		}
		else if (type == "[INFO]") {
			std::string fullMessage = strings::formatString(
				"%s",
				msg.c_str()
			);
			infos.push_back("[INFO] " + fullMessage);
			std::cout << strings::GetTimestamp() << "\t" << type << "\t" << fullMessage << "\n";
			logfile.flush();
		}
		else if (type == "[FATAL]") {
			std::string fullMessage = strings::formatString(
				"[%s:%u %s] %s",
				filename.c_str(),
				static_cast<unsigned>(location.line()),
				funktion.c_str(),
				msg.c_str()
			);
			std::cerr << strings::GetTimestamp() << "\t" << type << "\t" << fullMessage << "\n";
			logfile.flush();
			lastError = fullMessage;
			errors.push_back(lastError);
			infos.push_back("[FATAL] " + fullMessage);
			throw(std::runtime_error((type + "\t" + fullMessage).c_str()));
		}
	}
	void startlogging(const std::string& path, const std::string& filename) {
		loggingstarted = true;
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
		loggingstarted = false;
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
	std::vector<std::string> GetAllMessages() {
		return infos;
	}
}