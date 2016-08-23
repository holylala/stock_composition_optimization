#ifndef _sum_compute_h_
#define _sum_compute_h_
#include <vector>
// double sum_constrained(const std::vector<double> &x, std::vector<double> &grad, void *my_func_data);

double sum_constrained_continous(const std::vector<double> &x,std::vector<double> &grad,void *my_func_data);


double fee_sum_constrained(const std::vector<double> &x,std::vector<double> &grad,void *my_func_data);
// double n_sum_constrained(const std::vector<double> &x, std::vector<double> &grad, void *my_func_data);

#endif