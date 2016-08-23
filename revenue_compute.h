#ifndef _revenue_compute_h_
#define _revenue_compute_h_
#include <vector>

double revenue_compute(std::vector<double> &weight,std::vector<int> &mask,std::vector<double> &w_original,const std::vector<double> &w_walk);

double revenue_cost(const std::vector<double> &w_walk,std::vector<double> &grad,void *my_func_data);
#endif