/*
 The MIT License (MIT)
 
 Copyright (c) 2024 Insoft. All rights reserved.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */


#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <fstream>
#include <iomanip>

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "ansi.hpp"
#include "image.hpp"

#include "build.h"

bool verbose = false;

enum class MessageType {
    Warning,
    Error,
    Verbose
};

std::ostream& operator<<(std::ostream& os, MessageType type) {
    switch (type) {
        case MessageType::Error:
            os << "\033[91;1;1;1merror\033[0m: ";
            break;

        case MessageType::Warning:
            os << "\033[98;1;1;1mwarning\033[0m: ";
            break;
            
        case MessageType::Verbose:
            os << ": ";
            break;

        default:
            os << ": ";
            break;
    }

    return os;
}

void usage(void)
{
    std::cout << "Copyright (C) 2024 Insoft. All rights reserved.\n";
    std::cout << "Insoft ANSI Code Art Creator.\n\n";
    std::cout << "Usage: bmp2ansi filename\n";
    std::cout << "\n";
    std::cout << "Usage: bmp2ansi {-version | -help}\n";
}

void version(void) {
    std::cout
    << "ANSI Art Generator v"
    << (unsigned)__BUILD_NUMBER / 100000 << "."
    << (unsigned)__BUILD_NUMBER / 10000 % 10 << "."
    << (unsigned)__BUILD_NUMBER / 1000 % 10 << "."
    << std::setfill('0') << std::setw(3) << (unsigned)__BUILD_NUMBER % 1000
    << "\n";
}

void error(void)
{
    std::cout << "bmp2ansi: try 'bmp2ansi -help' for more information\n";
    exit(0);
}

void info(void) {
    std::cout << "Copyright (c) 2023-2024 Insoft. All rights reserved\n";
    int rev = (unsigned)__BUILD_NUMBER / 1000 % 10;
    std::cout << "ANSI Art Creator v" << (unsigned)__BUILD_NUMBER / 100000 << "." << (unsigned)__BUILD_NUMBER / 10000 % 10 << (rev ? "." + std::to_string(rev) : "") << " BUILD " << std::setfill('0') << std::setw(3) << __BUILD_NUMBER % 1000 << "\n\n";
}


int main(int argc, const char * argv[])
{
    if ( argc == 1 ) {
        error();
        return 0;
    }
    
    std::string directory, name, out_filename, in_filename;
   
    
    for( int n = 1; n < argc; n++ ) {
        if (*argv[n] == '-') {
            std::string args(argv[n]);
            
            
            if (args == "-o") {
                if (++n > argc) error();
                out_filename = argv[n];
                continue;
            }
            
            if (args == "-help") {
                usage();
                return 0;
            }
            
            if (args == "-version") {
                version();
                return 0;
            }
            
            error();
            return 0;
        }
        in_filename = argv[n];
    }
    
    
    if (name.empty()) {
        name = regex_replace(in_filename, std::regex(R"(\.\w+$)"), "");
        size_t pos = name.rfind("/");
        name = name.substr(pos + 1, name.length() - pos);
    }
    
    
    if (out_filename.empty()) {
        size_t pos = in_filename.rfind(".bmp");
        if (pos != std::string::npos) {
            out_filename = in_filename.substr(0, pos) + ".sh";
        } else {
            directory = in_filename;
            out_filename = directory + ".sh";
        }
    }
    
    info();
    
    // Start measuring time
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    ANSI ansi = ANSI(in_filename);
    
    if (ansi.createBashFile(out_filename) == 0) {
        // Display elasps time in secononds.
        double delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
        printf("Completed in %.3f seconds.\n", delta_us * 1e-6);
        std::cout << "UTF-8 File '" << out_filename << "' Succefuly Created.\n";
        return 0;
    }
    
    std::cout << "UTF-8 File '" << out_filename << "' Failed.\n";
    return -1;
}

