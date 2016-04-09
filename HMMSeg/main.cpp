
//
//  main.cpp
//  test
//
//  Created by lin on 16/3/24.
//  Copyright © 2016年 lin. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <cmath>

#include "HMMSeg.h"

using namespace std;

int main(int argc, const char * argv[]) {
    
    std::string ss="小明硕士毕业于中国科学院计算所";
    HMMSeg hs("./icwb2-data/training/msr_training.utf8");
    
    std::vector<std::string> vec;
    hs.Cut(ss, vec);
    for (int i=0; i<vec.size(); i++) {
        std::cout<<vec[i]<<"/";
    }
    std::cout<<"\n";
    return 0;
}