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
    
    if (_image->bitWidth == 4) {
        convertPixmapTo8BitPixmapNoCopy(_image);
    }
    
    return 0;
}

std::string ANSI::getColorImageArt(void) {
    std::string art;
    
    art = generateColorImageArt();
    
    return art;
}

std::string ANSI::generateColorImageArt(void) {
    std::string art, code;
    uint8_t *pixel = (uint8_t *)_image->data;
    uint8_t color;
    
    int prevColor = -1;
    
    art = _variableName + "=$(cat <<EOF\n";
    for (auto y = 0; y < _image->height; y++) {
        for (auto x = 0; x < _image->width; x++) {
            color = *pixel++;
            if (prevColor != color)
                art.append(getBackgroundColorCode(color));
            art.append(" ");
            if (horizontalDoubleWidth) art.append(" ");
            prevColor = color;
        }
        if (prevColor == transparencyIndex)
            art.append(getBackgroundColorCode(transparencyIndex));
        art.append("\n");
    }
    art += "EOF\n)\n";
    return art;
}


std::string ANSI::getBackgroundColorCode(const int num) {
    std::string str;
    
    if (num == transparencyIndex) return R"(\e[0;m)";
    
    if (num < 16){
        str.append(R"(\e[0;)");
        str.append(std::to_string((num < 8) ? num + 40 : num + 92));
    }
    else {
        str = R"(\e[48;5;)";
        str.append(std::to_string(num));
    }
    
    str.append("m");
    
    return str;
}
