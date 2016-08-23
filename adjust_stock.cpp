// #include "jni.h"
// #include <nlopt.hpp>
// #include <vector>
// #include <stdio.h>
// #include <limits>
// #include <exception>
// #include <iostream>
// #include <map>
// #include "Condition.h"
// #include "cost_compute.h"
// #include "fee_compute.h"
// #include "sum_compute.h"
// #include "risk_compute.h"
// #include "revenue_compute.h"
// #include <math.h>

// int np=8;
// double fee_ratio_uplimit=0.005;
// double approach_base=1.2;
// double epsilon=std::numeric_limits<double>::epsilon();


// class optimization_result{
//   public:
//     nlopt::result result;
//     double minf;
//     std::vector<double> *weight;
//     optimization_result(nlopt::result result,double minf,std::vector<double> *weight){
//       this->result=result;
//       this->minf=minf;
//       this->weight=weight;
//     }
//     ~optimization_result(){
//       delete weight;
//     }
// };


// public void adjust_stock(){

// }