#ifndef _LIBSVM_H
#define _LIBSVM_H

#include "utilities.h"

enum { C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR };  /* svm_type */
enum { LINEAR, POLY, RBF, SIGMOID, PRECOMPUTED }; /* kernel_type */

struct SVMParameter
{
  int svm_type;
  int kernel_type;
  int degree;  /* for poly */
  double gamma;  /* for poly/rbf/sigmoid */
  double coef0;  /* for poly/sigmoid */

  /* these are for training only */
  double cache_size; /* in MB */
  double eps;  /* stopping criteria */
  double C;  /* for C_SVC, EPSILON_SVR and NU_SVR */
  int nr_weight;    /* for C_SVC */
  int *weight_label;  /* for C_SVC */
  double* weight;    /* for C_SVC */
  double nu;  /* for NU_SVC, ONE_CLASS, and NU_SVR */
  double p;  /* for EPSILON_SVR */
  int shrinking;  /* use the shrinking heuristics */
  int probability; /* do probability estimates */
};

//
// SVMModel
//
struct SVMModel
{
  struct SVMParameter param;  /* parameter */
  int nr_class;    /* number of classes, = 2 in regression/one class svm */
  int l;      /* total #SV */
  struct Node **SV;    /* SVs (SV[l]) */
  double **sv_coef;  /* coefficients for SVs in decision functions (sv_coef[k-1][l]) */
  double *rho;    /* constants in decision functions (rho[k*(k-1)/2]) */
  double *probA;    /* pariwise probability information */
  double *probB;
  int *sv_indices;        /* sv_indices[0,...,nSV-1] are values in [1,...,num_traning_data] to indicate SVs in the training set */

  /* for classification only */

  int *label;    /* label of each class (label[k]) */
  int *nSV;    /* number of SVs for each class (nSV[k]) */
        /* nSV[0] + nSV[1] + ... + nSV[k-1] = l */
  /* XXX */
  int free_sv;    /* 1 if SVMModel is created by svm_load_model*/
        /* 0 if SVMModel is created by svm_train */
};

struct SVMModel *svm_train(const struct Problem *prob, const struct SVMParameter *param);
void svm_cross_validation(const struct Problem *prob, const struct SVMParameter *param, int nr_fold, double *target);

int svm_save_model(const char *model_file_name, const struct SVMModel *model);
struct SVMModel *svm_load_model(const char *model_file_name);

int svm_get_svm_type(const struct SVMModel *model);
int svm_get_nr_class(const struct SVMModel *model);
void svm_get_labels(const struct SVMModel *model, int *label);
void svm_get_sv_indices(const struct SVMModel *model, int *sv_indices);
int svm_get_nr_sv(const struct SVMModel *model);
double svm_get_svr_probability(const struct SVMModel *model);

double svm_predict_values(const struct SVMModel *model, const struct Node *x, double* dec_values);
double svm_predict(const struct SVMModel *model, const struct Node *x);
double svm_predict_dec_values(const struct SVMModel *model, const struct Node *x, double **dec_values);
double svm_predict_probability(const struct SVMModel *model, const struct Node *x, double* prob_estimates);

void svm_free_model_content(struct SVMModel *model_ptr);
void svm_free_and_destroy_model(struct SVMModel **model_ptr_ptr);
void svm_destroy_param(struct SVMParameter *param);

const char *svm_check_parameter(const struct Problem *prob, const struct SVMParameter *param);
int svm_check_probability_model(const struct SVMModel *model);

void svm_set_print_string_function(void (*print_func)(const char *));

#endif /* _LIBSVM_H */
