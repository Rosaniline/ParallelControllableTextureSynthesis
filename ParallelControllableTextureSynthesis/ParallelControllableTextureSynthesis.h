//
//  ParallelControllableTextureSynthesis.h
//  ParallelControllableTextureSynthesis
//
//  Created by Rosani Lin on 13/4/12.
//  Copyright (c) 2013年 Rosani Lin. All rights reserved.
//

#ifndef __ParallelControllableTextureSynthesis__ParallelControllableTextureSynthesis__
#define __ParallelControllableTextureSynthesis__ParallelControllableTextureSynthesis__

#include <iostream>
#include <fstream>
#include "RosaniTools.h"

using namespace std;
using namespace cv;

class ParallelControllableTextureSynthesis {
    
public:
    
    // Default constructor
    ParallelControllableTextureSynthesis();
    
    // Default destructor
    ~ParallelControllableTextureSynthesis();
    
    // Begin synthesizing
    cv::Mat synthesis (const string& texture_file, double magnify_ratio);
    
    
    Mat guidedSynthesis (const string& texture, const string& guide);
    

    
    
private:
    
    // Initialize arrays including synthesized pyramid and coordinate pyramid
    void initialization ();
    
    // Upsampling
    void upsample (int level);
    
    // Basic jitter
    void jitter (int level);
    
    // Pull
    void pull (int level);
    
    // Basic correction
    void correction (int level);
    
    // Maps coordinate to synthesized texture
    void coordinateMapping (int level);
    
    // Construct similar set of given input sample texture
    void similarSetConstruction ();
    
    
    
    void showCoordinate(dynamicArray2D<Point> &coorMap, Mat& sample);
    
    Mat sample_texture, synthesized_texture, guide_img;
    
    vector<dynamicArray2D<Point> > syn_coor;
    vector<Mat> syn_texture, sample_pyr, guide_pyr;
    dynamicArray2D<vector<Point> > sample_similar_set;
    
    string sample_texture_path;
    
    patchTools patch;
    
    static const int    PYRAMID_LEVEL       =   5;
    static const int    OUTSPACE_FACTOR     =   1;
    static const int    JITTER_AMPLITUDE    =   1;
    static const int    PATCH_WIDTH         =   2;
    static const int    COHERENCE_SEARCH_W  =   1;
    static const int    SIMILAR_NEIGHBOR_N  =  10;
    
    
    
    
};



#endif /* defined(__ParallelControllableTextureSynthesis__ParallelControllableTextureSynthesis__) */
