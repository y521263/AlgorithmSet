//
//  Trie.cpp
//  ACautomaton
//
//  Created by lin on 16/4/8.
//  Copyright © 2016年 lin. All rights reserved.
//

#include "Trie.h"

void TrieTree::addNode(std::string& s)
{
    TrieNode* node = root;
    for (int i=0; i<s.size(); i++) {
        if ( (*node)[s[i]] == NULL) {
            (*node)[s[i]] = new TrieNode();
        }
        node = (*node)[s[i]];
    }
    node->isWord = true;
    node->word = s;
}
bool TrieTree::isIn(std::string& s)
{
    TrieNode* node = root;
    for (int i=0; i<s.size(); i++) {
        if ((*node)[s[i]] == NULL) return false;
        node = (*node)[s[i]];
    }
    if (node != NULL && node->isWord) return true;
    return false;
}
void TrieTree::setFail()
{
    //使用队列 利用bfs遍历完所有节点
    std::queue<TrieNode*> q;
    q.push(root);
    while (!q.empty()) {
        TrieNode* tmp = q.front();
        TrieNode* p = tmp->fail;
        q.pop();
        for (int i=0; i<CHILDNUM; i++) {
            if( !tmp->next[i] ) continue;
            //父节点的失败指针 作为子节点的失败指针
            p = tmp->fail;
            
            //当p->next[i]为空 继续寻找fail指向的节点
            while (p && !p->next[i]) p = p->fail;
            
            tmp->next[i]->fail = p ? p->next[i]:root;
            
            //子节点放入队列 待后续构造
            q.push(tmp->next[i]);
        }
    }
}
int TrieTree::query(std::string& s)
{
    std::set<std::string> ret;
    query(s, ret);
    return ret.size();
}
void TrieTree::query(std::string& s,std::set<std::string>& ret)
{
    TrieNode* p = root;
    for (int i=0; i<s.size(); i++) {
        //匹配不上 则使用失败指针
        while( p && !(*p)[s[i]]) p = p->fail;
        
        //这里的 p不动 下次循环再次从这个节点开始匹配
        p = p ? (*p)[s[i]] : root;
        
        //遍历完所有的失败节点，并且记录 所有找到的字符串
        for (TrieNode* tmp = p; tmp; tmp = tmp->fail) {
            if (tmp->isWord) {
                ret.insert(tmp->word);
            }
        }
    }
}
