#ifndef _fee_compute_h_
#define _fee_compute_h_
#include <vector>
// double fee(const std::vector<double> &w_walk,const std::vector<double> &w_original,const std::vector<double> &price,const double fee_uplimit);

double fee_continous(const std::vector<double> &w_walk,const std::vector<double> &w_original,
                    const std::vector<int> &mask,const std::vector<double> &price,const double fee_uplimit);

// double approach_fee(const std::vector<double> &w_walk,const std::vector<double> &w_original,const std::vector<double> &w_walk_u,
//                     const std::vector<double> &w_walk_d,const std::vector<double> &price,const double fee_uplimit);

// double fee_approach_constrained(const std::vector<double> &w_walk, std::vector<double> &grad, void *my_func_data);

double fee_constrained_continous(const std::vector<double> &w_walk,std::vector<double> &grad,void *my_func_data);

#endif