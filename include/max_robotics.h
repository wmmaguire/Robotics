#pragma once
#include <Eigen/Dense>
#include <vector>

namespace mr {
  /*
   * Description: evaluates value if close enough to 0
   * Input: value [double]
   * Returns: Boolean
   */
  bool NearZero(const double);

  // fileio utilities
  namespace fileio {
    // load vec from file
    void load_vec_row(std::string file_name, std::vector<double> &vec, int line);
    void load_vec_col(std::string file_name, std::vector<double> &vec, int col);
    void load_vec_array(std::string file_name, std::vector<std::vector<double> > &array);
  }
}
