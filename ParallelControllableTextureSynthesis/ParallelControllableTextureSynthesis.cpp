//
//  ParallelControllableTextureSynthesis.cpp
//  ParallelControllableTextureSynthesis
//
//  Created by Rosani Lin on 13/4/12.
//  Copyright (c) 2013年 Rosani Lin. All rights reserved.
//

#include "ParallelControllableTextureSynthesis.h"


ParallelControllableTextureSynthesis::ParallelControllableTextureSynthesis () {
    
    srand((int)time(NULL));
    
}

ParallelControllableTextureSynthesis::~ParallelControllableTextureSynthesis() {
    
}


Mat ParallelControllableTextureSynthesis::synthesis(const string &texture_file, double magnify_ratio) {
    
    sample_texture = imread(texture_file.c_str());
    sample_texture_path = texture_file;
    
    similarSetConstruction();
//
//    initialization(magnify_ratio);
//    
//    for (int i = 1; i <= PYRAMID_LEVEL; i ++) {
//        
//        upsample(i);
//        jitter(i);
//        coordinateMapping(i);
//        showMat(syn_texture[i]);
//        
//        correction(i);
//        coordinateMapping(i);
//        showMat(syn_texture[i]);
//        
//    }
    
    return synthesized_texture;
}



void ParallelControllableTextureSynthesis::initialization(double magnify_ratio) {
    
    buildPyramid(Mat(sample_texture.rows*magnify_ratio, sample_texture.cols*magnify_ratio, CV_8UC3), syn_texture, PYRAMID_LEVEL);
    
    reverse(syn_texture.begin(), syn_texture.end());
    
    for_each(syn_texture.begin(), syn_texture.end(), [&](Mat texture){
        
        dynamicArray2D<Point> local_coor (texture.rows, texture.cols);
        syn_coor.push_back(local_coor);
        
    });
    
    
    // Regular initialization
    syn_coor[0].forEach_withCorr([=](Point& coor, int i, int j) {
        
        coor = Point(j, i);
 
    });

    
    
    for (int i = 0; i < syn_coor.size(); i ++) {
        cout<<syn_coor[i].rows<<", "<<syn_texture[i].rows<< endl;
    }
    
    
}


void ParallelControllableTextureSynthesis::upsample(int level) {
    
//    for (int i = 0; i < syn_coor[level - 1].rows; i ++) {
//        for (int j = 0; j < syn_coor[level - 1].cols; j ++) {
//            
//            syn_coor[level].at(i*2    , j*2    ) = syn_coor[level - 1].at(i, j)*2;
//            syn_coor[level].at(i*2 + 1, j*2    ) = syn_coor[level - 1].at(i, j)*2 + Point(1, 0);
//            syn_coor[level].at(i*2    , j*2 + 1) = syn_coor[level - 1].at(i, j)*2 + Point(0, 1);
//            syn_coor[level].at(i*2 + 1, j*2 + 1) = syn_coor[level - 1].at(i, j)*2 + Point(0, 0);
//            
//        }
//    }

    syn_coor[level].forEach_withCorr([&](Point& texture, int i, int j) {
        
        texture = syn_coor[level - 1].at(i/2, j/2)*2 + Point(j%2, i%2);
    
        coordinateTrim(texture);
        
        
    });
    
    
    
    
}


void ParallelControllableTextureSynthesis::jitter (int level) {
    
    syn_coor[level].forEach([=](Point& coor) {
        
        coor = coor + Point(ceil((rand()%3 - 1) + 0.5), ceil((rand()%3 - 1) + 0.5))*JITTER_AMPLITUDE;
        
        coordinateTrim(coor);
        
    });
    
}


void ParallelControllableTextureSynthesis::correction(int level) {
    
    
    double local_shrink_ratio = (double)syn_texture[level].rows/sample_texture.rows;
//    Mat re_texture = sample_texture.clone();
//    if ( syn_coor[level].rows < sample_texture.rows ) {
//        resize(sample_texture, re_texture, syn_texture[level].size());
//    }
//    
//    dynamicArray2D<Point> temp_coor(syn_coor[level].rows, syn_coor[level].cols);
//    
//    for (int i = 0; i < syn_texture[level].rows; i ++) {
//        for (int j = 0; j < syn_texture[level].cols; j ++) {
//            
//
//            double min_cost = INFINITY;
//            Point min_loc(0);
//            
//            for (int m = -COHERENCE_SEARCH_W; m <= COHERENCE_SEARCH_W; m ++) {
//                for (int n = -COHERENCE_SEARCH_W; n <= COHERENCE_SEARCH_W; n ++) {
//                    
//                    double local_cost = 0;
//                    int valid_count = 0;
//                    
//                    for (int p = -PATCH_WIDTH; p <= PATCH_WIDTH; p ++) {
//                        for (int q = -PATCH_WIDTH; q <= PATCH_WIDTH; q ++) {
//                            
//                            if ( (syn_coor[level].at(i, j).y + m + p) >= 0 && (syn_coor[level].at(i, j).y + m + p) < syn_texture[level].rows
//                                && (syn_coor[level].at(i, j).x + n + q) >= 0 && (syn_coor[level].at(i, j).x + n + q) < syn_texture[level].cols
//                                && (i + p) >= 0 && (i + p) < syn_texture[level].rows
//                                && (j + q) >= 0 && (j + q) < syn_texture[level].cols ) {
//                                
//                                local_cost += Vec3bDiff(syn_texture[level].at<Vec3b>(i + p, j + q), re_texture.at<Vec3b>(syn_coor[level].at(i, j).y + m + p, syn_coor[level].at(i, j).x + n + q));
//                                valid_count ++;
//                                
//                            }
//                            
//                        }
//                    }
//                    
//                    local_cost /= valid_count;
//                    if ( local_cost < min_cost ) {
//                        min_cost = local_cost;
//                        min_loc = Point(syn_coor[level].at(i, j).x + n, syn_coor[level].at(i, j).y + m);
//                    }
//                    
//                }
//            }
//            
//            temp_coor.at(i, j) = min_loc;
//            
//        }
//    }
//    
//    
//    syn_coor[level] = temp_coor;
    
    
}


void ParallelControllableTextureSynthesis::coordinateMapping(int level) {
    
    
    Mat re_texture = sample_texture.clone();
    if ( syn_coor[level].rows < sample_texture.rows ) {
        resize(sample_texture, re_texture, syn_texture[level].size());
    }
    
    for (int i = 0; i < syn_texture[level].rows; i ++) {
        for (int j = 0; j < syn_texture[level].cols; j ++) {
            
            syn_texture[level].at<Vec3b>(i, j) = re_texture.at<Vec3b>(syn_coor[level].at(i, j));
            
        }
    }
    
    
    
}


void ParallelControllableTextureSynthesis::coordinateTrim(Point &coor) {
    
    coor = Point(coor.x % sample_texture.rows, coor.y % sample_texture.cols);
    
}



void ParallelControllableTextureSynthesis::similarSetConstruction() {
    
    sample_similar_set = dynamicArray2D<vector<Point> > (sample_texture.rows, sample_texture.cols);
    
    string file_path = sample_texture_path.substr(0, sample_texture_path.find_last_of("/") + 1);
    string file_name = sample_texture_path.substr(sample_texture_path.rfind("/") + 1).substr(0, sample_texture_path.find_last_of(".") - file_path.size());

    
    ifstream read_file(file_path.append(file_name).append(".txt").c_str());
    


    if ( read_file.fail() ) {
        
        for (int i = 0; i < sample_texture.rows; i ++) {
            for (int j = 0; j < sample_texture.cols; j ++) {
                
                multimap<double, Point> local_similar_set;
                
                for (int p = 0; p < sample_texture.rows; p ++) {
                    for (int q = 0 ; q < sample_texture.cols; q ++) {
                        
                        double local_cost = 0.0;
                        int valid_count = 0;
                        
                        for (int m = -PATCH_WIDTH; m <= PATCH_WIDTH; m ++) {
                            for (int n = -PATCH_WIDTH; n <= PATCH_WIDTH; n ++) {
                                
                                if ( (i + m) >= 0 && (i + m) < sample_texture.rows
                                    && (j + n) >= 0 && (j + n) < sample_texture.cols
                                    && (p + m) >= 0 && (p + m) < sample_texture.rows
                                    && (q + n) >= 0 && (q + n) < sample_texture.cols ) {
                                    
                                    local_cost += Vec3bDiff(sample_texture.at<Vec3b>(i + m, j + n), sample_texture.at<Vec3b>(p + m, q + n));
                                    
                                    valid_count ++;
                                    
                                }
                                
                            }
                        }
                        
                        local_similar_set.insert(pair<double, Point> (local_cost/valid_count, Point(q, p)));
                        
                    }
                }
                
                
                int temp_counter = 0;
                
                for_each(local_similar_set.begin(), local_similar_set.end(), [&](pair<double, Point> candidate) {
                    
                    temp_counter ++;
                    if ( temp_counter <= SIMILAR_NEIGHBOR_N ) {
                        sample_similar_set.at(i, j).push_back(candidate.second);
                    }
                    
                });
                
                
                local_similar_set.clear();
            }
        }

        
        ofstream write_file(file_path.c_str());
        
        write_file << SIMILAR_NEIGHBOR_N;
        
        for (int i = 0; i < sample_similar_set.rows; i ++) {
            for (int j = 0; j < sample_similar_set.cols; j ++) {
                
                write_file << endl;
                
                write_file << i <<" "<< j <<" ";
                
                for (int p = 0; p < sample_similar_set.at(i, j).size(); p ++) {
                    write_file << sample_similar_set.at(i, j)[p].x<<" "<< sample_similar_set.at(i, j)[p].y<<" ";
                }
                
                
            }
        }
        
    }
    
    else {
        
        if (!read_file.eof()) {
            
            int similar_num = 0;
            read_file >> similar_num;
            
            int i = 0, j = 0, nx = 0, ny = 0;
            
            while ( read_file >> i && read_file >> j ) {
                
                for (int k = 0; k < similar_num; k ++) {
                    
                    read_file >> nx; read_file >> ny;
                    
                    sample_similar_set.at(i, j).push_back(Point(nx, ny));
                    
                }
                
            }
        }
        
        
        
        
    }
    
    
    
//    for (int i = 0; i < sample_similar_set.rows; i ++) {
//        for (int j = 0; j < sample_similar_set.cols; j ++) {
//            
//            for (int p = 0; p < sample_similar_set.at(i, j).size(); p ++) {
//                cout<<sample_similar_set.at(i, j)[p]<<", ";
//            }
//            
//            cout<<endl;
//            
//        }
//    }
    
    

    
    
}












