#include "revenue_compute.h"
#include "Condition.h"
#include "fee_compute.h"
#include <iostream>
using namespace std;


double revenue_compute(std::vector<double> &weight,std::vector<int> &mask,std::vector<double> &w_original,const std::vector<double> &w_walk){
    double revenue=0.0;
    int j=0;
    for(int i=0;i<w_original.size();i++){
        if(mask[i]==0){
            revenue+=weight[i]*w_walk[j++];
        }
        else{
            revenue+=weight[i]*w_original[i];
        }
    }
    return revenue;
}

double revenue_cost(const std::vector<double> &w_walk,std::vector<double> &grad,void *my_func_data){
    Condition *condition=(Condition *)my_func_data;
    std::vector<double> *revenue_weight=condition->revenue_weight;
    std::vector<int> *mask=condition->mask;
    std::vector<double> *w_original=condition->w_original;
    std::vector<double> *price=condition->price;
    std::vector<double> *w_walk_u=condition->w_walk_u;
    std::vector<double> *w_walk_d=condition->w_walk_d;
    double fee_uplimit=condition->get_fee_uplimit();
    double fee=fee_continous(w_walk,*w_original,*mask,*price,fee_uplimit);
    double revenue=revenue_compute(*revenue_weight,*mask,*w_original,w_walk);
    if(!grad.empty()){
        int j=0;
        for(int i=0;i<w_original->size();i++){
            if(mask->at(i)==0){
                // cout<<w_walk[j]<<" ";
                //fee grad;
                double up=w_walk_u->at(i);
                double down=w_walk_d->at(i);
                if(w_walk[j]>=up)
                    grad[j]=fee_uplimit/price->at(i);
                else if(w_walk[j]<=down)
                    grad[j]=(-1)*fee_uplimit/price->at(i);
                else
                    grad[j]=0.0;
                //加上revenue weight grad
                grad[j]-=revenue_weight->at(i);
                j++;    
            }    
            else
                ;
                // cout<<w_original->at(i)<<" ";
        }
    }
    // cout<<"fee - revenue is :"<<fee-revenue<<endl;
    return fee-revenue;
}