#include <nlopt.hpp>
#include <vector>
#include <stdio.h>
#include <limits>
#include <exception>
#include <iostream>
double myvfunc(const std::vector<double> &x, std::vector<double> &grad, void *my_func_data)
{
    printf("%lf %lf ",x[0],x[1]);
    if (!grad.empty()) {
        grad[0] = 0.0;
        grad[1] = 0.5 / sqrt(x[1]);
    }
    printf("cost is %lf\n",sqrt(x[1]));
    return sqrt(x[1]);
}
typedef struct {
    double a, b;
} my_constraint_data;
double myvconstraint(const std::vector<double> &x, std::vector<double> &grad, void *data)
{
    my_constraint_data *d = reinterpret_cast<my_constraint_data*>(data);
    double a = d->a, b = d->b;
    if (!grad.empty()) {
        grad[0] = 3 * a * (a*x[0] + b) * (a*x[0] + b);
        grad[1] = -1.0;
    }
    return ((a*x[0] + b) * (a*x[0] + b) * (a*x[0] + b) - x[1]);
}
//--------------------------------------------------------------------------------------------------------------------

int psk_main(int argc,char *arg[]){
    nlopt::opt opt_local(nlopt::LD_LBFGS,2);

    nlopt::opt opt(nlopt::AUGLAG, 2);
    opt.set_min_objective(myvfunc,NULL);
    my_constraint_data data[2] = { {2,0}, {-1,1} };
    opt.add_inequality_constraint(myvconstraint, &data[0], 1e-8);
    opt.add_inequality_constraint(myvconstraint, &data[1], 1e-8);
    opt.set_xtol_rel(1e-4);
    std::vector<double> lb(2);
    lb[0] = -HUGE_VAL; lb[1] = 1e-5;
    opt.set_lower_bounds(lb);

    opt.set_local_optimizer(opt_local);
    std::vector<double> x(2);
    x[0] = 1.234; x[1] = 5.678;
    double minf;
    try{
        nlopt::result result = opt.optimize(x, minf);
    }catch(std::exception &e){
        std::cout<<e.what()<<std::endl;
    }

    printf("%lf\n",minf);
    getchar();
}