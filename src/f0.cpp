#include <Rcpp.h>
using namespace Rcpp;

//' @export
// [[Rcpp::export]]

NumericVector f0cpp(NumericVector ii, NumericVector dd, int nrows) {
  NumericVector rr(nrows);
  for(int i=0;i<nrows;i++)  {     // question: is this done automatically?
    rr[i] = 0;   
  }
  for(int i=0;i<ii.size();i++)  {
    rr[ ii[i] ] = rr[ ii[i] ] + dd[i];
  }
  return rr;
}
