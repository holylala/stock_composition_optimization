#include "Condition.h"
#include <limits>
extern double fee_ratio_uplimit;
double Condition::get_fee_uplimit(){
    return this->fee_uplimit;
}
double Condition::get_total_assets(){
    return this->total_assets;
}
Condition::Condition(double assets):total_assets(assets),fee_uplimit(assets*fee_ratio_uplimit){

}