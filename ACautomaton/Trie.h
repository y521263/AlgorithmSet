//
//  Trie.hpp
//  ACautomaton
//
//  Created by lin on 16/4/8.
//  Copyright © 2016年 lin. All rights reserved.
//

#ifndef Trie_hpp
#define Trie_hpp

#include <iostream>
#include <cstring>
#include <string>
#include <queue>
#include <set>

const int CHILDNUM = 26;
class TrieNode{
public:
    TrieNode(){fail=NULL;isWord=false;memset(next,0,sizeof(next));}
    TrieNode(std::string& s){word=s;fail=NULL;isWord=false;memset(next,0,sizeof(next));}
    TrieNode* fail;
    TrieNode* next[CHILDNUM];
    TrieNode* & operator [] (char ch){
        return next[ (ch-'a')%CHILDNUM ];
    }
    TrieNode* & operator [] (int inx){
        return next[ inx%CHILDNUM ];
    }
    ~TrieNode(){freeNode();}
    bool isWord;
    std::string word;
private:
    void freeNode(){
        for (int i=0; i<CHILDNUM; i++) {
            if ( next[i] == NULL) continue;
            delete next[i];
        }
    }
};

class TrieTree{
public:
    TrieTree():root(new TrieNode()){}
    void addNode(std::string& s);
    bool isIn(std::string& s);
    
    //计算失败指针
    void setFail();
    
    //作匹配
    int query(std::string& s);
    void query(std::string& s,std::set<std::string>& ret);
    
    ~TrieTree(){delete root;}
private:
    TrieNode* root;
};

#endif /* Trie_hpp */
