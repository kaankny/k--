#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include "Logger.hpp"
#include "messages.h"

class Compiler {
public:
    static bool compile(const std::string &assemblyFile, const std::string &executableName) {
        // Assembly kodunu derleme
        std::string assembleCmd = "nasm -o output.obj -f win64 " + assemblyFile;
		int ret = 0;
		system("echo %PATH%");
        system(assembleCmd.c_str());
        if (ret != 0) {
            Logger::getInstance().log(LogLevel::ERROR, "Assembly failed.");
            return false;
        }


		assembleCmd = "gcc -m64 -no-pie -o " + executableName + " output.obj";
		system(assembleCmd.c_str());
		if (ret != 0) {
			Logger::getInstance().log(LogLevel::ERROR, "Linking failed.");
			return false;
		}

        // Link ile bağlama
        // std::string linkCmd = "link /ENTRY:main /SUBSYSTEM:CONSOLE /OUT:" + executableName + ".exe out.obj";
        // std::cout << linkCmd << std::endl;
        // ret = system(linkCmd.c_str());
        // if (ret != 0) {
        //     Logger::getInstance().log(LogLevel::ERROR, "Linking failed.");
        //     return false;
        // }

        // // Obj dosyasını link ederek çalıştırılabilir dosya oluşturma
        // ret = system("ld -o out out.obj --entry=main");
		// if (ret != 0) {
        //     Logger::getInstance().log(LogLevel::ERROR, "Linking failed.");
        //     return false;
        // }
		
		// GCC ile linkleme
		// std::cout << "gcc -no-pie -o a.out out.obj" << std::endl;	
		// ret = system("gcc -no-pie -o a.out out.obj");
		// if (ret != 0) {
		// 	Logger::getInstance().log(LogLevel::ERROR, "Linking failed.");
		// 	return false;
		// }


        return true;
    }
};
