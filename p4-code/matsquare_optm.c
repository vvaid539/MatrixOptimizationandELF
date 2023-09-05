// optimized versions of matrix diagonal summing
#include "matvec.h"

int matsquare_VER1(matrix_t *mat, matrix_t *matsq) {
  matrix_t t = *mat;
  matrix_t t2 = *matsq;
  for(int i=0; i< t.rows; i++){
    for(int k=0; k< t2.rows; k++){
      MSET(t2, i, k, 0);
    }
  }
  for(int i=0; i< t.rows; i++){
    for(int k=0; k< t.rows; k++){
      int mik = MGET(t, i, k);
      for(int j=0; j< t.cols; j++){
        int mkj = MGET(t, k, j);
        int cur = MGET(t2, i, j);
        int new = cur + mik*mkj;
        MSET(t2, i, j, new);
      }
    }
  }
  return 0;   
}

int matsquare_VER2(matrix_t *mat, matrix_t *matsq) {
  for(int i=0; i<mat->rows; i++){
    for(int j=0; j<mat->cols; j++){
      mset(matsq,i,j,0);                          // initialize to 0s
      for(int k=0; k<mat->rows; k++){
        int mik = mget(mat, i, k);
        int mkj = mget(mat, k, j);
        int cur = mget(matsq, i, j);
        int new = cur + mik*mkj;
        mset(matsq, i, j, new);
      }
    }
  }
  return 0;
}


int matsquare_OPTM(matrix_t *mat, matrix_t *matsq){
  if(mat->rows != mat->cols   ||      // must be a square matrix to square it
     mat->rows != matsq->rows ||
     mat->cols != matsq->cols)
  {
    printf("matsquare_OPTM: dimension mismatch\n");
    return 1;
  }

  // Call to some version of optimized code
  return matsquare_VER1(mat, matsq);
}
/////////////////////////////////////////////////////////////////////////////////
// ADDITIONAL INFORMATION
//
// (A) VERSION: If you implemented several versions, indicate which
// version you timed
// 
// ####################### YOUR ANSWER HERE #########################
// Version 1
// ##################################################################
// 
//
// (B) TIMING ON loginNN.cselabs.umn.edu:
// Paste a copy of the results of running matsquare_benchmark on the
// above machines in the space below which shows how your performance
// optimizations improved on the baseline codes.
// 
// ####################### YOUR ANSWER HERE #########################
// ==== Matrix Square Benchmark Version 2 ====
//   SIZE       BASE       OPTM  SPDUP   LOG2  SCALE POINTS 
//    256 3.9765e-01 6.3837e-02   6.23   2.64   0.94   2.47 
//    273 4.1042e-01 7.8133e-02   5.25   2.39   1.00   2.39 
//    512 3.7882e+00 5.0551e-01   7.49   2.91   1.88   5.45 
//    801 1.3987e+01 1.9909e+00   7.03   2.81   2.93   8.25 
//   1024 4.6870e+01 4.2649e+00  10.99   3.46   3.75  12.97 
//   RAW POINTS: 31.54
//   TOTAL POINTS: 30 / 30
//
// ##################################################################
// 
// (C) OPTIMIZATIONS:
// Describe in some detail the optimizations you used to speed the code
// up.  THE CODE SHOULD CONTAIN SOME COMMENTS already to describe these
// but in the section below, describe in English the techniques you used
// to make the code run faster.  Format your descriptions into discrete
// chunks such as.
// 
// Optimization 1: Blah bla blah... This should make run faster because
// yakkety yakeety yak.
// The first optimization that we did is setting a local variable to the two 
// pointer parameters. The reason for this is that the local variables are going
// to be closer for the CPU to access that memory addresses that are in main memory. 
// Thus, this will speed up the process.

// Optimization 2: Blah bla blah... This should make run faster because
// yakkety yakeety yak.
// The second optimization that I did was set all the values of the second 
// parameter to outside of the three for loops, into their own double for loop
// This is better as it happens simultaneously so the memory accessed is closeby
// instead of in the long for loop where many different things are accessed and the cache
// is constantly changed

// Optimization 3: Blah bla blah... This should make run faster because
// yakkety yakeety yak.
// Another optimization that I used was to change all the function calls to macros. Along 
// with the idea of having local variables tha fulfill the macro parameters, the macros have 
// a faster run time in comparison to the functions. Macros are preprocssed while functions are
// compiled which shows a time difference. 

// Full credit solutions will describe 2-3 optimizations and describe
// WHY these improved performance in at least a couple sentences.
// 
// ####################### YOUR ANSWER HERE #########################
// Optimization 1: Blah bla blah... This should make run faster because
// yakkety yakeety yak.
// The first optimization that we did is setting a local variable to the two 
// pointer parameters. The reason for this is that the local variables are going
// to be closer for the CPU to access that memory addresses that are in main memory. 
// Thus, this will speed up the process. 
// 

// Optimization 2: Blah bla blah... This should make run faster because
// yakkety yakeety yak.
// The second optimization that I did was set all the values of the second 
// parameter to outside of the three for loops, into their own double for loop
// This is better as it happens simultaneously so the memory accessed is closeby
// instead of in the long triple for loop where many different things are accessed and the cache
// is constantly changed

// Optimization 3: Blah bla blah... This should make run faster because
// yakkety yakeety yak.
// Another optimization that I used was to change all the function calls to macros. Along 
// with the idea of having local variables tha fulfill the macro parameters, the macros have 
// a faster run time in comparison to the functions. Macros are preprocssed while functions are
// compiled which shows a time difference. 
// ##################################################################
