#include "opencv2/features2d/features2d.hpp"
#include "b39vt_assignment/image_processing.hpp"

using namespace cv;

bool templateMatching(const cv::Mat& im, const cv::Mat& templ, double thresh)
{

	int match_method = 5;

	char* image_window = "image window";
//	char* result_window = "result window";
	char* template_window = "template";


	cv::namedWindow( image_window, CV_WINDOW_AUTOSIZE );
//  cv::namedWindow( result_window, CV_WINDOW_AUTOSIZE );
	cv::namedWindow( template_window, CV_WINDOW_AUTOSIZE );

	
	cv::Mat result;
	/// Source image to display
  cv::Mat img_display;
  im.copyTo( img_display );
	double sizes [3] = {0.75, 1, 1.25};
	cv::imshow( template_window, templ);

for (double a = 0.6; a < 1.5; a = a + 0.1){

	cv::Mat utempl;

	cv::resize(templ, utempl, cv::Size(), a, a);
  /// Create the result matrix
  int result_cols =  im.cols - utempl.cols + 1;
  int result_rows = im.rows - utempl.rows + 1;

  result.create( result_rows, result_cols, CV_32FC1 );
  /// Do the Matching and Normalize
  cv::matchTemplate( im, utempl, result, match_method );
//  cv::normalize( result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
  cv::Point matchLoc;

  cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
	
	

  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { ROS_INFO_STREAM ("minVal " << minVal);
    	matchLoc = minLoc; }
  else
    { ROS_INFO_STREAM ("maxVal " << maxVal);
    matchLoc = maxLoc; }



  /// Show me what you got



  /*-- Step 1: Detect the keypoints using SURF Detector
  int minHessian = 10;

  OrbFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_1, keypoints_2;

  detector.detect( im, keypoints_1 );
  detector.detect( templ, keypoints_2 );

  //-- Step 2: Calculate descriptors (feature vectors)
  OrbDescriptorExtractor extractor;

  Mat descriptors_1, descriptors_2;

  extractor.compute( im, keypoints_1, descriptors_1 );
  extractor.compute( templ, keypoints_2, descriptors_2 );

  //-- Step 3: Matching descriptor vectors with a brute force matcher
  BFMatcher matcher(NORM_L2);
  std::vector< DMatch > matches;
  matcher.match( descriptors_1, descriptors_2, matches );
  //-- Draw matches
  //Mat img_matches;

  drawMatches( im, keypoints_1, templ, keypoints_2, matches, img_display, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), 0 );
ROS_INFO_STREAM("display image post-process " << img_display.size());
 */
  if (maxVal >thresh){
  //ROS_INFO_STREAM ("maxVal " << maxVal);
  //ROS_INFO_STREAM("Thresh = " << thresh);
 	//ROS_INFO_STREAM("Found something!");
  cv::rectangle( img_display, matchLoc, cv::Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), cv::Scalar::all(0), 2, 8, 0 );
  //cv::rectangle( result, matchLoc, cv::Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), cv::Scalar::all(0), 2, 8, 0 );
  	  cv::imshow( image_window, img_display );

  //cv::imshow( template_window, templ);
	waitKey(20);
	 return true;
}

}

  //-- Show detected matches
  //imshow("Matches", img_matches );
	  cv::imshow( image_window, img_display );
//  cv::imshow( result_window, result );

 	waitKey(20); 
	return false;
}
