#ifndef _revenue_compute_h_
#define _revenue_compute_h_
#include <vector>
/**
以当前的权重计算收益率
**/
double revenue_compute(std::vector<double> &weight,std::vector<int> &mask,std::vector<double> &w_original,const std::vector<double> &w_walk);
/**
使用收益率作为目标函数或者受限条件，计算收益率和相应的梯度
**/
double revenue_cost(const std::vector<double> &w_walk,std::vector<double> &grad,void *my_func_data);
#endif