#pragma once

#include <iostream>
#include <string>

enum LogLevel
{
    INFO,
    DEBUG,
    WARNING,
    ERROR
};

class Logger
{
    public:
        static Logger& getInstance()
        {
            static Logger instance;
            return instance;
        }

        void log(LogLevel level, std::string message)
        {
            switch (level)
            {
                // print colorful and add timestamp
                case LogLevel::INFO:
                    std::cout << "\033[1;32m[INFO] \033[0m" << message  << std::endl;
                    break;
				case LogLevel::DEBUG:
					std::cout << "\033[1;34m[DEBUG] \033[0m" << message  << std::endl;
					break;
				case LogLevel::WARNING:
					std::cout << "\033[1;33m[WARNING] \033[0m" << message  << std::endl;
					break;
				case LogLevel::ERROR:
					std::cout << "\033[1;31m[ERROR] \033[0m" << message  << std::endl;
					break;
                default:
                    std::cout << "Invalid log level." << std::endl;
                    break;
            }
        }

    private:
        Logger() {}
        Logger(Logger const&) = delete;
        void operator=(Logger const&) = delete;
};