//#include "b39vt_assignment/image_processing.hpp"
#include <iostream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;

//updateNumOfSpins (int numOfSpins)
// {numOfSpins == numOfSpins++;return 0;}
  
  //personly chosen variables
  double MinMatchingValue = 10E8;  
 


void templateMatching(const cv::Mat& im, const cv::Mat& templ,const std::vector<std::string> templates,int y,int x,int numOfSpins,double average)
{
  int match_method=CV_TM_SQDIFF;

  cv::Mat result;
  cv::Mat edit_copy;
  
  im.copyTo(edit_copy);
   
	/// Create the result matrix
  int result_cols = im.cols - templ.cols + 1;
  int result_rows = im.rows - templ.rows + 1;

  result.create( result_rows, result_cols, CV_32FC1 );

  /// Do the Matching and Normalize
  cv::matchTemplate( im, templ, result, match_method );
  //cv::normalize( result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
  cv::Point matchLoc;

  cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
 

  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc;//cout << "the minimum value of resemblance is: " << minVal<< "\n"<<"\n" <<endl ;cout << "we re checking with template " << templates[y] << "\n" <<endl;
    }
  else
    { matchLoc = maxLoc;}

    

  /// Show me what you got
  
  if (minVal <= MinMatchingValue){
  cv::rectangle( edit_copy, matchLoc, cv::Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ),     cv::Scalar::all(0), 2, 8, 0 );
  
  
  
  //If there's a match we will give the value.
  cout << "the minimum value of resemblance is: " << minVal  <<endl ;
  cout << "we have a match with template " << templates[y] << "\n" <<endl;
  
  }
  cv::rectangle(result, matchLoc, cv::Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), cv::Scalar::all(0), 2, 8, 0 );
  
  //display results visualy (hevy on the pc processor)
  
	//cv::imshow("OPENCV_WINDOW", edit_copy);
  cv::waitKey(5);
  
  //int getMinVal()
  //{return minVal;}
  
  //Average values a diffrent ranges to help determine values of minVal
  
//if( x==200)
  //{  int updateNumOfSpins(int numOfSpins);
    
  // while (wait == false){
  //  cv::waitKey(3); wait == true;}
  // cout << "the average value on minVal at range:   "<< x  <<"  =  " << average << "\n" <<endl ;
   //cout << "the numOfSpins  " << numOfSpins << "    and the minVal  " << minVal <<"\n" <<endl;
 // }
  
}
