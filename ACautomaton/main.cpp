//
//  main.cpp
//  ACautomaton
//
//  Created by lin on 16/4/8.
//  Copyright © 2016年 lin. All rights reserved.
//

#include <iostream>
#include <vector>

#include "Trie.h"

void test()
{
    TrieTree root;
    std::vector<std::string> vec = {"say","she","shr","he","her"};
    for (int i=0; i<vec.size(); i++) {
        root.addNode(vec[i]);
    }
    root.setFail();
    std::string ss = "yasherhs";
    int num = root.query(ss);
}
int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    test();
    return 0;
}
