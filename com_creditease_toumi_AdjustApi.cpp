// #include "com_creditease_toumi_AdjustApi.h"
// #include "package.h"
// #include <iostream>
// static Package *package;
// JNIEXPORT void JNICALL Java_com_creditease_toumi_AdjustApi_init
//   (JNIEnv *env, jobject object, jint np, jdoubleArray price, jdoubleArray w_walk_lb, jdoubleArray w_walk_ub, jdoubleArray revenue_weight, jdoubleArray risk_weight)
// {
//     package=new Package(np);
//     jboolean isCopy=true;
//     double *price_=(env)->GetDoubleArrayElements(price,&isCopy);
//     package->setPrice(price_);
//     double *w_walk_lb_=(env)->GetDoubleArrayElements(w_walk_lb,&isCopy);
//     package->setLb(w_walk_lb_);
//     double *w_walk_ub_=(env)->GetDoubleArrayElements(w_walk_ub,&isCopy);
//     package->setUb(w_walk_ub_);
//     double *revenue_weight_=(env)->GetDoubleArrayElements(revenue_weight,&isCopy);
//     package->setRevenue(revenue_weight_);
//     double *risk_weight_=(env)->GetDoubleArrayElements(risk_weight,&isCopy);
//     package->setRisk(risk_weight_);
//     env->ReleaseDoubleArrayElements(price,price_,JNI_ABORT);
//     env->ReleaseDoubleArrayElements(w_walk_lb,w_walk_lb_,JNI_ABORT);
//     env->ReleaseDoubleArrayElements(w_walk_ub,w_walk_ub_,JNI_ABORT);
//     env->ReleaseDoubleArrayElements(revenue_weight,revenue_weight_,JNI_ABORT);
//     env->ReleaseDoubleArrayElements(risk_weight,risk_weight_,JNI_ABORT);
// }
// JNIEXPORT void JNICALL Java_com_creditease_toumi_AdjustApi_reloadModel
//   (JNIEnv *env, jobject object, jint np, jdoubleArray price, jdoubleArray w_walk_lb, jdoubleArray w_walk_ub, jdoubleArray revenue_weight, jdoubleArray risk_weight)
// {
//     package->setNp(np);
//     jboolean isCopy=true;
//     double *price_=(env)->GetDoubleArrayElements(price,&isCopy);
//     package->setPrice(price_);
//     double *w_walk_lb_=(env)->GetDoubleArrayElements(w_walk_lb,&isCopy);
//     package->setLb(w_walk_lb_);
//     double *w_walk_ub_=(env)->GetDoubleArrayElements(w_walk_ub,&isCopy);
//     package->setUb(w_walk_ub_);
//     double *revenue_weight_=(env)->GetDoubleArrayElements(revenue_weight,&isCopy);
//     package->setRevenue(revenue_weight_);
//     double *risk_weight_=(env)->GetDoubleArrayElements(risk_weight,&isCopy);
//     package->setRisk(risk_weight_);
//     env->ReleaseDoubleArrayElements(price,price_,JNI_ABORT);
//     env->ReleaseDoubleArrayElements(w_walk_lb,w_walk_lb_,JNI_ABORT);
//     env->ReleaseDoubleArrayElements(w_walk_ub,w_walk_ub_,JNI_ABORT);
//     env->ReleaseDoubleArrayElements(revenue_weight,revenue_weight_,JNI_ABORT);
//     env->ReleaseDoubleArrayElements(risk_weight,risk_weight_,JNI_ABORT);
// }
// JNIEXPORT void JNICALL Java_com_creditease_toumi_AdjustApi_run
//   (JNIEnv *env, jobject object, jdouble assets, jdoubleArray w_original, jdoubleArray out_weight,jboolean verbose){
//     jboolean isCopy=false;
//     double *w_original_=(env)->GetDoubleArrayElements(w_original,&isCopy);
//     double *out_weight_=(env)->GetDoubleArrayElements(out_weight,&isCopy);
//     package->run(assets,w_original_,out_weight_,verbose);
//     env->ReleaseDoubleArrayElements(w_original,w_original_,0);
//     env->ReleaseDoubleArrayElements(out_weight,out_weight_,0);
// }