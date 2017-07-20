# R function to process one chunk - wraps C++ function
f1b <- function(h, i0, i1, nrows, rs, fname)  {
  rr <- f3cpp(i0, i1, nrows, fname)
  rs <- rs + rr  
  return(rs)
}

run.test5 <- function(chunk_size, nchunks, fn)  {
  library(Rcpp)
  library(rhdf5)
  H5close()    # close any open file handles

  h5f <- H5Fopen(fn)
  nrows <- h5read(h5f, "mm10/shape")[1]
  df <- h5ls(h5f)   # data frame with description of 
  ndata <- as.numeric(df[df$group == "/mm10" & df$name == "indices","dim"])

  row.sums.2 <- rep(0,nrows)
  for (i in 1:nchunks)  {
    row.sums.2 = f1b(h5f, 1+(i-1)*chunk_size, i*chunk_size, nrows, row.sums.2, fn)
  }
  return(row.sums.2)
}

