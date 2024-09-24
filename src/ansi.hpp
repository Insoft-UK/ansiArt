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

#ifndef ANSI_HPP
#define ANSI_HPP

#include <stdio.h>
#include <stdint.h>

#include <string>
#include <vector>

#include "image.hpp"

class ANSI {
public:
    const uint8_t &transparencyIndex, &canvasIndex;
    ANSI() : transparencyIndex(_transparencyIndex), canvasIndex(_canvasIndex) {
    }
    
    ~ANSI() {
        reset(_image);
    }
    
    int loadImage(const std::string &filename);
    std::string generateArt(void);
    
    void setTransparencyIndex(uint8_t newValue) {
        _transparencyIndex = newValue;
    }
    
    void setCanvasIndex(uint8_t newValue) {
        _canvasIndex = newValue;
    }
private:
    TImage *_image;
    uint8_t _transparencyIndex = 201;
    uint8_t _canvasIndex = 201;
    
    std::string generateColorArt(void);
    std::string generate256ColorArt(void);
    std::string canvasColor(void);
};

#endif /* ANSI_HPP */
