//
//  ParallelControllableTextureSynthesis.cpp
//  ParallelControllableTextureSynthesis
//
//  Created by Rosani Lin on 13/4/12.
//  Copyright (c) 2013年 Rosani Lin. All rights reserved.
//

#include "ParallelControllableTextureSynthesis.h"


ParallelControllableTextureSynthesis::ParallelControllableTextureSynthesis () {
    
    patch = patchTools(2*PATCH_WIDTH + 1, 2*PATCH_WIDTH + 1);
    
    srand((int)time(NULL));
    
}

ParallelControllableTextureSynthesis::~ParallelControllableTextureSynthesis() {
    
}


Mat ParallelControllableTextureSynthesis::guidedSynthesis(const string &texture, const string &guide) {
    
    guide_img = imread(guide.c_str());
    sample_texture = imread(texture.c_str());
    
    initialization();
    
    for (int i = 1; i < PYRAMID_LEVEL; i ++) {
        
        upsample(i);
        
        coordinateMapping(i);
        showMat(syn_texture[i], "ORI", 1);
        
        pull(i);
        
        coordinateMapping(i);
        showMat(syn_texture[i], "PULL", 1);
        
        
        for (int i = 0; i < 8; i ++) {
            cout<<i<<", ";
            correction(i);
        } cout<<endl;
        
        coordinateMapping(i);
        showMat(syn_texture[i], "COR", 0);
        
        
        
    }
    
    
    return guide_img;
}


void ParallelControllableTextureSynthesis::pull(int level) {
    
    double alpha = 0.5;
    
    for (int i = 0; i < syn_texture[level].rows; i ++) {
        for (int j = 0; j < syn_texture[level].cols; j ++) {
            
            double min_cost = INFINITY, local_cost = 0;
            Point min_loc(0);
            
            for (int m = -COHERENCE_SEARCH_W; m <= COHERENCE_SEARCH_W; m ++) {
                for (int n = -COHERENCE_SEARCH_W; n <= COHERENCE_SEARCH_W; n ++) {
                    
                    Point target = syn_coor[level].at(i, j) + Point(n, m);
                    target = Point(target.x % sample_pyr[level].cols, target.y % sample_pyr[level].rows);
                    
                    
                    local_cost = alpha*patch.SSD(guide_pyr[level], Point(j, i), sample_pyr[level], target) + 0.35*patch.SSD(syn_texture[level], Point(j, i), sample_pyr[level], target);
                    
                    if ( local_cost < min_cost ) {
                        
                        min_cost = local_cost;
                        min_loc = target;
                    }
                    
                }
                
            }
            
//            cout<<syn_coor[level].at(i, j)<<", "<<min_loc;
            syn_coor[level].at(i, j) = min_loc;
            
        }
    }
    
    
    
    
}



void ParallelControllableTextureSynthesis::initialization() {
    
    buildPyramid(Mat(guide_img.rows, guide_img.cols, CV_8UC3), syn_texture, PYRAMID_LEVEL);
    
    reverse(syn_texture.begin(), syn_texture.end());
    
    for_each(syn_texture.begin(), syn_texture.end(), [&](Mat texture){
        
        dynamicArray2D<Point> local_coor (texture.rows, texture.cols);
        syn_coor.push_back(local_coor);
        
        // Initialized pyramid of sample texture
        Mat temp_sample = sample_texture.clone();
        
        if ( temp_sample.rows > texture.rows ) {
            resize(sample_texture, temp_sample, texture.size());
        }
        
        sample_pyr.push_back(temp_sample);
        
        
        // Initialized pyramid of guide image
        Mat temp_guide;
        resize(guide_img, temp_guide, texture.size());
        
        guide_pyr.push_back(temp_guide);
        
        
    });
    
    
    // Regular initialization
    syn_coor[0].forEach_withCorr([=](Point& coor, int i, int j) {
        
        coor = Point(j, i);
 
    });
    
    
    
}


void ParallelControllableTextureSynthesis::upsample(int level) {
    
    syn_coor[level].forEach_withCorr([&](Point &coor, int i, int j) {
        
        coor = 2*syn_coor[level - 1].at(i/2, j/2) + Point(j%2, i%2);
        
        coor = Point(coor.x % sample_pyr[level].cols, coor.y % sample_pyr[level].rows);
    });
    
    
}


void ParallelControllableTextureSynthesis::jitter (int level) {
    
    syn_coor[level].forEach([=](Point& coor) {
        
        coor = coor + Point(rand()%3 - 1, rand()%3 - 1);//Point(floor((rand()%3 - 1) + 0.5), floor((rand()%3 - 1) + 0.5))*JITTER_AMPLITUDE;
        
        // trim coordinate from breaking the boundary
        coor = Point((coor.x + sample_pyr[level].cols)%sample_pyr[level].cols, (coor.y + sample_pyr[level].rows) % sample_pyr[level].rows);
        
    });
    
}


void ParallelControllableTextureSynthesis::correction (int level) {
    
//    Mat resizedSample = sample_pyr[level].clone();
    
    
    for (int pass = 0; pass < 4; pass ++) {
        
        for (int i = pass/2; i < syn_texture[level].rows; i += 2) {
            for (int j = pass%2; j < syn_texture[level].cols; j += 2) {
                
                double local_cost = 0, min_cost = INFINITY;
                Point min_loc(0);
            
                for (int m = -COHERENCE_SEARCH_W; m <= COHERENCE_SEARCH_W; m ++) {
                    for (int n = -COHERENCE_SEARCH_W; n <= COHERENCE_SEARCH_W; n ++) {
                        
                        Point target = syn_coor[level].at(i, j) + Point(n, m);
                        target = Point(target.x % sample_pyr[level].cols, target.y % sample_pyr[level].rows);
                        
                        
                        local_cost = patch.SSD(syn_texture[level], Point(j, i), sample_pyr[level], target);
                        
                        if ( local_cost < min_cost ) {
                            
                            min_cost = local_cost;
                            min_loc = target;
                        }
                        
                    }

                }
                
//                for (int m = -1; m <= 1; m ++) {
//                    for (int n = -1; n <= 1; n ++) {
//                        
//                        Point target = syn_coor[level].at(i, j) + Point(n, m);
//                        target = Point(target.x % resizedSample.cols, target.y % resizedSample.rows);
//                        
//                        
//                        local_cost = patch.SSD(syn_texture[level], Point(j, i), resizedSample, target);
//                        
//                        if ( local_cost < min_cost ) {
//                            
//                            min_cost = local_cost;
//                            min_loc = target;
//                        }
//                        
//                    }
//                }
                
                
                
                syn_coor[level].at(i, j) = min_loc;
            }
        }
        
    }
    

    
    
}




void ParallelControllableTextureSynthesis::coordinateMapping(int level) {
    
    
    
    
    for (int i = 0; i < syn_texture[level].rows; i ++) {
        for (int j = 0; j < syn_texture[level].cols; j ++) {        
            
            syn_texture[level].at<Vec3b>(i, j) = sample_pyr[level].at<Vec3b>(syn_coor[level].at(i, j));
            
        }
    }
    
    
    
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


void ParallelControllableTextureSynthesis::showCoordinate(dynamicArray2D<Point> &coorMap, Mat& sample) {
    
    Mat temp_show;
    
    if ( sample.rows > sample_texture.rows ) {
        temp_show = sample_texture.clone();
    }
    
    else {
        temp_show = sample.clone();
    }
    
    Mat temp = Mat(coorMap.rows, coorMap.cols, CV_8UC3);
    
    for (int i = 0; i < temp.rows; i ++) {
        for (int j = 0; j < temp.cols; j ++) {
            
            temp.at<Vec3b>(i, j) = Vec3b(0, (temp.rows - 1 - coorMap.at(i, j).y)%temp_show.rows*(255.0/temp_show.rows),  coorMap.at(i, j).x%temp_show.cols*(255.0/temp_show.cols));
            
            
        }
    }
    
    resizeMat(temp, 3.0);
    showMat(temp);
}










