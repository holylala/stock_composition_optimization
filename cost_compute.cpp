// #include <vector>
// #include "cost_compute.h"
// #include "Condition.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include "fee_compute.h"
// #include <limits>
// extern int np;
// extern double fee_ratio_uplimit;
// extern double approach_base;
// extern double epsilon;
// const int weight2=100;
// const int weight3=500;
// double cost_f (const std::vector<double> &w_walk, std::vector<double> &grad, void *my_func_data){
//     Condition *condition=(Condition *)my_func_data;
//     std::vector<double> *w_target=condition->w_target;
//     std::vector<double> *w_original=condition->w_original;
//     std::vector<int> *mask=condition->mask;
//     double cost=0.0;
//     double sum=0.0;

//     int j=0;
//     for(int i=0;i<w_target->size();i++){
//         if(mask->at(i)==0){
//             cost+=fabs(w_walk[j]-w_target->at(i));
//             if(!grad.empty()){
//                 if(fabs(w_walk[j]-w_target->at(i))<=epsilon){
//                     grad[j]=0.0;
//                 }else if(w_walk[j]>w_target->at(i))
//                     grad[j]=1.0;
//                 else
//                     grad[j]=-1.0;
//             }
//             printf("%lf ",w_walk[j]);
//             j++;
//         }else{
//             cost+=fabs(w_original->at(i)-w_target->at(i));
//             printf("%lf ",w_original->at(i));
//         }

//     }
//     printf("cost is %lf\n",cost);
//     // for(int i=0;i<w_walk.size();i++){
//     //     double x=w_walk[i];
//     //     double y=w_target[i];
//     //     cost+=fabs(x-y);
//     //     if (!grad.empty()){
//     //         if(x>y)
//     //             grad[i]=1;
//     //         else if(fabs(x-y)<=epsilon)
//     //             grad[i]=0;
//     //         else
//     //             grad[i]=-1;
//     //         printf("%lf ",x);
//     //     }
//     // }
//     // printf("cost is %lf\n",cost);
    
//     // return cost;
// }
// double cost_f_continuous(const std::vector<double> &w_walk,std::vector<double> &grad,void *my_func_data){
//     Condition *condition=(Condition *)my_func_data;
//     std::vector<double> *w_target=condition->w_target;
//     std::vector<double> *w_original=condition->w_original;
//     std::vector<int> *mask=condition->mask;
//     double cost=0.0;
//     double sum=0.0;
//     int j=0;
//     for(int i=0;i<w_target->size();i++){
//         if(mask->at(i)==0){
//             cost+=(w_walk[j]-w_target->at(i))*(w_walk[j]-w_target->at(i));
//             if(!grad.empty())
//                 grad[j]=2*(w_walk[j]-w_target->at(i));
//             //printf("%lf ",w_walk[j]);
//             j++;
//         }else{
//             //printf("%lf ",w_original->at(i));
//             cost+=(w_original->at(i)-w_target->at(i))*(w_original->at(i)-w_target->at(i));
//         }        
//     }
//     //printf("cost is %lf\n",cost);
//     return cost;
  
// //   for(int i=0;i<w_walk.size();i++){
// //       double x=w_walk[i];
// //       double y=w_target[i];
// //       cost+=(x-y)*(x-y);
// //       if (!grad.empty()){
// //           grad[i]=2*(x-y);
// //       }
// //       printf("%lf ",x);
// //   }
// //   printf("cost is %lf\n",cost);
// //   return cost;    
// }

// // double t_cost_f(const std::vector<double> &w_walk,std::vector<double> &grad,void *my_func_data){

// //     Condition * condition=(Condition *)my_func_data;
// //     std::vector<double> w_walk_u=*(condition->w_walk_u);
// //     std::vector<double> w_walk_d=*(condition->w_walk_d);
// //     std::vector<double> w_original=*(condition->w_original);
// //     std::vector<double> price=*(condition->price);
// //     std::vector<double> w_target=*(condition->w_target);
// //     double fee_uplimit=condition->get_fee_uplimit();

// //     double cost=0.0;
// //     double sum=0.0;
// //     double fee_apc=approach_fee(w_walk,w_original,w_walk_u,w_walk_d,price,fee_uplimit);
// //     double fee_cost=weight2*(fee_apc-fee_ratio_uplimit);
// //     for(int i=0;i<w_walk.size();i++){
// //         double df_x=0.0;
// //         double up=w_walk_u[i];
// //         double down=w_walk_d[i];
// //         double original=w_original[i];
// //         double price_=price[i];
// //         double x=w_walk[i];
// //         printf("%lf ",x);

        
// //         double y=w_target[i];
// //         cost+=fabs(x-y);
// //         if(x>y)
// //             df_x+=1;
// //         else if(fabs(x-y)<=epsilon)
// //             df_x+=0;
// //         else
// //             df_x+=-1;
// //         //-----------------------------------------------------
// //         if(fee_cost>=0.0){
// //             if(x>up){
// //                 df_x+=weight2*fee_uplimit/price_;
// //             }else if(x>original)
// //                 df_x+=weight2*(1.5-approach_base)/(up-original);
// //             else if(fabs(x-original)<=epsilon)
// //                 df_x+=0.0;
// //             else if(x>=down)
// //                 df_x+=weight2*(1.5-approach_base)/(down-original);
// //             else 
// //                 df_x+=weight2*(-1)*fee_uplimit/price_;
// //         }
// //         //------------------------------------------------------------------
// //         sum+=w_walk[i];
// //         if (!grad.empty())
// //             grad[i]=df_x;
// //     }   
// //     if(fee_cost>0) 
// //         cost+=fee_apc;
// //     cost+=weight3*(sum+2*fee_apc-1)*(sum+2*fee_apc-1);
// //     for(int i=0;i<w_walk.size();i++){
// //         if(!grad.empty()){
// //             grad[i]+=2*(sum+2*fee_apc-1);
// //         }
// //     }
// //     printf("cost is %lf\n",cost);
// //     return cost;
// // }
