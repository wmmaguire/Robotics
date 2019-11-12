#include "max_robotics.h"
#include <cstdio>
#include <math.h>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui/highgui.hpp>
/*
 * test_lib.cpp
 * Test script for max robotics library
 */

// visualization declarations
cv::Mat bg(3500,3500, CV_8UC3, cv::Scalar(255,255,255));
cv::Point2i cv_offset(
  double vals[2], int image_width=2000, int image_height=2000){
  cv::Point2i output;
  output.x = int(vals[0] * 100) + image_width/2;
  output.y = image_height - int(vals[1] * 100) - image_height/3;
  return output;
};

// find closest value to point
double * find_nearest_neighbor(double* point, std::vector<std::vector<double> > array) {
  // update static output
  static double nn[3];
  nn[0] = 0;
  nn[1] = 0;
  nn[2] = -1;
  //conduct search
  for (int r = 0; r < array.size(); r++) {
    double temp_distance = 0;
    temp_distance += pow(point[0] - array[r].at(0), 2);
    temp_distance += pow(point[1] - array[r].at(1), 2);
    // skip over point
    if (temp_distance == 0.) continue;
    // update nn if temp_distance
    if (nn[2] < 0 || temp_distance < nn[2]) {
      nn[0] = array[r].at(0);
      nn[1] = array[r].at(1);
      nn[2] = temp_distance;
    }
  }
  nn[2] = std::sqrt(nn[2]);
  return nn;
}


int main(int argc, char** argv) {
  // set variables
  std::string file_name;
  
  // parse for command line args
  if (argc > 1) {
    file_name = argv[1];
  } else {
    std::cout << "[ERROR] must provide command line args\n";
    std::cout << "\t-arg1: file path [string]\n";
    return 1;
  }

  // extract row from 
  std::vector<std::vector<double> > array;
  mr::fileio::load_vec_array(file_name, array);

  // exit if error reading array in file
  if (array.empty()){
    std::cout << "[ERROR] reading file: " << file_name << "\n";
    return 2;
  }
  
  // exit if array dimensions are wrong
  std::cout << "Array Dimensions: [ " << array.size() << " , " << array[0].size() << " ]\n";
  if (array[0].size() != 2) {
    std::cout << "[ERROR] array must be x by 2\n";
    return 3;
  }
  
  // process data for visualization
  cv::namedWindow("find_cluster", cv::WINDOW_NORMAL);
  double vals[2] = {0,0};
  for (int r = 0; r < array.size(); r++) {
    // draw circle
    vals[0] = array.at(r).at(0);
    vals[1] = array.at(r).at(1);
    cv::circle(bg, cv_offset(vals, bg.cols, bg.rows),
                7, cv::Scalar(0, 0, 255), -1);
    cv::imshow("find_cluster", bg);
  }

  // find each points nearest neighbor
  
  double* nn_output;
  double nn[2]        = {0,0};
  for (int r = 0; r < array.size(); r++) {
    vals[0] = array.at(r).at(0);
    vals[1] = array.at(r).at(1);
    nn_output = find_nearest_neighbor(vals, array);
    nn[0]     = nn_output[0];
    nn[1]     = nn_output[1];
    // display results
    printf("[%d, %d] Nearest Neighbor [%d, %d, %.2f]\n", 
        int(vals[0]), int(vals[1]),
        int(nn_output[0]), int(nn_output[1]), nn_output[2]);
    cv::line(bg, cv_offset(vals, bg.cols, bg.rows),
                  cv_offset(nn, bg.cols, bg.rows),
                  cv::Scalar(200, 200, 200), 2, 8);
    cv::imshow("find_cluster", bg);
    cv::waitKey(0);
 } 
  return 0;
}
