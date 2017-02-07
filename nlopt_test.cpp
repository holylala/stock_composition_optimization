// #include <nlopt.h>
// #include <gsl/gsl_blas.h>
// #include <gsl/gsl_cblas.h>
// #include <iostream>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <math.h>

// using namespace std;
// double sum_constrained(unsigned int np,const double *x,double *grad,void* data){
//     double sum=-1.0;
//     if(grad!=NULL){
//             for(int i=0;i<np;i++){
//                 grad[i]=1.0;
//             }
//     }
//     for(int i=0;i<np;i++){
//         sum+=x[i];
//     }
//     return sum;
// }
// struct earning_data{
//     double *p;
//     double earning;
// };
// double earning_constraint(unsigned int np,const double *x,double *grad,void *data){
//     earning_data* e_d=(earning_data *)data;
//     if(grad!=NULL){
//         for(int i=0;i<np;i++){
//             grad[i]=e_d->p[i];
//         }
//     }
//     double c[] = {0.00};
//     cblas_dgemm (CblasRowMajor, CblasNoTrans, CblasNoTrans,1,1,np,
//                   1.0, x, np,e_d->p,1,
//                   0.0, c,1);
//     return c[0]-e_d->earning;
// }
// double earning_func(unsigned int np,const double *x,void *data){
//     earning_data* e_d=(earning_data *)data;
//     double c[] = {0.00};
//     cblas_dgemm (CblasRowMajor, CblasNoTrans, CblasNoTrans,1,1,np,
//                   1.0, x, np,e_d->p,1,
//                   0.0, c,1);
//     return c[0];
// }
// struct conv_data{
//     double *conv;
// };
//  double conv_obj(unsigned int np,const double *x,double *grad,void *data){
//         conv_data *c_d=(conv_data *)data;
//         if(grad!=NULL){
//             //double *grad_=new double[np];
//             memset(grad,np*sizeof(double),0);
//             //grad_=np.dot(conv,np.asmatrix(x).T)
//             cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,
//             np,1,np,2.0,c_d->conv,np,x,1,0.0,grad,1);
//         }
//         double *middle=new double[np];
//         memset(middle,np*sizeof(double),0);
//         cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,1,np,np,
//         1.0,x,np,c_d->conv,np,0.0,middle,np);
//         double risk[]={0.0};
//         cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,1,1,np,1.0,middle,np,x,1,0.0,risk,1);
//         delete []middle;
//         return risk[0];
//  }
//  double conv_func(unsigned int np,const double *x,void *data){
//      conv_data *c_d=(conv_data *)data;
//      double *middle=new double[np];
//     memset(middle,np*sizeof(double),0);
//     cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,1,np,np,
//     1.0,x,np,c_d->conv,np,0.0,middle,np);
//     double risk[]={0.0};
//     cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,1,1,np,1.0,middle,np,x,1,0.0,risk,1);
//     delete []middle;
//     return risk[0];
//  }
                
// int main(int argc,char *arg[]){
//     int np=10;
//     int number=188;
//     FILE *fp=fopen("D:\\workspace4python\\FirstTest\\weekcplusplus.txt","rb");
//     double *week_data=new double[number*np];
//     fread(week_data,number*np,sizeof(double),fp);
//     fclose(fp);
//     double *mean=new double[np];
//     double *variance=new double[np];
//     for(int i=0;i<np;i++){
//         mean[i]=0.0;
//         variance[i]=0.0;
//         for(int j=0;j<number;j++){
//             mean[i]+=week_data[i*number+j];
//             variance[i]+=week_data[i*number+j]*week_data[i*number+j];
//         }
//         mean[i]/=number;
//         variance[i]-=number*mean[i]*mean[i];
//         variance[i]=sqrt(variance[i]);
//     }
//     double *week_data_transpose=new double[np*number];
//     for(int i=0;i<number;i++){
//         for(int j=0;j<np;j++){
//             week_data_transpose[i*10+j]=week_data[j*number+i];
//         }
//     }
//     double *c=new double[np*np];
//     memset(c,0,np*np*sizeof(double));
//     cblas_dgemm(CblasRowMajor, 
//                CblasNoTrans, CblasNoTrans, np, np, number,
//                1.0, week_data, number, week_data_transpose, np, 0.0, c, np);
//     free(week_data);
//     free(week_data_transpose);
//     //我们做的是无偏估计，因此是除以186
//     for(int i=0;i<np;i++){
//         for(int j=0;j<np;j++){
//                 c[i*np+j]=(c[i*np+j]-number*mean[i]*mean[j])/(number-1);
//         }
//     }

//     double x[]={0.05,0.05,0.4,0.05,0.05,0.05,0.05,0.05,0.2,0.05};
    
//     double *grad=NULL;
//     double sum=sum_constrained(np,x,grad,NULL);
//     // double earning=earning_constraint(x,grad,mean,0.00004,np);
//     cout<<sum<<std::endl;
//     // cout<<earning<<std::endl;
//     cout<<"test"<<endl;

//     double min_e=0.000238442561632;
//     double max_e=0.00150711235001;
//     FILE *earning_result_file=fopen("earning_result.txt","wb");
//     FILE *risk_result_file=fopen("risk_result.txt","wb");
//     for(int iter=0;iter<1000;iter++){
//         double *start_pos=new double[np];
//         for(int i=0;i<np;i++){
//             start_pos[i]=x[i];
//         }
//         double earning_o=min_e+(iter+1)*(max_e-min_e)/100;
//         nlopt_opt opt=nlopt_create(NLOPT_AUGLAG,np);
//         nlopt_opt opt_local=nlopt_create(NLOPT_LD_LBFGS,np);
//         nlopt_set_local_optimizer(opt,opt_local);
//         double *lower_bounds=new double[np];
//         double *upper_bounds=new double[np];
//         for(int i=0;i<np;i++){
//             lower_bounds[i]=0.05;
//             upper_bounds[i]=0.4;
//         }
//         nlopt_set_lower_bounds(opt,lower_bounds);
//         nlopt_set_upper_bounds(opt,upper_bounds);
//                 earning_data e_d;
//         e_d.p=mean;
//         e_d.earning=earning_o;
//         nlopt_add_equality_constraint(opt,earning_constraint,&e_d,1e-16);

//         nlopt_add_equality_constraint(opt,
//                                 sum_constrained,
//                                 NULL,
//                                 1e-8);

//         conv_data c_d;
//         c_d.conv=c;
//         nlopt_set_min_objective(opt, conv_obj, 
//         					  &c_d);
//         nlopt_set_xtol_rel(opt, 1e-4);
//         nlopt_set_maxtime(opt,0.05);
//         double y=0.0;
//         nlopt_optimize(opt, start_pos,
//                         &y);
//         cout<<iter<<" iteration :"<<endl;
//         for(int i=0;i<np;i++){
//             cout<<start_pos[i]<<" ";
//         }
//         cout<<endl;
//         double real_earning=earning_func(np,start_pos,&e_d);
//         cout<<"real earning:"<<real_earning;
//         fwrite(&real_earning,sizeof(double),1,earning_result_file);
//         double real_conv=conv_func(np,start_pos,&c_d);
//         cout<<"real risk:"<<real_conv;
//         fwrite(&real_conv,sizeof(double),1,risk_result_file);
//         cout<<"sum :"<<sum_constrained(np,start_pos,NULL,NULL)<<endl;
//         delete []start_pos;
//         cout<<"------------------------------------------------------------"<<endl;
//     }
//     fclose(earning_result_file);
//     fclose(risk_result_file);
//     free(mean);
//     free(variance);
//     free(c);
//     cout<<"optimization end"<<endl;


// }