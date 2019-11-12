#include "max_robotics.h"
/*
 * max_robotics.cpp
 * Adapted from modern robotics: see modernrobotics.org
 * Provides useful tools for open-chain robotics
 */
#include <Eigen/Dense>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
//#define M_PI           3.14159265358979323846  /* pi */
#define DATA_PATH "/Users/maxmaguire/Desktop/PostGradWork/DataFolder/TxtFiles/"

namespace mr {
  // evaluate if num is close to 0
  bool NearZero(const double val) {return (std::abs(val) < 0.00001);}

  namespace fileio {

    // read vec row from file path
    void load_vec_row(std::string file_name, std::vector<double> &vec, int line) {
      // reading from file	
      std::ifstream infile(DATA_PATH + file_name);
      std::cout << "Retrieving row " << line << " in " << file_name << std::endl;
      if (infile.is_open()) {
        // set variables
        std::string tmp_line;
        double tmp_val;
        int    header_lines = 0;
        int    count        = 0;
        // skip over header
        while (std::getline(infile, tmp_line)) {
          if (tmp_line[0] != '#'){
            count++;
            break;
          }
        }
        // skip to desired line
        while (count < line) { 
          infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          count++;
        }
        // read desired row into vector and close file
        if (line > 0) {
          // Exception case 2: Invalid line number
          if (!std::getline(infile, tmp_line)) {
            throw std::invalid_argument("\n\tInvalid line number [" + std::to_string(line) + "]");
          }
        }
        std::istringstream s(tmp_line);
        while (s >> tmp_val) { 
          vec.push_back(tmp_val);
        }
        infile.close();
      // Exception case 1: Invalid file path 
      } else { 
        throw std::invalid_argument("\n\tCould not access file [" + file_name + "]");
      }
      // Exception case 3: Invalid data type
      if (vec.empty()) {
            throw std::invalid_argument("\n\tInvalid Data type");
      }
    } 

    // load vec collumn from file path
    void load_vec_col(std::string file_name, std::vector<double> &vec, int col) {
      // reading from file	
      std::ifstream infile(DATA_PATH + file_name);
      std::cout << "Retrieving col " << col << " in " << file_name << std::endl;
      if (infile.is_open()) {
        // set variables
        std::string tmp_line;
        double tmp_val;
        int    count;
        int    header_lines = 0;
        // read through file lines and populate vector with desired col val
        while (std::getline(infile, tmp_line)) {
          //skip over header
          if (tmp_line[0] == '#') continue;
          // push desired column value to vector
          std::istringstream s(tmp_line);
          count = 0;
          while (s >> tmp_val) {
            if (count == col) { break;}
            count++;
          }
          // Exception case 2: Invalid column number
          if (count != col) {
            throw std::invalid_argument("\n\tInvalid column number [ " + std::to_string(col) + " > " + std::to_string(count) + " ]");
          }
          vec.push_back(tmp_val);
        }
        infile.close();
      // Exception case 1: Invalid file path 
      } else { 
        throw std::invalid_argument("\n\tCould not access file [" + file_name + "]");
      }
     
      // Exception case 3: Invalid data type
      if (vec.empty()) {
            throw std::invalid_argument("\n\tInvalid Data type");
      }
    } 
    
    // load vec array from file path
    void load_vec_array(std::string file_name, std::vector<std::vector<double> > &array){
      // reading from file	
      std::ifstream infile(DATA_PATH + file_name);
      std::cout << "Retrieving col array in " << file_name << std::endl;
      if (infile.is_open()) {
        // set variables
        std::string tmp_line;
        double tmp_val;
        int    row = 0, col = 0;
        // iterate through lines in file
        while (std::getline(infile, tmp_line)) {
          //skip over header
          if (tmp_line[0] == '#') continue;
          // populate row vector
          std::vector<double> tmp_vec;
          std::istringstream s(tmp_line);
          while ( s >> tmp_val) {
            tmp_vec.push_back(tmp_val);
          }
          // populate col vector with row vector
          array.push_back(tmp_vec);
        }
        infile.close();
      // Exception case 1: Invalid file path 
      } else { 
        throw std::invalid_argument("\n\tCould not access file [" + file_name + "]"); 
      }

      // Exception case 2: Invalid data type
      if (array.empty()) {
            throw std::invalid_argument("\n\tInvalid Data type");
      }
    }
  }
}
