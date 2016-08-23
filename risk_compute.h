#ifndef _risk_compute_h_
#define _risk_compute_h_
#include <vector>

double risk_compute(std::vector<double> &risk_weight,std::vector<double> &w_original,const std::vector<double> &w_walk,std::vector<int> &mask);

double risk_compute(std::vector<double> &risk_weight,std::vector<double> &w_original);

double risk_constrained(const std::vector<double> &w_walk,std::vector<double> &grad,void *my_func_data);

double risk_constrained_ne(const std::vector<double> &w_walk,std::vector<double> &grad,void *my_func_data);
#endif