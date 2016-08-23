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

// double fee(const std::vector<double> &w_walk,const std::vector<double> &w_original,const std::vector<double> &price,const double fee_uplimit){
//     double fee=0.0;
//     for(int i=0;i<w_walk.size();i++){
//       double x=w_walk[i];
//       double y=w_original[i];
//       double price_=price[i];
//       if(fabs(x-y)<=epsilon){
        
//       }else{
//         fee+=std::max(fabs(x-y)*fee_uplimit/price_,1.5);  
//       }
//     } 
//     return fee*fee_ratio_uplimit/fee_uplimit; 
// }

double fee_continous(const std::vector<double> &w_walk,const std::vector<double> &w_original,const std::vector<int> &mask,const std::vector<double> &price,const double fee_uplimit){
    double fee=0.0;
    int j=0;
    for(int i=0;i<w_original.size();i++){
        if(mask[i]==0){
            fee+=std::max(fabs(w_walk[j++]-w_original[i])*fee_uplimit/price[i],1.5);
        }
    }
    return fee*fee_ratio_uplimit/fee_uplimit;
    // for(int i=0;i<w_walk.size();i++){
    //   double x=w_walk[i];
    //   double y=w_original[i];
    //   double price_=price[i];
    //   fee+=std::max(fabs(x-y)*fee_uplimit/price_,1.5);  
    // } 
    // return fee*fee_ratio_uplimit/fee_uplimit;     
}

// double approach_fee(const std::vector<double> &w_walk,const std::vector<double> &w_original,const std::vector<double> &w_walk_u,
//                     const std::vector<double> &w_walk_d,const std::vector<double> &price,const double fee_uplimit){
//   double fee=0.0;
//     for(int i=0;i<w_walk.size();i++){
//       double x=w_walk[i];
//       double y=w_original[i];
//       double price_=price[i];
//       double up=w_walk_u[i];
//       double down=w_walk_d[i];
//       if(fabs(x-y)<=epsilon){
        
//       }else if(x>=up || x<=down){
//         fee+=fabs(x-y)*fee_uplimit/price_;    
//       }else if(x>y){
//         fee+=(1.5-approach_base)*(x-y)/(up-y)+approach_base;
//       }else{
//         fee+=(1.5-approach_base)*(x-y)/(down-y)+approach_base;
//       }
//     } 
//     return fee*fee_ratio_uplimit/fee_uplimit;   
// }


// double fee_approach_constrained(const std::vector<double> &w_walk, std::vector<double> &grad, void *my_func_data){
//     Condition * condition=(Condition *)my_func_data;
//     std::vector<double> w_walk_u=*(condition->w_walk_u);
//     std::vector<double> w_walk_d=*(condition->w_walk_d);
//     std::vector<double> w_original=*(condition->w_original);
//     std::vector<double> price=*(condition->price);
//     double fee_uplimit=condition->get_fee_uplimit();

//     double fee_apc=approach_fee(w_walk,w_original,w_walk_u,w_walk_d,price,fee_uplimit)-fee_ratio_uplimit;

//     printf("fee is %lf ",fee_apc);
//     for(int i=0;i<w_walk.size();i++){
//         double df_x=0.0;
//         double up=w_walk_u[i];
//         double down=w_walk_d[i];
//         double original=w_original[i];
//         double price_=price[i];
//         double x=w_walk[i];
//         printf("%lf ",x);
//         if(x>up){
//             df_x+=fee_uplimit/price_;
//         }else if(x>original)
//             df_x+=(1.5-approach_base)/(up-original);
//         else if(fabs(x-original)<=epsilon)
//             df_x+=0.0;
//         else if(x>=down)
//             df_x+=(1.5-approach_base)/(down-original);
//         else 
//             df_x+=(-1)*fee_uplimit/price_;
//         if (!grad.empty())
//             grad[i]=df_x;
//     }
//     printf("\n");
//     return fee_apc;
// }
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

    // for(int i=0;i<w_walk.size();i++){
    //     double df_x=0.0;
    //     double up=w_walk_u[i];
    //     double down=w_walk_d[i];
    //     double original=w_original[i];
    //     double price_=price[i];
    //     double x=w_walk[i];
    //     printf("%lf ",x);
    //     if(x>up){
    //         df_x+=fee_uplimit/price_;
    //     }
    //     else if(x>=down)
    //         df_x+=0.0;
    //     else 
    //         df_x+=(-1)*fee_uplimit/price_;
    //     if (!grad.empty())
    //         grad[i]=df_x;
    // }
    // printf("\n");
    // return fee_c;    
}