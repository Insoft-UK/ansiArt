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


#include "ansi.hpp"
#include <array>
#include <fstream>

int ANSI::loadImage(const std::string& filename) {
    _image = loadBMPGraphicFile(filename);
    if (_image == nullptr) return -1;
    
    return 0;
}

std::string ANSI::generateArt(void) {
    std::string art;
    
    if (_image->bitWidth == 8) art = generate256ColorArt();
    if (_image->bitWidth == 4) art = generateColorArt();
    
    return art;
}

std::string ANSI::generateColorArt(void) {
    std::string art;
    uint8_t *pixel = (uint8_t *)_image->data;
    uint8_t color;
    
    union {
        struct {
            uint8_t l: 4;
            uint8_t h: 4;
        };
        uint8_t v;
    } byte;
    
    art = "ANSI_ART=$(cat <<EOF\n";
    for (auto y = 0; y < _image->height; y++) {
        for (auto x = 0; x < _image->width; x+=2) {
            byte.v = *pixel++;
            color = byte.h;
            color += (color < 8) ? 40 : 92;
            art.append(R"(\e[0;)");
            art.append(std::to_string(color));
            art.append("m ");
            
            color = byte.l;
            color += (color < 8) ? 40 : 92;
            art.append(R"(\e[0;)");
            art.append(std::to_string(color));
            art.append("m ");
        }
        art.append(canvasColor());
        art.append("\n");
    }
    art += "EOF\n)\n";
    return art;
}

std::string ANSI::generate256ColorArt(void) {
    std::string art;
    uint8_t *pixel = (uint8_t *)_image->data;
    uint8_t color;
    
    art = "ANSI_ART=$(cat <<EOF\n";
    for (auto y = 0; y < _image->height; y++) {
        for (auto x = 0; x < _image->width; x++) {
            color = *pixel++;
            if (transparencyIndex == color) {
                art.append(canvasColor() + " ");
            } else {
                art.append(R"(\e[48;5;)");
                art.append(std::to_string(color));
                art.append("m ");
            }
        }
        art.append(canvasColor());
        art.append("\n");
    }
    art += "EOF\n)\n";
    return art;
}

std::string ANSI::canvasColor(void) {
    std::string str;
    
    if (_canvasIndex == _transparencyIndex) return R"(\e[0;m)";
    
    str = R"(\e[48;5;)";
    str.append(std::to_string(_canvasIndex));
    str.append("m");
    
    return str;
}
