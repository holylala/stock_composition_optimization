#include <vector>
#include "fee_compute.h"
#include "Condition.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits>

extern int np;
extern double fee_ratio_uplimit;
extern double approach_base;
extern double epsilon;

double fee_continous(const std::vector<double> &w_walk,const std::vector<double> &w_original,const std::vector<int> &mask,const std::vector<double> &price,const double fee_uplimit){
    double fee=0.0;
    int j=0;
    for(int i=0;i<w_original.size();i++){
        if(mask[i]==0){
            fee+=std::max(fabs(w_walk[j++]-w_original[i])*fee_uplimit/price[i],1.5);
        }
    }
    return fee*fee_ratio_uplimit/fee_uplimit; 
}

double fee_constrained_continous(const std::vector<double> &w_walk,std::vector<double> &grad,void *my_func_data){
    Condition * condition=(Condition *)my_func_data;
    std::vector<double> *w_walk_u=condition->w_walk_u;
    std::vector<double> *w_walk_d=condition->w_walk_d;
    std::vector<double> *w_original=condition->w_original;
    std::vector<double> *price=condition->price;
    std::vector<int> *mask=condition->mask;
    double fee_uplimit=condition->get_fee_uplimit();

    double fee_c=fee_continous(w_walk,*w_original,*mask,*price,fee_uplimit)-fee_ratio_uplimit;
    //printf("fee is %lf\n",fee_c);
    int j=0;
    if(!grad.empty()){
        for(int i=0;i<w_original->size();i++){
            if(mask->at(i)==0){
                double up=w_walk_u->at(i);
                double down=w_walk_d->at(i);
                if(w_walk[j]>=up)
                    grad[j]=fee_uplimit/price->at(i);
                else if(w_walk[j]<=down)
                    grad[j]=(-1)*fee_uplimit/price->at(i);
                else
                    grad[j]=0.0;
                j++;

            }
        }  
    }
    return fee_c;
}