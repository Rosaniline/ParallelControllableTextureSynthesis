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
    
//    ifstream ifile("/Users/xup6qup3/Google Drive/code/Dev.temp/ParallelControllableTextureSynthesis/ParallelControllableTextureSynthesis/tx.txt");
//    
//    if ( !ifile.eof() ) {
//        
////        int i = 0;
//        string s;
//        getline(ifile, s);
//        cout<<s<<endl;
//        
//        while ( !ifile.eof() ) {
//            
//            int i = 0, j = 0, nx = 0, ny = 0;
//            ifile >> i; ifile >> j;
//            
//            cout<<"("<<i<<", "<<j<<")  ";
//            
//            for (int i = 0; i < 10; i ++) {
//                
//                ifile >> nx; ifile >> ny;
//                
//                cout<<nx<<", "<<ny<<" : ";
//                
//            }
//            
//            cout<<endl;
//        }
//        
//    }

    



    return 0;
}

