// The MIT License (MIT)
//
// Copyright (c) 2024 Insoft
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include <stdio.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include <filesystem>

#include "ansi.hpp"
#include "image.hpp"

#include "../version_code.h"
#define NAME "ANSI Art Generator"
#define COMMAND_NAME "ansiart"

bool verbose = false;

enum class MessageType {
    Warning,
    Error,
    Verbose
};

std::ostream& operator<<(std::ostream& os, MessageType type) {
    switch (type) {
        case MessageType::Error:
            os << R"(\e[1;91mError\e[0m: )";
            break;

        case MessageType::Warning:
            os << R"(\e[1;93mWarning\e[0m: )";
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

void version(void) {
    using namespace std;
    std::cout
    << "Copyright (C) 2023-" << YEAR << " Insoft. All rights reserved.\n"
    << "Insoft "<< NAME << " version, " << VERSION_NUMBER << " (BUILD " << VERSION_CODE << ")\n"
    << "Built on: " << DATE << "\n"
    << "Licence: MIT License\n\n"
    << "For more information, visit: http://www.insoft.uk\n";
}

void error(void) {
    std::cout << COMMAND_NAME << ": try '" << COMMAND_NAME << " --help' for more information\n";
    exit(0);
}

void info(void) {
    using namespace std;
    std::cout
    << "          ***********     \n"
    << "        ************      \n"
    << "      ************        \n"
    << "    ************  **      \n"
    << "  ************  ******    \n"
    << "************  **********  \n"
    << "**********    ************\n"
    << "************    **********\n"
    << "  **********  ************\n"
    << "    ******  ************  \n"
    << "      **  ************    \n"
    << "        ************      \n"
    << "      ************        \n"
    << "    ************          \n\n"
    << "Copyright (C) 2023-" << YEAR << " Insoft. All rights reserved.\n"
    << "Insoft " << NAME << " `" << std::string(YEAR).substr(2) << "\n\n";
}


void help(void) {
    using namespace std;
    std::cout
    << "Copyright (C) 2023-" << YEAR << " Insoft. All rights reserved.\n"
    << "Insoft "<< NAME << " version, " << VERSION_NUMBER << " (BUILD " << VERSION_CODE << ")\n"
    << "\n"
    << "Usage: " << COMMAND_NAME << " <input-file> [-o <output-file>]\n"
    << "\n"
    << "Options:\n"
    << "  -o <output-file>        Specify the filename for generated ANSI art.\n"
    << "  -t <transparency-color> Set the transparency color. (ignored in emoji mode)\n"
    << "  -x2                     Sets horizontal double width mode, which causes each horizontal unit in\n"
    << "                          the output to be doubled in size..\n"
    << "  -e                      Use Emoji color blocks.\n"
    << "\n"
    << "Additional Commands:\n"
    << "  " << COMMAND_NAME << " {--version | --help }\n"
    << "    --version              Display the version information.\n"
    << "    --help                 Show this help message.\n";
}




int main(int argc, const char * argv[])
{
    if ( argc == 1 ) {
        error();
        return 0;
    }
    
    std::string out_filename, in_filename;
    ANSI ansi = ANSI();
    
    bool emoji = false;
    
    for( int n = 1; n < argc; n++ ) {
        if (*argv[n] == '-') {
            std::string args(argv[n]);
            
            if (args == "-e") {
                emoji = true;
                continue;
            }
            
            if (args == "-t") {
                if (++n > argc) error();
                ansi.transparencyIndex = atoi(argv[n]);
                continue;
            }
            
            if (args == "-x2") {
                ansi.horizontalDoubleWidth = true;
                continue;
            }
            
            if (args == "-o") {
                if (++n > argc) error();
                out_filename = argv[n];
                continue;
            }
            
            if (args == "-n") {
                if (++n > argc) error();
                ansi.setName(argv[n]);
                continue;
            }
            
            if (args == "--help") {
                help();
                return 0;
            }
            
            if (args == "--version") {
                version();
                return 0;
            }
            
            error();
            return 0;
        }
        in_filename = argv[n];
    }
    
    info();
    
    
    
    if (!std::filesystem::exists(in_filename)) {
        std::cout << MessageType::Error << "File '" << in_filename << "' not found.\n";
        return -1;
    }
    
    if (out_filename.empty()) {
        out_filename = std::filesystem::path(in_filename).replace_extension(".sh");
    }
    
    if (ansi.loadImage(in_filename) != 0) {
        std::cout << MessageType::Error << "Please provide a valid image file.\n";
        return -1;
    }
    
    std::string art;
    
    if (emoji) {
        art = ansi.getEmojiImageArt();
    } else {
        art = ansi.getColorImageArt();
    }
    
    std::ofstream outfile;
    outfile.open(out_filename, std::ios::out | std::ios::binary);
    if(!outfile.is_open()) {
        std::cout << "UTF-8 File '" << out_filename << "' Failed.\n";
        return -1;
    }
    
    outfile.write(art.c_str(), art.length());
    outfile.close();
    
    std::cout << (emoji ? "Emoji" : "ANSI") << " Image Art Generated!\n";
    std::cout << "UTF-8 File '" << out_filename << "' Succefuly Created.\n";
    
    return 0;
}

