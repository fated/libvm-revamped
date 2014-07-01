#ifndef LIBVM_KNN_H_
#define LIBVM_KNN_H_

#include "utilities.h"

struct KNNParameter {
  int num_neighbors;
};

struct KNNModel {
  struct KNNParameter param;
  int num_ex;
  int num_classes;
  int *labels;  // label of each class (label[k])
  double **dist_neighbors;
  int **label_neighbors;
};

template <typename T>
static inline void InsertLabel(T *labels, T label, int num_neighbors, int index) {
  for (int i = num_neighbors-1; i > index; --i)
    labels[i] = labels[i-1];
  labels[index] = label;

  return;
}

double PredictKNN(struct Problem *train, struct Node *x, const int num_neighbors);
double CalcDist(const struct Node *x1, const struct Node *x2);
int CompareDist(double *neighbors, double dist, int num_neighbors);

#endif  // LIBVM_KNN_H_
