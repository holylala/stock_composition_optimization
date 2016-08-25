#include "com_creditease_toumi_AdjustApi.h"
#include "package.h"
#include <iostream>
static Package *package;
JNIEXPORT void JNICALL Java_com_creditease_toumi_AdjustApi_init
  (JNIEnv *env, jobject object, jint np, jdoubleArray price, jdoubleArray w_walk_lb, jdoubleArray w_walk_ub, jdoubleArray revenue_weight, jdoubleArray risk_weight)
{
    package=new Package(np);
    double *price_=(env)->GetDoubleArrayElements(price,JNI_FALSE);
    package->setPrice(price_);
    double *w_walk_lb_=(env)->GetDoubleArrayElements(w_walk_lb,JNI_FALSE);
    package->setLb(w_walk_lb_);
    double *w_walk_ub_=(env)->GetDoubleArrayElements(w_walk_ub,JNI_FALSE);
    package->setUb(w_walk_ub_);
    double *revenue_weight_=(env)->GetDoubleArrayElements(revenue_weight,JNI_FALSE);
    package->setRevenue(revenue_weight_);
    double *risk_weight_=(env)->GetDoubleArrayElements(risk_weight,JNI_FALSE);
    package->setRisk(risk_weight_);
}
JNIEXPORT void JNICALL Java_com_creditease_toumi_AdjustApi_reloadModel
  (JNIEnv *env, jobject object, jint np, jdoubleArray price, jdoubleArray w_walk_lb, jdoubleArray w_walk_ub, jdoubleArray revenue_weight, jdoubleArray risk_weight)
{
    package->setNp(np);
    double *price_=(env)->GetDoubleArrayElements(price,JNI_FALSE);
    package->setPrice(price_);
    double *w_walk_lb_=(env)->GetDoubleArrayElements(w_walk_lb,JNI_FALSE);
    package->setLb(w_walk_lb_);
    double *w_walk_ub_=(env)->GetDoubleArrayElements(w_walk_ub,JNI_FALSE);
    package->setUb(w_walk_ub_);
    double *revenue_weight_=(env)->GetDoubleArrayElements(revenue_weight,JNI_FALSE);
    package->setRevenue(revenue_weight_);
    double *risk_weight_=(env)->GetDoubleArrayElements(risk_weight,JNI_FALSE);
    package->setRisk(risk_weight_);
}
JNIEXPORT void JNICALL Java_com_creditease_toumi_AdjustApi_run
  (JNIEnv *env, jobject object, jdouble assets, jdoubleArray w_original, jdoubleArray out_weight){
    double *w_original_=(env)->GetDoubleArrayElements(w_original,JNI_FALSE);
    double *out_weight_=(env)->GetDoubleArrayElements(out_weight,JNI_FALSE);
    package->run(assets,w_original_,out_weight_);
    for(int i=0;i<package->getNp();i++){
      std::cout<<out_weight_[i]<<" ";
    }
    std::cout<<"\n";
    env->ReleaseDoubleArrayElements(out_weight,out_weight_,0);
}