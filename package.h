#ifndef _package_h_
#define _package_h_
#include <vector>
class Package{
    private:
        int np;
        std::vector<double> *price;
        std::vector<double> *w_walk_lb;
        std::vector<double> *w_walk_ub;
        std::vector<double> *revenue_weight;
        std::vector<double> *risk_weight;
    public:
        Package(int np);
        int getNp();
        void setNp(int np);
        void setPrice(double *weight);
        void setLb(double *lb);
        void setUb(double *ub);
        void setRevenue(double *weight);
        void setRisk(double *wight);
        void run(double assets,double *original_weights,double *out_weights,bool verbose);


};
#endif