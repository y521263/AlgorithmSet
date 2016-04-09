//
//  Strs.cpp
//  test
//
//  Created by lin on 16/3/29.
//  Copyright © 2016年 lin. All rights reserved.
//

#include "Strs.h"

namespace Strs {
    bool Utf8ToUnicode32(const std::string& str, std::vector<uint32_t>& vec) {
        uint32_t tmp;
        vec.clear();
        for(size_t i = 0; i < str.size();) {
            if(!(str[i] & 0x80)) { // 0xxxxxxx
                // 7bit, total 7bit
                tmp = (uint8_t)(str[i]) & 0x7f;
                i++;
            } else if ((uint8_t)str[i] <= 0xdf && i + 1 < str.size()) { // 110xxxxxx
                // 5bit, total 5bit
                tmp = (uint8_t)(str[i]) & 0x1f;
                
                // 6bit, total 11bit
                tmp <<= 6;
                tmp |= (uint8_t)(str[i+1]) & 0x3f;
                i += 2;
            } else if((uint8_t)str[i] <= 0xef && i + 2 < str.size()) { // 1110xxxxxx
                // 4bit, total 4bit
                tmp = (uint8_t)(str[i]) & 0x0f;
                
                // 6bit, total 10bit
                tmp <<= 6;
                tmp |= (uint8_t)(str[i+1]) & 0x3f;
                
                // 6bit, total 16bit
                tmp <<= 6;
                tmp |= (uint8_t)(str[i+2]) & 0x3f;
                
                i += 3;
            } else if((uint8_t)str[i] <= 0xf7 && i + 3 < str.size()) { // 11110xxxx
                // 3bit, total 3bit
                tmp = (uint8_t)(str[i]) & 0x07;
                
                // 6bit, total 9bit
                tmp <<= 6;
                tmp |= (uint8_t)(str[i+1]) & 0x3f;
                
                // 6bit, total 15bit
                tmp <<= 6;
                tmp |= (uint8_t)(str[i+2]) & 0x3f;
                
                // 6bit, total 21bit
                tmp <<= 6;
                tmp |= (uint8_t)(str[i+3]) & 0x3f;
                
                i += 4;
            } else {
                return false;
            }
            vec.push_back(tmp);
        }
        return true;
    }
    void Unicode32ToUtf8(std::vector<uint32_t>::iterator begin, std::vector<uint32_t>::iterator end, std::string& res) {
        res.clear();
        uint32_t ui;
        while(begin != end) {
            ui = *begin;
            if(ui <= 0x7f) {
                res += char(ui);
            } else if(ui <= 0x7ff) {
                res += char(((ui >> 6) & 0x1f) | 0xc0);
                res += char((ui & 0x3f) | 0x80);
            } else if(ui <= 0xffff) {
                res += char(((ui >> 12) & 0x0f) | 0xe0);
                res += char(((ui >> 6) & 0x3f) | 0x80);
                res += char((ui & 0x3f) | 0x80);
            } else {
                res += char(((ui >> 18) & 0x03) | 0xf0);
                res += char(((ui >> 12) & 0x3f) | 0x80);
                res += char(((ui >> 6) & 0x3f) | 0x80);
                res += char((ui & 0x3f) | 0x80);
            }
            begin ++;
        }
    }
    void split(std::string& s, std::string delim, std::vector< std::string >& ret)
    {
        size_t last = 0;
        size_t index = s.find(delim, last);
        while (index != std::string::npos)
        {
            ret.push_back(s.substr(last, index - last));
            last = index + delim.length();
            index = s.find(delim, last);
        }
        if (last <s.length())
            ret.push_back(s.substr(last, s.length() - last));
    }
    
}