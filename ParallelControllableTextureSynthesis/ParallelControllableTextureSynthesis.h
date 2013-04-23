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
    

    
    
private:
    
    // Initialize arrays including synthesized pyramid and coordinate pyramid
    void initialization (double magnify_ratio);
    
    // Upsampling
    void upsample (int level);
    
    // Basic jitter
    void jitter (int level);
    
    // Basic correction
    void correction (int level);
    
    // Maps coordinate to synthesized texture
    void coordinateMapping (int level);
    
    // Cut off the coordinates outside of texture boundary
    void coordinateTrim (Point& coor);
    
    // Construct similar set of given input sample texture
    void similarSetConstruction ();
    
    Mat sample_texture;
    Mat synthesized_texture;
    vector<dynamicArray2D<Point> > syn_coor;
    vector<Mat> syn_texture;
    dynamicArray2D<vector<Point> > sample_similar_set;
    
    string sample_texture_path;
    
    static const int    PYRAMID_LEVEL       =   6;
    static const int    OUTSPACE_FACTOR     =   1;
    static const int    JITTER_AMPLITUDE    =   1;
    static const int    PATCH_WIDTH         =   2;
    static const int    COHERENCE_SEARCH_W  =   5;
    static const int    SIMILAR_NEIGHBOR_N  =  10;
    
    
    
    
};



#endif /* defined(__ParallelControllableTextureSynthesis__ParallelControllableTextureSynthesis__) */
