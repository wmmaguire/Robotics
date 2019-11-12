#include "max_robotics.h"
#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui/highgui.hpp>
/*
 * test_lib.cpp
 * Test script for max robotics library
 */
cv::Mat bg(3500,3500, CV_8UC3, cv::Scalar(255,255,255));
cv::Point2i cv_offset(
  double val, int image_width=2000, int image_height=2000){
  cv::Point2i output;
  output.x = int(val * 100) + image_width/2;
  output.y = image_height - int(val * 100) - image_height/3;
  return output;
};


int main() {
  // set variables
  int line;
  std::string file_name;
  std::vector<double> vec;
  std::vector<std::vector<double> > array;
  
  double val;

  std::cout << "Enter a file name" << std::endl;
  std::getline(std::cin, file_name);
	
  std::cout << "Enter a line number" << std::endl;
  std::cin >> line;
  std::cout.flush();
  // retrieve file contents
  mr::fileio::load_vec_array(file_name, array);
  std::cout << file_name << " dims: [ " << array.size() << " , " << array[0].size() << " ]\n";
  //mr::fileio::load_vec_row(file_name, vec, line);
  mr::fileio::load_vec_col(file_name, vec, line);
  // exit if error reading
  if (vec.empty()) return 0;
  // process data for visualization
  cv::namedWindow("test_lib", cv::WINDOW_NORMAL);
  std::cout << "index, value" << std::endl;
  for (int i = 0; i < vec.size(); i++) {
    val = vec.at(i);
    std::cout << i << "    , " << val << " -->";
    if (mr::NearZero(val)) {
      val = 0.;
    }
    // draw circle
    cv::circle(bg, cv_offset(val, bg.cols, bg.rows),
                7, cv::Scalar(0, 0, 255), -1);
    std::cout << val << std::endl;
  
    cv::imshow("test_lib", bg);
    cv::waitKey(0);
  }
  //initialize a 120X350 matrix of black pixels:
  cv::namedWindow("test_lib2", cv::WINDOW_NORMAL);
  cv::Mat output = cv::Mat::zeros( 120, 350, CV_8UC3 ); 
  //write text on the matrix:
  putText(output,
            "Hello World :)",
            cvPoint(15,70),
            cv::FONT_HERSHEY_PLAIN,
            3,
            cvScalar(0,255,0),
            4);

  cv::imshow("test_lib2", output);
  cv::waitKey(0);
  return 1;
}
