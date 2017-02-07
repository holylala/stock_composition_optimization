#include "sum_compute.h"
#include <vector>
#include "Condition.h"
#include "fee_compute.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;

double sum_constrained_continous(const std::vector<double> &x,std::vector<double> &grad,void *my_func_data){

    Condition * condition=(Condition *)my_func_data;
    std::vector<double> *w_walk_u=condition->w_walk_u;
    std::vector<double> *w_walk_d=condition->w_walk_d;
    std::vector<double> *w_original=condition->w_original;
    std::vector<double> *price=condition->price;
    std::vector<int> *mask=condition->mask;
    double fee_uplimit=condition->get_fee_uplimit();

    double sum=0.0;
    double fee_c=fee_continous(x,*w_original,*mask,*price,fee_uplimit);
    int j=0;
    for(int i=0;i<w_original->size();i++){
        if(mask->at(i)==0){
            if(!grad.empty()){
                double up=w_walk_u->at(i);
                double down=w_walk_d->at(i);
                if(x[j]>=up)
                    grad[j]=2*fee_uplimit/price->at(i);
                else if(x[j]<=down)
                    grad[j]=2*(-1)*fee_uplimit/price->at(i);
                else
                    grad[j]=0.0;
                grad[j]+=1.0;
            }
            sum+=x[j];
            j++;
        }
        else{
            sum+=w_original->at(i);
        }
    }
    //printf("sum is %lf\n",sum+2*fee_c-1.0);
    return sum+2*fee_c-1.0;
    // for(int i=0;i<x.size();i++){
    //     if(!grad.empty())
    //         grad[i]=1.0;
    //     sum+=x[i];
    // }
    // printf("sum is %lf\n",sum+2*fee_c-1.0);
    // return (sum+2*fee_c-1.0);
}
double fee_sum_constrained(const std::vector<double> &x,std::vector<double> &grad,void *my_func_data){
    Condition * condition=(Condition *)my_func_data;
    std::vector<double> *w_walk_u=condition->w_walk_u;
    std::vector<double> *w_walk_d=condition->w_walk_d;
    std::vector<double> *w_original=condition->w_original;
    std::vector<double> *price=condition->price;
    std::vector<int> *mask=condition->mask;
    double fee_uplimit=condition->get_fee_uplimit();

    double sum=0.0;
    double fee_c=fee_continous(x,*w_original,*mask,*price,fee_uplimit);
    int j=0;
    for(int i=0;i<w_original->size();i++){
        if(mask->at(i)==0){
            // cout<<x[j]<<" ";
            if(!grad.empty()){
                double up=w_walk_u->at(i);
                double down=w_walk_d->at(i);
                if(x[j]>=up)
                    grad[j]=fee_uplimit/price->at(i);
                else if(x[j]<=down)
                    grad[j]=(-1)*fee_uplimit/price->at(i);
                else
                    grad[j]=0.0;
                grad[j]+=1.0;
            }
            sum+=x[j];
            j++;
        }
        else{
            // cout<<w_original->at(i)<<" ";
            sum+=w_original->at(i);
        }
    }
    // cout<<"sum and fee -1.0 is:"<<sum+fee_c-1.0<<endl;
    return sum+fee_c-1.0;
}
