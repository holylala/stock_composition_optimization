#ifndef _risk_compute_h_
#define _risk_compute_h_
#include <vector>
/**
包含mask的风险计算，即变换了权重后的风险计算
**/
double risk_compute(std::vector<double> &risk_weight,std::vector<double> &w_original,const std::vector<double> &w_walk,std::vector<int> &mask);
/**
原始的风险计算
**/
double risk_compute(std::vector<double> &risk_weight,std::vector<double> &w_original);
/**
risk_new - risk_old <= threshold
**/
double risk_constrained(const std::vector<double> &w_walk,std::vector<double> &grad,void *my_func_data);
/**
risk_new -risk_old >=-threshold
**/
double risk_constrained_ne(const std::vector<double> &w_walk,std::vector<double> &grad,void *my_func_data);
#endif