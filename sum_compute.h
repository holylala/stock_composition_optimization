#ifndef _sum_compute_h_
#define _sum_compute_h_
#include <vector>

/**
使用 sum+2*fee_c-1.0 资产权重和费率
**/
double sum_constrained_continous(const std::vector<double> &x,std::vector<double> &grad,void *my_func_data);

/**
使用 费用比率和资产权重的和作为 受限条件，计算相应的函数值和梯度
**/
double fee_sum_constrained(const std::vector<double> &x,std::vector<double> &grad,void *my_func_data);

#endif