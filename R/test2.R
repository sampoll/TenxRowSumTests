# R function to process one chunk - wraps C++ function
f1 <- function(h, i0, i1, nrows, rs)  {
  ii <- h5read(h, "/mm10/indices", index=list(i0:i1))
  dd <- h5read(h, "/mm10/data", index=list(i0:i1))
  rr <- f0cpp(ii, dd, nrows)
  rs <- rs + rr  
  return(rs)
}

run.test2 <- function(chunk_size, nchunks)  {
  library(Rcpp)
  library(rhdf5)
  # sourceCpp("../f0.cpp")
  H5close()    # close any open file handles

  fn <- "/Users/spollack/Downloads/1M_neurons_filtered_gene_bc_matrices_h5.h5"
  h5f <- H5Fopen(fn)
  nrows <- h5read(h5f, "mm10/shape")[1]
  df <- h5ls(h5f)   # data frame with description of 
  ndata <- as.numeric(df[df$group == "/mm10" & df$name == "indices","dim"])

  row.sums.2 <- rep(0,nrows)
  for (i in 1:nchunks)  {
    row.sums.2 = f1(h5f, 1+(i-1)*chunk_size, i*chunk_size, nrows, row.sums.2)
  }
  return(row.sums.2)
}

