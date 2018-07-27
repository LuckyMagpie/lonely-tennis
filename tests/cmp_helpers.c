#include <math.h>
#include <stdbool.h>

#include "cmp_helpers.h"

bool array_eq(float* arr1, float* arr2, int size)
{
    for (int i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }

    return true;
}

bool model_matrix_eq(float* model_matrix1, float* model_matrix2)
{
    for (int i = 0; i < 8; i++) {
        if (!(fabs(model_matrix1[i] - model_matrix2[i]) <= 0.0001)) {
            return false;
        }
    }

    return true;
}
