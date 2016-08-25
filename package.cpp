#include "package.h"
#include <nlopt.hpp>
#include <vector>
#include <stdio.h>
#include <limits>
#include <exception>
#include <iostream>
#include <map>
#include "Condition.h"
#include "cost_compute.h"
#include "fee_compute.h"
#include "sum_compute.h"
#include "risk_compute.h"
#include "revenue_compute.h"
#include <math.h>
using namespace std;

double fee_ratio_uplimit=0.005;
double approach_base=1.2;
double epsilon=std::numeric_limits<double>::epsilon();

class optimization_result{
  public:
    nlopt::result result;
    double minf;
    std::vector<double> *weight;
    optimization_result(nlopt::result result,double minf,std::vector<double> *weight){
      this->result=result;
      this->minf=minf;
      this->weight=weight;
    //   std::vector<double> lf(np);
      // this->weight=new std::vector<double>(np);
      // for(int i=0;i<this->weight->size();i++)
      //   this->weight->at(i)=weight->at(i);
    }
    ~optimization_result(){
      delete weight;
    }
};

Package::Package(int np){
    this->np=np;
    price=new std::vector<double>(np);
    w_walk_lb=new std::vector<double>(np);
    w_walk_ub=new std::vector<double>(np);
    revenue_weight=new std::vector<double>(np);
    risk_weight=new std::vector<double>(np*np);
}
int Package::getNp(){
    return np;
}
void Package::setNp(int x){
    this->np=x;
    delete price;
    price=new std::vector<double>(np);
    delete w_walk_lb;
    w_walk_lb=new std::vector<double>(np);
    delete w_walk_ub;
    w_walk_ub=new std::vector<double>(np);
    delete revenue_weight;
    revenue_weight=new std::vector<double>(np);
    delete risk_weight;
    risk_weight=new std::vector<double>(np*np);
}
void Package::setPrice(double *weight){
    for(int i=0;i<np;i++){
        price->at(i)=weight[i];
    }
}
void Package::setLb(double *weight){
    for(int i=0;i<np;i++){
        w_walk_lb->at(i)=weight[i];
    }
}
void Package::setUb(double *weight){
    for(int i=0;i<np;i++){
        w_walk_ub->at(i)=weight[i];
    }
}
void Package::setRevenue(double *weight){
    for(int i=0;i<np;i++){
        revenue_weight->at(i)=weight[i];
    }
}
void Package::setRisk(double *weight){
    for(int i=0;i<np;i++){
        for(int j=0;j<np;j++)
            risk_weight->at(i*np+j)=weight[i*np+j];
    }
}
void Package::run(double assets,double *w_original,double *out_weights){
    std::vector<double> w_walk_u(np);
    std::vector<double> w_walk_d(np); 
    std::vector<double> w_original_vec(np);
    double fee_uplimit=assets*fee_ratio_uplimit;
    cout<<"up and down:"<<endl;
    for(int i=0;i<np;i++){
        w_walk_u[i]=w_original[i]+1.5*price->at(i)/fee_uplimit;
        w_walk_d[i]=w_original[i]-1.5*price->at(i)/fee_uplimit;  
        w_original_vec[i]=w_original[i];
        cout<<w_walk_u[i]<<" "<<w_walk_d[i]<<endl;
    }


    Condition *condition=new Condition(assets);
    condition->w_original=&w_original_vec;
    condition->w_walk_d=&w_walk_d;
    condition->w_walk_u=&w_walk_u;
    condition->price=price;
    std::vector<int> mask_d(np);
    condition->mask=&mask_d;
    for(int i=0;i<np;i++){
        condition->mask->at(i)=0;
    }
    condition->revenue_weight=revenue_weight;
    condition->risk_weight=risk_weight;

    std::map<int,optimization_result*> result_map;
    int possibility=pow(2,np);
    
    for(int i=0;i<possibility;i++){
        int fake_np=0;
        for(int j=0;j<np;j++){
            condition->mask->at(j)= (i & (1<<j)) ? 1:(fake_np++,0);
            //condition->mask->at(j)=0;
            printf("%d ",condition->mask->at(j));
        }
        printf("fake_np is %d;i is %d\n",fake_np,i);
        if(fake_np==0){
            //所有的数据都保持不动
            std::vector<double> grad(fake_np);
            double minf=revenue_cost(std::vector<double>(0),grad,condition);
            printf("%lf\n",minf);
            std::vector<double> *weight_result=new std::vector<double>(np);
            for(int j=0;j<np;j++)
                weight_result->at(j)=w_original[j];
            result_map.insert(std::pair<int,optimization_result*>(i,new optimization_result(nlopt::SUCCESS,minf,weight_result)));
            continue;
        }
        // if(i==105)
        //   std::cout<<std::endl;
        //-----------------------------------------------------------------
        std::vector<double> *w_walk=new std::vector<double>(fake_np);
        std::vector<double> *w_walk_lower_bounds=new std::vector<double>(fake_np);
        std::vector<double> *w_walk_upper_bounds=new std::vector<double>(fake_np);
        //-----------------------------------------------------------------
        int t=0;
        for(int j=0;j<np;j++){
            if(condition->mask->at(j)==0){
                w_walk->at(t)=(w_original[j]>=w_walk_ub->at(j)?w_walk_ub->at(j):(w_original[j]<=w_walk_lb->at(j)?w_walk_lb->at(j):w_original[j]));
                w_walk_lower_bounds->at(t)=w_walk_lb->at(j);
                w_walk_upper_bounds->at(t)=w_walk_ub->at(j);
                t++;
            }
        }
        nlopt::opt opt_local(nlopt::LD_LBFGS,fake_np);

        nlopt::opt opt(nlopt::AUGLAG, fake_np);
        // opt.set_min_objective(cost_f_continuous,condition);
        // opt.add_inequality_constraint(fee_constrained_continous,condition,1e-8);
        // opt.add_equality_constraint(sum_constrained_continous,condition,1e-8);

        opt.set_min_objective(revenue_cost,condition);
        opt.add_inequality_constraint(risk_constrained,condition,1e-8);
        //opt.add_inequality_constraint(risk_constrained_ne,condition,1e-8);
        opt.add_equality_constraint(fee_sum_constrained,condition,1e-8);
        //opt.add_inequality_constraint(fee_sum_constrained,condition,1e-8);
        opt.set_xtol_rel(1e-6);
        opt.set_maxtime(0.05);
        opt.set_lower_bounds(*w_walk_lower_bounds);
        opt.set_upper_bounds(*w_walk_upper_bounds);
        opt.set_local_optimizer(opt_local);
        nlopt::result result;
        double minf=100.0;
        std::vector<double> *weight_result=new std::vector<double>(np);
        try{
            result = opt.optimize(*w_walk, minf);
            int l=0;
            for(int j=0;j<np;j++){
            if(condition->mask->at(j)==0){
                std::cout<<w_walk->at(l)<<" ";
                weight_result->at(j)=w_walk->at(l);
                l++;
            }
            else{
                std::cout<<w_original[j]<<" ";
                weight_result->at(j)=w_original[j];
            }
            }
            std::cout<<std::endl;
            std::vector<double> sx(0);
            std::cout<<risk_constrained(*w_walk,sx,condition)<<std::endl;
            std::cout<<fee_sum_constrained(*w_walk,sx,condition)<<std::endl;
            result_map.insert(std::pair<int,optimization_result*>(i,new optimization_result(result,minf,weight_result)));
        }catch(std::exception &e){
            std::cout<<e.what()<<std::endl;
            result_map.insert(std::pair<int,optimization_result*>(i,new optimization_result(nlopt::FAILURE,minf,weight_result)));
        }
        //--------------------------------------------------------------------
        delete w_walk;
        delete w_walk_lower_bounds;
        delete w_walk_upper_bounds;
        //--------------------------------------------------------------------
    }

    std::map<int,optimization_result*>::iterator it;  
    it=result_map.begin();  
    double mmm=100.0;
    //std::vector<double> mm_weight(np);
    std::cout<<"================================="<<std::endl;
    for (it;it!=result_map.end();it++)  
    {  
        if(it->second->minf<mmm && (it->second->result >0 && it->second->result <6)){
            mmm=it->second->minf;
            for(int i=0;i<np;i++)
                //mm_weight[i]=it->second->weight->at(i);
                out_weights[i]=it->second->weight->at(i);
        }
        std::cout<<it->first<<":"<<it->second->result<<" "<<it->second->minf<<std::endl;  
    }  

}
