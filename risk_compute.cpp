#include "risk_compute.h"
#include "Condition.h"
#include <vector>
#include <iostream>
using namespace std;
double risk_compute(std::vector<double> &risk_weight,std::vector<double> &w_original,const std::vector<double> &w_walk,std::vector<int> &mask){
    double risk=0.0;
    int np=w_original.size();
    int k=0;
    for(int i=0;i<np;i++){
        int l=0;
        for(int j=0;j<np;j++){
            double left=w_original[i];
            if(mask[i]==0)
                left=w_walk[k];
            double right=w_original[j];
            if(mask[j]==0)
                right=w_walk[l++];
            risk+=left*risk_weight[i*np+j]*right;
        }
        if(mask[i]==0)
            k++;
    }
    return risk;
}
double risk_compute(std::vector<double> &risk_weight,std::vector<double> &w_original){
    double risk=0.0;
    int np=w_original.size();
    int k=0;
    for(int i=0;i<np;i++){
        int l=0;
        for(int j=0;j<np;j++){
            double left=w_original[i];
            double right=w_original[j];
            risk+=left*risk_weight[i*np+j]*right;
        }
    }
    return risk;    
}
double threshhold=4e-5;
double risk_constrained(const std::vector<double> &w_walk,std::vector<double> &grad,void *my_func_data){
    Condition *condition=(Condition *)my_func_data;
    std::vector<double> *risk_weight=condition->risk_weight;
    std::vector<int> *mask=condition->mask;
    std::vector<double> *w_original=condition->w_original;
    int l=0;// grad指示标量
    const int np=w_original->size();
    if(!grad.empty()){
        for(int i=0;i<np;i++){
            if(mask->at(i)==0){
                // cout<<w_walk[l]<<" ";
                grad[l]=0.0;
                int k=0;
                for(int j=0;j<np;j++){
                    if(mask->at(j)==0)
                        grad[l]+=risk_weight->at(i*np+j)*w_walk[k++];
                    else
                        grad[l]+=risk_weight->at(i*np+j)*w_original->at(j);
                }
                k=0;
                for(int j=0;j<np;j++){
                    if(mask->at(j)==0)
                        grad[l]+=risk_weight->at(j*np+i)*w_walk[k++];
                    else
                        grad[l]+=risk_weight->at(j*np+i)*w_original->at(j);
                }
                l++;
            }
            else
                ;
                // cout<<w_original->at(i)<<" ";
        }

    }
    double risk_n=risk_compute(*risk_weight,*w_original,w_walk,*mask);
    double risk_o=risk_compute(*risk_weight,*w_original);
    // cout<<"risk is :"<<risk_n-risk_o-threshhold<<endl;
    return risk_n-risk_o-threshhold;
}

double risk_constrained_ne(const std::vector<double> &w_walk,std::vector<double> &grad,void *my_func_data){
    Condition *condition=(Condition *)my_func_data;
    std::vector<double> *risk_weight=condition->risk_weight;
    std::vector<int> *mask=condition->mask;
    std::vector<double> *w_original=condition->w_original;
    int l=0;// grad指示标量
    const int np=w_original->size();
    if(!grad.empty()){
        for(int i=0;i<np;i++){
            if(mask->at(i)==0){
                // cout<<w_walk[l]<<" ";
                grad[l]=0.0;
                int k=0;
                for(int j=0;j<np;j++){
                    if(mask->at(j)==0)
                        grad[l]+=risk_weight->at(i*np+j)*w_walk[k++];
                    else
                        grad[l]+=risk_weight->at(i*np+j)*w_original->at(j);
                }
                k=0;
                for(int j=0;j<np;j++){
                    if(mask->at(j)==0)
                        grad[l]+=risk_weight->at(j*np+i)*w_walk[k++];
                    else
                        grad[l]+=risk_weight->at(j*np+i)*w_original->at(j);
                }
                grad[l]=(-1)*grad[l];
                l++;
            }
            else
                ;
                // cout<<w_original->at(i)<<" ";
        }
    }
    double risk_o=risk_compute(*risk_weight,*w_original);
    double risk_n=risk_compute(*risk_weight,*w_original,w_walk,*mask);
    // cout<<"risk ne is :"<<risk_o-threshhold-risk_n<<endl;
    return risk_o-threshhold-risk_n;
}