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

int np=8;
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
//-------------------------------------------------------------------------------------------------------------------


int main (int argc,char *arg[])
{

  std::vector<double> w_target(np);
  w_target[0]=0.05;
  w_target[1]=0.05;
  w_target[2]=0.05;
  w_target[3]=0.3;
  w_target[4]=0.05;
  w_target[5]=0.4;
  w_target[6]=0.05;
  w_target[7]= 0.05;

  std::vector<double> w_original(np);
  w_original[0]= 0.05957734885559657;
  w_original[1]= 0.04765414459188403;
  w_original[2]= 0.043177060336855956;
  w_original[3]= 0.2726055246781877;
  w_original[4]= 0.052109103799610694;
  w_original[5]= 0.4144245107929405;
  w_original[6]= 0.0607072545271851;
  w_original[7]= 0.0496086989099577;

  std::vector<double> price(np);
  price[0]= 104.09;
  price[1]= 34.88;
  price[2]= 33.53;
  price[3]= 49.04;
  price[4]= 87.08;
  price[5]= 54.51;
  price[6]= 83.05;
  price[7]= 32.57;

  std::vector<double> w_walk_lb(np);
  w_walk_lb[0]=0.05;
  w_walk_lb[1]=0.05;
  w_walk_lb[2]=0.05;
  w_walk_lb[3]=0.0;
  w_walk_lb[4]=0.05;
  w_walk_lb[5]=0.05;
  w_walk_lb[6]=0.05;
  w_walk_lb[7]=0.05;

  std::vector<double> w_walk_ub(np);
  w_walk_ub[0]=0.4;
  w_walk_ub[1]=0.4;
  w_walk_ub[2]=0.4;
  w_walk_ub[3]=0.4;
  w_walk_ub[4]=0.4;
  w_walk_ub[5]=0.4;
  w_walk_ub[6]=0.4;
  w_walk_ub[7]=0.4;

  std::vector<double> w_walk_u(np);
  std::vector<double> w_walk_d(np); 
  double assets=11002.3282;
  double fee_uplimit=assets*fee_ratio_uplimit;
  cout<<"up and down:"<<endl;
  for(int i=0;i<np;i++){
    w_walk_u[i]=w_original[i]+1.5*price[i]/fee_uplimit;
    w_walk_d[i]=w_original[i]-1.5*price[i]/fee_uplimit;  
    cout<<w_walk_u[i]<<" "<<w_walk_d[i]<<endl;
  }
  cout<<"up and down end"<<endl;
  std::vector<double> revenue_weight(np);
  double *rev_w=new double[np]{0.0021532084194948575,      7.475738493503719E-4,   -0.006326840321119702,  1.2386054562314402E-4,  0.0011076249589600004,  9.778069429387285E-4,   0.0026140588361591734,  0.0011475750683716931};

  for(int i=0;i<np;i++)
    revenue_weight[i]=rev_w[i];
  delete []rev_w;

  std::vector<double> risk_weight(np*np);
  double *ri_w=new double[np*np]{
    4.1067985538413934E-4, 3.907075502328885E-4,   0.0013062323808480131,  7.735662410732668E-6,   -1.1913970711014461E-5, -1.0174725175322413E-5, 3.4813213480351635E-4,  4.576893690584706E-4,
    3.9070755023288845E-4,        4.980370896229549E-4,   0.001513817137052966,   1.1699173768480222E-5,  -9.542461024679097E-6,  -1.4393811000831372E-5, 3.456790153355711E-4,   5.715494965920865E-4,
    0.0013062323808480131,        0.0015138171370529662,  0.04958201978191183,    7.684132301858718E-5,   1.1367509755177182E-4,  -1.9702229531360643E-5, 8.827622707034841E-4,   0.0026399487149149346,
    7.735662410732667E-6, 1.1699173768480224E-5,  7.684132301858717E-5,   2.4624148670171226E-5,  3.07984244815917E-5,    8.048000897627799E-6,   5.8710295576385835E-5,  3.0988867325667597E-6,
    -1.1913970711014461E-5,       -9.542461024679098E-6,  1.1367509755177181E-4,  3.07984244815917E-5,    9.202487363548772E-5,   2.862862120817286E-5,   1.4124648511060727E-4,  -2.5535310365541127E-5,
    -1.0174725175322411E-5,       -1.4393811000831372E-5, -1.9702229531360646E-5, 8.048000897627799E-6,   2.862862120817286E-5,   2.0301514048952664E-5,  4.325400833141057E-5,   -2.1909014009757933E-5,
    3.4813213480351635E-4,        3.456790153355711E-4,   8.827622707034841E-4,   5.8710295576385835E-5,  1.4124648511060727E-4,  4.325400833141057E-5,   0.0010310818434250437,  3.579261165328639E-4,
    4.5768936905847063E-4,        5.715494965920865E-4,   0.002639948714914934,   3.0988867325667597E-6,  -2.5535310365541127E-5, -2.1909014009757936E-5, 3.579261165328639E-4,   0.0013533114981784528
  };
  for(int i=0;i<np;i++){
    for(int j=0;j<np;j++)
      risk_weight[i*np+j]=ri_w[i*np+j];
  }
  delete []ri_w;

  Condition *condition=new Condition(assets);
  condition->w_original=&w_original;
  //condition->w_target=&w_target;
  condition->w_walk_d=&w_walk_d;
  condition->w_walk_u=&w_walk_u;
  condition->price=&price;
  std::vector<int> mask_d(np);
  condition->mask=&mask_d;
  for(int i=0;i<np;i++){
    condition->mask->at(i)=0;
  }
  condition->revenue_weight=&revenue_weight;
  condition->risk_weight=&risk_weight;


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
        w_walk->at(t)=(w_original[j]>=w_walk_ub[j]?w_walk_ub[j]:(w_original[j]<=w_walk_lb[j]?w_walk_lb[j]:w_original[j]));
        w_walk_lower_bounds->at(t)=w_walk_lb[j];
        w_walk_upper_bounds->at(t)=w_walk_ub[j];
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
    opt.set_maxtime(10.0);
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
  std::vector<double> mm_weight(np);
  std::cout<<"================================="<<std::endl;
  for (it;it!=result_map.end();it++)  
  {  
      if(it->second->minf<mmm && (it->second->result >0 && it->second->result <6)){
        mmm=it->second->minf;
        for(int i=0;i<np;i++)
          mm_weight[i]=it->second->weight->at(i);
      }
      std::cout<<it->first<<":"<<it->second->result<<" "<<it->second->minf<<std::endl;  
  }  
  std::cout<<"================================="<<std::endl;  
  std::cout<<"min f is:"<<mmm<<std::endl;
  for(int i=0;i<np;i++){
    std::cout<<mm_weight[i]<<" ";
  }
  std::cout<<std::endl;
  getchar();

//   std::vector<double> w_walk(np);

//   for(int i=0;i<np;i++){
//     w_walk[i]=w_original[i];
//   }

//     nlopt::opt opt_local(nlopt::LN_PRAXIS,np); //不需要可以微分
//     //nlopt::opt opt_local(nlopt::LD_LBFGS,np); //需要可以微分

//     // opt_local.set_min_objective(cost_f,NULL);
//     // opt_local.set_xtol_rel(1e-4);

//     nlopt::opt opt(nlopt::AUGLAG, np);
//     opt.set_min_objective(cost_f,NULL);
//     opt.add_inequality_constraint(fee_constrained,NULL,1e-8);
//     opt.add_equality_constraint(sum_constrained,NULL,1e-8);
//     opt.set_xtol_rel(1e-6);
//     // opt.add_inequality_constraint(sum_constrained,NULL,1e-8);
//     // opt.add_inequality_constraint(n_sum_constrained,NULL,1e-8);
//     opt.set_local_optimizer(opt_local);

//     double minf;
//     std::vector<double> s;
//     printf("initial cost is :%lf%\n",cost_f(w_walk,s,NULL));
//     printf("initial fee is :%lf%\n",fee_constrained_continous(w_walk,s,NULL));
//     printf("initial sum is :%lf%\n",sum_constrained_continous(w_walk,s,NULL));
//     nlopt::result result;
//     try{
//          result = opt.optimize(w_walk, minf);
//     }catch(std::exception &e){
        
//         std::cout<<e.what()<<std::endl;
//     }
//     printf("min cost is %lf\n",minf);
//     for(int i=0;i<np;i++){
//         printf(" %d parameter is:%lf\n",i,w_walk[i]);
//     }
//     printf("end cost is :%lf%\n",cost_f(w_walk,s,NULL));
//     printf("end fee is :%lf%\n",fee_constrained_continous(w_walk,s,NULL));
//     printf("end sum is :%lf%\n",sum_constrained_continous(w_walk,s,NULL));
//     getchar();


}
