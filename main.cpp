/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: larry
 *
 * Created on February 18, 2019, 2:24 PM
 */

#include <cstdlib>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

/*
 * 
 */
int detect_keypoints(Mat img){
    Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
    vector<KeyPoint> keypoints_1;
    Mat descriptors_1, img_keypoints_1;
    f2d->detect(img, keypoints_1); //Step 1 : Detect Keypoints
    f2d->compute(img, keypoints_1, descriptors_1); //Step 2 : Calculate descriptors (feature vectors)
    drawKeypoints(img, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT); //Draw Keypoints
    int kp = keypoints_1.size();
    cout << kp << endl;
    return kp;
}
void draw_text(Mat img, int kp){
    string text;
    stringstream ss;
    ss << kp;
    text = ss.str();
    putText(img, text, Point2f(0,10), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255), 2);
}
void draw_grid(Mat img, int N){
    int width = img.size().width;
    int height = img.size().height;
    for(int i = 0; i < width; i+=N)
        line(img, Point(i,0), Point(i,height), Scalar(0,255,255));
    for(int i = 0; i < height; i+=N)
        line(img, Point(0,i), Point(width,i), Scalar(0,255,255));
}
int main(int argc, char** argv) {
    
    Mat img_1 = imread("vlcsnap-00024.png");
    imshow("img1", img_1);
    int N = 32; // assume size of block varies from 16x16 to 64x64 (same as LCU in HEVC)
    
    //draw a grid line
    draw_grid(img_1, N);   
    
    // detect & count number of keypoints in each block9
    Mat tile;
    for(int r =0; r< img_1.rows; r+=N)
        
        for(int c=0; c<img_1.cols; c+=N)
        {
            
            tile = img_1(Range(r, min(r+N, img_1.rows)), Range(c,min(c+N, img_1.cols)));
            //detect and count number of key points in each block
            int kp = detect_keypoints(tile);
            draw_text(tile, kp);
            
        }
    imshow("Grid Image", img_1);
    waitKey(0);
    return 0;
}