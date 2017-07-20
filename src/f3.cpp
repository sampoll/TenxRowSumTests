#include <Rcpp.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include "hdf5.h"

extern "C"  {
#include "c_getchunk.h"
}

using namespace Rcpp;
using namespace std;

//' @export
// [[Rcpp::export]]

NumericVector f3cpp(int i0, int i1, int nrows, StringVector fnamev) {

  int offset = i0 - 1;
  int nele = i1 - offset;

  int *ii = (int *)malloc(nele*sizeof(int));
  int *dd = (int *)malloc(nele*sizeof(int));

  vector<string> vstrings(fnamev.size());
  string s = Rcpp::as< string >(fnamev(0));

  const char *fname = s.c_str();

  NumericVector rr(nrows);

  int status = c_getchunk(&offset, &nele, ii, dd, fname);
  for(int i=0;i<nrows;i++)  {     // question: is this done automatically?
    rr[i] = 0;   
  }
  for(int i=0;i<nele;i++)  {
    rr[ ii[i] ] = rr[ ii[i] ] + dd[i];
  }

  free(ii);
  free(dd);
  return rr;
}
