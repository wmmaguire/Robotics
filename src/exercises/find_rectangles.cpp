#include "max_robotics.h"
#include <cstdio>
#include <utility>
#include <unordered_map>
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

// find 
std::map<std::pair<double, double>, std::vector<double> > find_rectangles(std::vector<std::vector<double> > array) {
  static std::map<std::pair<double, double>, std::vector<double> > rect;
  double x_val, y_val; 
  double x_val2, y_val2;
  for (int i = 0; i < array.size() - 1; i++) {
    x_val = array[i].at(0);
    y_val = array[i].at(1);
    for (int j = i + 1; j < array.size(); j++) {
      x_val2 = array[j].at(0);
      y_val2 = array[j].at(1);
      // increment count for y pairs
      if (x_val == x_val2 && y_val2 > y_val) {
        std::pair<int, int> y_pair(y_val, y_val2);
        rect[y_pair].push_back(x_val);
      }
    }
  }
  return rect;
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
  cv::namedWindow("find_rectangles", cv::WINDOW_NORMAL);
  double vals[2] = {0,0};
  for (int r = 0; r < array.size(); r++) {
    // draw circle
    vals[0] = array.at(r).at(0);
    vals[1] = array.at(r).at(1);
    cv::circle(bg, cv_offset(vals, bg.cols, bg.rows),
                7, cv::Scalar(0, 0, 255), -1);
    cv::imshow("find_rectangles", bg);
  }

  // find all rectangles
  std::map<std::pair<double, double>, std::vector<double> > rectangles;
  std::map<std::pair<double, double>, std::vector<double> >::iterator it;
  rectangles = find_rectangles(array);
  // draw each rectangle
  int total_count = 0;
  for (it = rectangles.begin(); it != rectangles.end(); it++ ) {
    for (int i = 0; i < it->second.size() - 1; i++ ) { 
      // draw rectangle line by line
      double edge[2];
      double edge_next[2];
      edge[0] = it->second.at(i);
      edge[1] = it->first.first;
      edge_next[0] = it->second.at(i + 1);
      edge_next[1] = it->first.first;
      cv::line(bg, cv_offset(edge, bg.cols, bg.rows),
                  cv_offset(edge_next, bg.cols, bg.rows),
                  cv::Scalar(150, 150, 150), 2, 8);
      edge_next[0] = it->second.at(i);
      edge_next[1] = it->first.second;
      cv::line(bg, cv_offset(edge, bg.cols, bg.rows),
                  cv_offset(edge_next, bg.cols, bg.rows),
                  cv::Scalar(150, 150, 150), 2, 8);
      edge[0] = it->second.at(i + 1);
      edge[1] = it->first.second;
      cv::line(bg, cv_offset(edge, bg.cols, bg.rows),
                 cv_offset(edge_next, bg.cols, bg.rows),
                  cv::Scalar(150, 150, 150), 2, 8);
      edge_next[0] = it->second.at(i + 1);
      edge_next[1] = it->first.first;
      cv::line(bg, cv_offset(edge, bg.cols, bg.rows),
                 cv_offset(edge_next, bg.cols, bg.rows),
                  cv::Scalar(150, 150, 150), 2, 8);
      cv::imshow("find_rectangles", bg);
      cv::waitKey(0);
      total_count += (i + 1); 
    }
  }
  // print out total number of rectangles
  std::cout << "total number of rectangles: " << total_count << "\n";

  return 0;
}
