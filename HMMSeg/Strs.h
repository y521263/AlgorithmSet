//
//  Strs.h
//  test
//
//  Created by lin on 16/3/29.
//  Copyright © 2016年 lin. All rights reserved.
//

#ifndef Strs_hpp
#define Strs_hpp

#include <vector>
#include <string>

namespace Strs {
    bool Utf8ToUnicode32(const std::string& str, std::vector<uint32_t>& vec);
    
    void Unicode32ToUtf8(std::vector<uint32_t>::iterator begin, std::vector<uint32_t>::iterator end, std::string& res);
    
    void split(std::string& s, std::string delim, std::vector< std::string >& ret);

}
#endif /* Strs_hpp */
