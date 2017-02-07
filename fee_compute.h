#ifndef _fee_compute_h_
#define _fee_compute_h_
#include <vector>

/**
计算费率
根据当前权重（w_walk）和原始权重(w_original)来计算手续费
**/
double fee_continous(const std::vector<double> &w_walk,const std::vector<double> &w_original,
                    const std::vector<int> &mask,const std::vector<double> &price,const double fee_uplimit);
/**
使用手续费作为限制条件，计算费率和受限条件导致的梯度
**/
double fee_constrained_continous(const std::vector<double> &w_walk,std::vector<double> &grad,void *my_func_data);

#endif