#ifndef _condition_h_
#define _condition_h_
#include <vector>

/**
进行调仓算法过程中需要传递的变量
**/
class Condition
{
    private:
        double fee_uplimit;
        double total_assets;
    public: 
        //std::vector<double> *w_target;
        std::vector<double> *w_original;
        std::vector<double> *price;
        std::vector<double> *w_walk_d;
        std::vector<double> *w_walk_u;
        std::vector<int> *mask;
        std::vector<double> *revenue_weight;
        std::vector<double> *risk_weight;
        double get_fee_uplimit();
        double get_total_assets();
        Condition(double assets);
};
#endif