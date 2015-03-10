//
//  main.cpp
//  RobustTextDetection
//
//  Created by Saburo Okita on 05/06/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#include <iostream>
#include <iterator>     // std::istream_iterator


#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

#include "RobustTextDetection.h"
#include "ConnectedComponent.h"

using namespace std;
using namespace cv;


int main(int argc, const char * argv[])
{

    namedWindow( "" );
    moveWindow("", 0, 0);
    
    if ( argc <= 1 )
    {
        std::cout << "Input image missing !! "<< std::endl << "usage:  "<<  argv[0] <<" image.png"<< std::endl;
        return 0;
    }
    
    Mat image = imread( argv[1] );
    
    /* Quite a handful or params */
    RobustTextParam param;
    param.minMSERArea        = 10;
    param.maxMSERArea        = 2000;
    param.cannyThresh1       = 20;
    param.cannyThresh2       = 100;
    
    param.maxConnCompCount   = 3000;
    param.minConnCompArea    = 75;
    param.maxConnCompArea    = 600;
    
    param.minEccentricity    = 0.1;
    param.maxEccentricity    = 0.995;
    param.minSolidity        = 0.4;
    param.maxStdDevMeanRatio = 0.5;
    
    /* Apply Robust Text Detection */
    /* ... remove this temp output path if you don't want it to write temp image files */
    string temp_output_path = ".";
    RobustTextDetection detector(param, temp_output_path );
    pair<Mat, Rect> result = detector.apply( image );
    
    /* Get the region where the candidate text is */
    Mat stroke_width( result.second.height, result.second.width, CV_8UC1, Scalar(0) );
    Mat(result.first, result.second).copyTo( stroke_width);
    
    
    /* Use Tesseract to try to decipher our image */
    tesseract::TessBaseAPI tesseract_api;
    tesseract_api.Init(NULL, "eng"  );
    tesseract_api.SetImage((uchar*) stroke_width.data, stroke_width.cols, stroke_width.rows, 1, stroke_width.cols);
    
    string out = string(tesseract_api.GetUTF8Text());

    /* Split the string by whitespace */
    vector<string> splitted;
    istringstream iss( out );
    copy( istream_iterator<string>(iss), istream_iterator<string>(), back_inserter( splitted ) );
    
    /* And draw them on screen */ 
    //QtFont font = fontQt("Helvetica", 24.0, cv::Scalar(0, 0, 0) );
   
    Point coord = Point( result.second.br().x + 10, result.second.tl().y );
    for( string& line: splitted )
    {
      //   cv::addText( image, line, coord, font );
	cv::putText(image, line, coord, 1, 2, cv::Scalar(0, 0 , 255), 3 );
        coord.y += 25;
    }
    
    rectangle( image, result.second, Scalar(0, 0, 255), 2);
    
    /* Append the original and stroke width images together */
    cvtColor( stroke_width, stroke_width, COLOR_GRAY2BGR );
    Mat appended( image.rows, image.cols + stroke_width.cols, CV_8UC3 );
    image.copyTo( Mat(appended, Rect(0, 0, image.cols, image.rows)) );
    stroke_width.copyTo( Mat(appended, Rect(image.cols, 0, stroke_width.cols, stroke_width.rows)) );
    
    imshow("", appended );
    waitKey();
    
    return 0;
}
