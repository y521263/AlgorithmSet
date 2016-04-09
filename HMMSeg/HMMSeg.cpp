//
//  HMMSeg.cpp
//  test
//
//  Created by lin on 16/3/29.
//  Copyright © 2016年 lin. All rights reserved.
//

#include "HMMSeg.h"

HMMSeg::HMMSeg(std::string train_file_path)
{
    vec_Emit_map.push_back(&Emit_map_B);
    vec_Emit_map.push_back(&Emit_map_M);
    vec_Emit_map.push_back(&Emit_map_E);
    vec_Emit_map.push_back(&Emit_map_S);
    TransProbMatrix = std::vector< std::vector<double> >(4,vector<double>(4,0));
    LoadTrainFile(train_file_path);
}
void HMMSeg::Cut(std::string strWords,vector<string>& vecWords)
{
    std::vector<uint32_t> vec;
    Strs::Utf8ToUnicode32(strWords,vec);
    if(vec.size()<1) return;
    std::vector<size_t> ret;
    Viterbe(vec, ret);
    size_t left=0;
    size_t right=0;
    for (int i=0; i<ret.size(); i++) {
        if (ret[i]>=HMMSeg::E) {
            right = i+1;
            string tmp;
            Strs::Unicode32ToUtf8(vec.begin()+left, vec.begin()+right, tmp);
            vecWords.push_back(tmp);
            left = right;
        }
    }
}
//加载训练文件 计算HMM模型
void HMMSeg::LoadTrainFile(std::string train_file_path)
{
    ifstream in(train_file_path);
    if(!in.is_open())
    {
        cout<<"LoadTrainFile error.\n";
        return;
    }
    string line;
    int64_t B_count=0;
    int64_t M_count=0;
    int64_t E_count=0;
    int64_t S_count=0;
    int64_t last_inx=-1;
    std::vector< std::vector<int> > count = std::vector< std::vector<int> >(4,vector<int>(4,0));
    while (getline(in, line)) {
        std::vector< std::string > ret;
        Strs::split(line, "  ", ret);
        for(int i=1;i<ret.size();i++)
        {
            std::vector<uint32_t> words;
            Strs::Utf8ToUnicode32(ret[i], words);
            int len = words.size();
            if (len <1) continue;
            if (len == 1) {
                S_count++;
                if (last_inx != -1) {
                    count[last_inx][3]++;
                }
                last_inx=3;
            }else{
                B_count++;
                E_count++;
                if (len-2>0) {
                    M_count = M_count + len-2;
                    count[0][1]++;  //B M
                    count[1][2]++;  //M E
                    if (len-3>0) {
                        count[1][1]+= len-3;    //M M
                    }
                }else{
                    count[0][2]++;  //B E
                }
                if (last_inx != -1) {
                    count[last_inx][0]++;
                }
                last_inx=2;
            }
            
            //EmitProbMatrix
            if (len == 1) {
                Emit_map_S[words[0]]++;
            }else if (len>1)
            {
                Emit_map_B[words[0]]++;
                Emit_map_E[words[len-1]]++;
                for (int j=1; j<len-1; j++) {
                    Emit_map_M[words[j]]++;
                }
            }
        }
    }
    in.close();
    
    //Init_Status
    int64_t allcountword = E_count + S_count;
    Init_Status.push_back( std::log((double)E_count/allcountword));
    Init_Status.push_back(-INT64_MAX);
    Init_Status.push_back(-INT64_MAX);
    Init_Status.push_back( std::log((double)S_count/allcountword));
    
    //TransProbMatrix
    std::vector<int64_t> vec_BMES;
    vec_BMES.push_back(B_count);
    vec_BMES.push_back(M_count);
    vec_BMES.push_back(E_count);
    vec_BMES.push_back(S_count);
    
    for (int i=0; i<vec_BMES.size(); i++) {
        for (int j=0; j<vec_BMES.size(); j++) {
            TransProbMatrix[i][j] = std::log(((double)count[i][j])/vec_BMES[i]);
        }
    }
    //EmitProbMatrix
    for (auto it = Emit_map_B.begin(); it != Emit_map_B.end();it++) {
        it->second = std::log(it->second/B_count);
    }
    for (auto it = Emit_map_M.begin(); it != Emit_map_M.end();it++) {
        it->second = std::log(it->second/M_count);
    }
    for (auto it = Emit_map_E.begin(); it != Emit_map_E.end();it++) {
        it->second = std::log(it->second/E_count);
    }
    for (auto it = Emit_map_S.begin(); it != Emit_map_S.end();it++) {
        it->second = std::log(it->second/S_count);
    }
}
double HMMSeg::GetEmitProb(Emit_map& Emit_map,uint32_t& word)
{
    auto it = Emit_map.find(word);
    if ( it == Emit_map.end()) {
        return  -INT64_MAX;
    }
    return it->second;
}

//维特比算法
void HMMSeg::Viterbe(std::vector<uint32_t>& words, std::vector<size_t>& ret)
{
    std::vector< vector<double> > weight = std::vector< vector<double> >(4,vector<double>(words.size(),0));
    std::vector< vector<int> > path = std::vector< vector<int> >(4,vector<int>(words.size(),0));
    
    //计算第1个字的概率
    for (int i=0; i<4; i++) {
        weight[i][0] =Init_Status[i]+ GetEmitProb(*(vec_Emit_map[i]),words[0]);
    }
    
    for (int i=1; i<words.size(); i++) {
        for (int j=0; j<4; j++) {
            weight[j][i] = -INT64_MAX;
            path[j][i]=3;
            for (int k=0; k<4; k++) {
                //前面一个字的状态概率＋状态转移概率＋发射矩阵概率
                //因为取对数 所以 ＋
                double tmp = weight[k][i-1]+TransProbMatrix[k][j]+GetEmitProb(*(vec_Emit_map[j]),words[i]);
                if (tmp>weight[j][i]) {
                    weight[j][i]=tmp;
                    path[j][i]=k;
                }
            }
        }
    }
    
    //计算完整个weight 最后一个字要么是 E 要么 S，比较一下 就可以确定。
    double endE,endS;
    endE = weight[2][words.size()-1];
    endS = weight[3][words.size()-1];
    int inx=-1;
    if (endE>endS)
        inx = 2;
    else
        inx = 3;
    ret.resize(words.size());
    
    //利用最后一个字的状态来回溯整个状态路径，很巧妙的运用了path数组
    for (int i=words.size()-1; i>=0; i--) {
        ret[i]=inx;
        inx = path[inx][i];
    }
}

