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

int svd_sift(Mat img){
    Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
    vector<KeyPoint> keypoints_1;
    Mat descriptors_1, img_keypoints_1;
    Mat dataMatrix (0, 128, CV_32F);
    
    f2d->detect(img, keypoints_1); //Step 1 : Detect Keypoints
    f2d->compute(img, keypoints_1, descriptors_1); //Step 2 : Calculate descriptors (feature vectors)
    
    drawKeypoints(img, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT); //Draw Keypoints
    int kp = keypoints_1.size();
    
    dataMatrix.push_back(descriptors_1);
    int rows = dataMatrix.rows; 
    int cols = dataMatrix.cols; 
    
    // SVD on dataMatrix
    Mat s,u,v;
    SVD::compute(dataMatrix, s,u,v);
    
//    int rank = 0;
//    int nbEigenValue = min(rows, cols);
//    
//    for (int i =0; i< nbEigenValue; i++)
//        if(s.at<float>(i) !=0){
//            rank ++;
//            //cout << "rank: " << rank << endl;
//        }
//    
    
    // Print out results
    //cout << "Keypoints: " << kp << endl;
    //cout << "Data Matrix:" << dataMatrix << endl;
    //cout << "Singular Array:" << s << endl;
    
    cout << "Rows:  " << rows << endl;
    //cout << "Cols:  " << cols << endl;
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
    
    /* START IMAGE */ 
    /* Ctr + Shift + C : comment */
    /*
    Mat img_1 = imread("vlcsnap-00024.png");
    imshow("img1", img_1);
    
    
    int N = 32; // assume size of block varies from 16x16 to 64x64 (same as LCU in HEVC)
    
    //draw a grid line
    draw_grid(img_1, N);   
    int blockNo = 0;
    
    // detect & count number of keypoints in each block9
    Mat tile;
    for(int r =0; r< img_1.rows; r+=N)
        
        for(int c=0; c<img_1.cols; c+=N)
        {
            
            tile = img_1(Range(r, min(r+N, img_1.rows)), Range(c,min(c+N, img_1.cols)));
            //detect and count number of key points in each block
            blockNo++;
            cout << "Block: [" << blockNo << "] -------------- " << endl;
            int kp = svd_sift(tile);
            draw_text(tile, kp);
            
        }
    imshow("Grid Image", img_1);
    */
    /* END IMAGE */
    
    /* START VIDEO */
    VideoCapture capture ("/home/larry/work/thu/videos/BasketballDrill.avi");
    
    
    Mat frame;
    int frameNumber = 0;
    
    while (1){
        
        capture >> frame;
        if (frame.empty())
        {
            cout << "image not load" ;
        }
        else
        {
            
            namedWindow("Video", CV_WINDOW_AUTOSIZE);
            imshow("Video", frame);
            frameNumber ++;
        }
                
        cout << "Frame [" << frameNumber << "]. " ;
        int kp = svd_sift(frame);
        
    }
    
    /* END VIDEO */
    waitKey(0);
    capture.release();
    return 0;
}