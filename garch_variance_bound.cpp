#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define pi 3.141592653589793
/**
garch计算边界中需要使用的插值
**/
double garch_backcast(double *input,double power,int size){
    double tau=75<=size?75:size;
    std::vector<double> w(tau);
    double sum=0.0;
    for(int i=0;i<tau;i++){
        w.at(i)=pow(0.94,i);
        sum+=w.at(i);
    }
    double backcast=0.0;
    for(int i=0;i<tau;i++){
        backcast+=pow(fabs(input[i]),power)*w.at(i)/sum;
    }
    return backcast;
}
/**
garch循环
**/
double *garch_recurion(double *parameters,double *fresides,double *sresides,double *sigma2,int p,int o,int q,int nobs,double backcast,double *var_bounds){
    for(int t=0;t<nobs;t++){
        int loc=0;
        sigma2[t]=parameters[loc];
        loc+=1;
        for(int j=0;j<p;j++){
            if((t-1-j)<0){
                sigma2[t]+=parameters[loc] *backcast;
            }else{
                sigma2[t]+=parameters[loc]*fresides[t-1-j];
            }
            loc+=1;
        }
        for(int j=0;j<o;j++){
            if((t-1-j)<0){
                sigma2[t]+=parameters[loc]*0.5*backcast;
            }else{
                sigma2[t]+=parameters[loc]*fresides[t-1-j]*(sresides[t-1-j]<0);
            }
            loc+=1;
        }
        for(int j=0;j<q;j++){
            if((t-1-j)<0){
                sigma2[t]+=parameters[loc]*backcast;
            }else{
                sigma2[t]+=parameters[loc]*sigma2[t-1-j];
            }
            loc+=1;
        }
        if(sigma2[t]<var_bounds[2*t]){
            sigma2[t]=var_bounds[2*t];
        }else if(sigma2[t]>var_bounds[2*t+1]){
            if(!isinf(sigma2[t])){
                sigma2[t]=var_bounds[2*t+1]+log(sigma2[t]/var_bounds[2*t+1]);
            }else{
                sigma2[t]=var_bounds[2*t+1]+1000;
            }
        }
    }
    return sigma2;
}
/**
这里的tau是整个序列的长度
**/
double *ewma_recursion(double lam,double *input,double *sigma2,int tau,double backcast){
    double *var_bounds=new double[tau*2];
    double *fresides=new double[tau];
    double parameters[3]={0.0,1.0-lam,lam};
    for(int i=0;i<tau;i++){
        var_bounds[2*i]=-1.0;
        var_bounds[2*i+1]=1.7e308;
        fresides[i]=pow(input[i],2.0);
    }
    
    garch_recurion(parameters,fresides,input,sigma2,1,0,1,tau,backcast,var_bounds);
    
    delete[] var_bounds;
    delete[] fresides;
    return sigma2;
}
double* compute_variance_bound(double *input,double power,int size){
    double tau=75<=size?75:size;
    std::vector<double> w(size);
    double sum=0.0;
    for(int i=0;i<tau;i++){
        w.at(i)=pow(0.94,i);
        sum+=w.at(i);
    }
    double initial_value=0.0;
    for(int i=0;i<tau;i++){
        initial_value+=w.at(i)*(pow(input[i],2.0));
    }
    initial_value/=sum;
    double *var_bound=new double[size];
    memset(var_bound,0,sizeof(double)*size);
    ewma_recursion(0.94,input,var_bound,size,initial_value);

    double *var_bounds=new double[2*size];
    for(int i=0;i<size;i++){
        var_bounds[2*i]=var_bound[i]/1e6;
        var_bounds[2*i+1]=var_bound[i]*1e6;
    }
    //----------------------------------------------------
    //不是使用一轮计算方差的方式，因为担心会溢出
    double variance=0.0;
    double mean=0.0;
    for(int i=0;i<size;i++){
        mean+=input[i];
    }
    mean/=size;
    for(int i=0;i<size;i++){
        variance+=(input[i]-mean)*(input[i]-mean);
    }
    variance/=size;
    //------------------------------------------------------
    double min_upper_bound=0.0;
    for(int i=0;i<size;i++){
        if(min_upper_bound<(input[i]*input[i]))
            min_upper_bound=input[i]*input[i];
    }
    min_upper_bound+=1;
    double lower_bound=variance/1e8;
    double upper_bound=1e7*min_upper_bound;
    for(int i=0;i<size;i++){
        var_bounds[2*i]=var_bounds[2*i]<lower_bound?lower_bound:var_bounds[2*i];
        var_bounds[2*i+1]=(var_bounds[2*i+1]<min_upper_bound)?min_upper_bound:(var_bounds[2*i+1]>upper_bound?upper_bound:var_bounds[2*i+1]);
    }
    if(power!=2.0){
        for(int i=0;i<2*size;i++){
            var_bounds[i]=pow(var_bounds[i],power/2.0);
        }
    }
    delete[] var_bound;
    return var_bounds;
}
double *compute_variance(double *parameters,double *resids,double *sigma2,double backcast,double *var_bounds,int p,int o,int q,double power,int size){
    double *fresids=new double[size];
    double *sresids=new double[size];
    for(int i=0;i<size;i++){
        fresids[i]=pow(fabs(resids[i]),power);
        sresids[i]=resids[i]>0.0?1.0:(resids[i]==0.0?0.0:-1.0);
    }
    garch_recurion(parameters,fresids,sresids,sigma2,p,o,q,size,backcast,var_bounds);
    double inv_power=2.0/power;
    for(int i=0;i<size;i++){
        sigma2[i]=pow(sigma2[i],inv_power);
    }
    return sigma2;
}
double compute_normal_loglikelihood(double *resids,double *sigma2,int size){
    double lls=0.0;
    for(int i=0;i<size;i++){
        lls+=-0.5*(log(2*pi)+log(sigma2[i])+pow(resids[i],2.0)/sigma2[i]);
    }
    return lls;
}
double compute_gaussian_loglikelihood(double *parameters,double *input,double backcast,double *var_bounds,int p,int o,int q,double power,int size){
    double *sigma2=new double[size];
    //memset(sigma2,sizeof(double)*size,0);
    memset(sigma2,0,sizeof(double)*size);
    compute_variance(parameters,input,sigma2,backcast,var_bounds,p,o,q,power,size);
    return compute_normal_loglikelihood(input,sigma2,size);
}
double* starting_values(double *input,int p,int o,int q,double power,int size){
    double alphas[]={.01, .05, .1, .2};
    double gammas[]={.01, .05, .1, .2};
    double abgs[]={.5, .7, .9, .98};
    /**
        np.mean(abs(resids) ** power)
        scale = np.mean(resids ** 2) / (target ** (2.0 / power))
        target *= (scale ** power)
    **/
    double target =0.0;
    for(int i=0;i<size;i++){
        target+=pow(fabs(input[i]),power);
    }
    target/=size;
    double scale=0.0;
    for(int i=0;i<size;i++){
        scale+=pow(input[i],2.0);
    }
    scale/=size;
    scale=scale/target;
    target*=(pow(scale,power));

    double backcast=garch_backcast(input,power,size);
    double *var_bounds=compute_variance_bound(input,power,size);
    // printf("%lf %lf",var_bounds[0],var_bounds[1]);
    double *sv=new double[1+p+o+q];
    double *sv_tmp=new double[1+p+o+q];
    double max_llfs=-1e99;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            for(int k=0;k<4;k++){
                double alpha=alphas[i];
                double gamma=gammas[j];
                double abg=abgs[k];
                
                sv_tmp[0]=(1.0-abg) * target;
                for(int m=0;m<p;m++){
                    sv_tmp[1+m]=alpha/p;
                    abg-=alpha;
                }
                for(int m=0;m<o;m++){
                    sv_tmp[1+p+m]=gamma/o;
                    abg-=gamma/2.0;
                }
                for(int m=0;m<q;m++){
                    sv_tmp[1+p+o+m]=abg/q;
                }
                double llfs=compute_gaussian_loglikelihood(sv_tmp,input,backcast,var_bounds,p,o,q,power,size);
                if(llfs>max_llfs){
                    max_llfs=llfs;
                    memcpy(sv,sv_tmp,(1+p+o+q)*sizeof(double));
                }

            }
        }
    }
    delete []sv_tmp;
    delete []var_bounds;
    return sv;


}
void fit(double *input,int p,int o,int q,int power,int size){
    double *sigma2=new double[size];
    memset(sigma2,0,sizeof(double)*size);
    double backcast=garch_backcast(input,power,size);
    double *sv_volatility=starting_values(input,p,o,q,power,size);
    double *var_bounds=compute_variance_bound(input,power,size);
    double *std_resids=new double[size];
    for(int i=0;i<size;i++){
        std_resids[i]=input[i]/sqrt(sigma2[i]);
    }
}

int main(int argc,char **argv)
{
    double input[]={0.945532630498276,
              0.614772790142383,
              0.834417758890680,
              0.862344782601800,
              0.555858715401929,
              0.641058419842652,
              0.720118656981704,
              0.643948007732270,
              0.138790608092353,
              0.279264178231250,
              0.993836948076485,
              0.531967023876420,
              0.964455754192395,
              0.873171802181126,
              0.937828816793698};
    for(int i=0;i<15;i++){
        input[i]-=0.702491126236;
    }
    //double *backcast=compute_variance_bound(input,2.0,15);
    //starting_values(input,1,0,1,2.0,15);
    fit(input,1,0,1,2.0,15);
    //printf("%lf\n",backcast);
    return 0;
}