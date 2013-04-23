//
//  main.cpp
//  ParallelControllableTextureSynthesis
//
//  Created by Rosani Lin on 13/4/12.
//  Copyright (c) 2013年 Rosani Lin. All rights reserved.
//

#include <iostream>
#include "ParallelControllableTextureSynthesis.h"


int main(int argc, const char * argv[])
{
    ParallelControllableTextureSynthesis textsyn = ParallelControllableTextureSynthesis();
    
    
    textsyn.synthesis("/Users/xup6qup3/Google Drive/code/Dev.temp/ParallelControllableTextureSynthesis/ParallelControllableTextureSynthesis/tx.jpg", 4.0);



    return 0;
}

