
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cassert>

#include <fcntl.h>
#include <unistd.h>

#include "Logger.h"

Logger::Logger(const std::string &afilename) : filename(afilename)
{ }

Logger::~Logger() { Close(); }

void Logger::Open(const std::string &path){
    std::lock_guard<std::recursive_mutex> guard(lock);

    // Construct lock file path
    lock_path = util::string_format("%s/%s.lock", path.c_str(), filename.c_str());

    // Open the log file for appending
    file_stream.open(filename, std::ios::app);
    if (!file_stream.is_open()) {
        // Handle error: Could not open log file
        // For this example, we'll just print to stderr
        std::cerr << "Error: Could not open log file: " << filename << std::endl;
    }
}

void Logger::Close(){
    std::lock_guard<std::recursive_mutex> guard(lock);

    if (file_stream.is_open()) {
        file_stream.close();
    }
}

void Logger::SetLevel(int level){
    m_LogLevel = level;
}

void Logger::Log(int level, const char *format, ...){
    if (m_LogLevel >= level) {
        std::lock_guard<std::recursive_mutex> guard(lock);

        if (file_stream.is_open()) {
            va_list args;
            va_start(args, format);
            vfprintf(file_stream, format, args);
            va_end(args);
            file_stream << std::endl; // Add newline
            file_stream.flush();      // Ensure immediate writing
        } else {
            // Log to stderr if file not open
            va_list args;
            va_start(args, format);
            vfprintf(stderr, format, args);
            va_end(args);
            fprintf(stderr, "\n");
        }
    }
}
void Logger::Info(const char *message){
    Log(LogLevelInfo, "[INFO]: %s", message);
}

void Logger::Error(const char *message){
    Log(LogLevelError, "[ERROR]: %s", message);
}

void Logger::Warn(const char *message){
    Log(LogLevelWarning, "[WARNING]: %s", message);
}