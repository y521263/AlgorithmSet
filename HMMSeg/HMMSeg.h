//
//  HMMSeg.h
//  test
//
//  Created by lin on 16/3/29.
//  Copyright © 2016年 lin. All rights reserved.
//

#ifndef HMMSeg_hpp
#define HMMSeg_hpp

#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <cmath>

#include "Strs.h"

using namespace std;

typedef unordered_map<uint32_t, double> Emit_map;

class HMMSeg{

public:
    HMMSeg(std::string train_file_path);
    
    void Cut(std::string strWords,vector<string>& vecWords);
    
    enum {B = 0, M, E, S, NUM};
private:
    
    void LoadTrainFile(std::string train_file_path);
    
    double GetEmitProb(Emit_map& Emit_map,uint32_t& word);
    
    void Viterbe(std::vector<uint32_t>& words, std::vector<size_t>& ret);
    
    Emit_map Emit_map_B;
    Emit_map Emit_map_M;
    Emit_map Emit_map_E;
    Emit_map Emit_map_S;
    
    //发射概率矩阵
    std::vector<Emit_map*> vec_Emit_map;
    
    //初始状态集合
    std::vector<double> Init_Status;
    
    //状态转移矩阵
    std::vector< std::vector<double> > TransProbMatrix;
    
};
#endif /* HMMSeg_hpp */
