#include <R.h>
#include <Rinternals.h>
#include <stdlib.h>
#include "c_getchunk.h"

SEXP f0c(SEXP i0_r, SEXP i1_r, SEXP nrows_r)   {

  /* extract data from Rvecs */
  int *i0_p = INTEGER(i0_r);
  int *i1_p = INTEGER(i1_r);
  int *nrows_p = INTEGER(nrows_r);
  int nrows = *nrows_p;

  /* convert to offset and count */
  int offset = (*i0_p-1);
  int count = (*i1_p) - offset;
  int *offset_p = &offset;
  int *count_p = &count;
  int n = count;

  /* allocate space */
  int *buf_indices = (int *)R_alloc(n, sizeof(int));
  int *buf_data = (int *)R_alloc(n, sizeof(int));
  int *buf_result = (int *)R_alloc(n, sizeof(nrows));

  c_getchunk(offset_p, count_p, buf_indices, buf_data);

  /* compute partial row sums */
  int i;
  for(i=0;i<nrows;i++)  {
    buf_result[i] = 0;
  }
  for(i=0;i<n;i++)  {
    buf_result[ buf_indices[i] ] += buf_data[i];
  }

  /* pack back into Rvec */
  SEXP result = Rf_allocVector(INTSXP, nrows);
  PROTECT(result);
  memcpy(INTEGER(result), buf_result, sizeof(int)*nrows);
  UNPROTECT(1);

  return(result);

}

// Same as f0, except use SEXP integer storage for result 
// instead of copying from locally allocate array
SEXP f1c(SEXP i0_r, SEXP i1_r, SEXP nrows_r)   {

  /* extract data from Rvecs */
  int *i0_p = INTEGER(i0_r);
  int *i1_p = INTEGER(i1_r);
  int *nrows_p = INTEGER(nrows_r);
  int nrows = *nrows_p;

  /* convert to offset and count */
  int offset = (*i0_p-1);
  int count = (*i1_p) - offset;
  int *offset_p = &offset;
  int *count_p = &count;
  int n = count;

  /* allocate space for indices and data */
  int *buf_indices = (int *)R_alloc(n, sizeof(int));
  int *buf_data = (int *)R_alloc(n, sizeof(int));

  /* allocate space for result */
  SEXP result = Rf_allocVector(INTSXP, nrows);
  int *rp = INTEGER(result);
  PROTECT(result);

  c_getchunk(offset_p, count_p, buf_indices, buf_data);

  /* compute partial row sums */
  int i;
  for(i=0;i<nrows;i++)  {
    rp[i] = 0;
  }
  for(i=0;i<n;i++)  {
    rp[ buf_indices[i] ] += buf_data[i];
  }

  UNPROTECT(1);
  return(result);

}



// For debugging
SEXP getchunk(SEXP offset, SEXP count)   {

  int *offset_p = INTEGER(offset);
  int *count_p = INTEGER(count);
  int n = *count_p;

  int *buf_indices = (int *)R_alloc(n, sizeof(int));
  int *buf_data = (int *)R_alloc(n, sizeof(int));

  int i;

  c_getchunk(offset_p, count_p, buf_indices, buf_data);

  printf("Buffers\n");
  for(i=0;i<n;i++)  {
    printf("%d  %d\n", buf_indices[i], buf_data[i]);
  }

  SEXP indices = Rf_allocVector(INTSXP, n);
  PROTECT(indices);
  memcpy(INTEGER(indices), buf_indices, sizeof(int)*n);

  SEXP data = Rf_allocVector(INTSXP, n);
  PROTECT(data);
  memcpy(INTEGER(data), buf_data, sizeof(int)*n);

  printf("R-vectors\n");
  for(i=0;i<n;i++)  {
    printf("%d  %d\n", INTEGER(indices)[i], INTEGER(data)[i]);
  }

  SEXP result = Rf_allocVector(VECSXP, 2);
  PROTECT(result);
  SET_VECTOR_ELT(result, 0, indices);
  SET_VECTOR_ELT(result, 1, data);
  UNPROTECT(3);

  return(result);

}
