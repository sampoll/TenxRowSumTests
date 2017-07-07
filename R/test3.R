# R function to process one chunk - wraps C++ function
run.test3 <- function(chunk_size, nchunks)  {
  H5close()    # close any open file handles

  fn <- "/Users/spollack/Downloads/1M_neurons_filtered_gene_bc_matrices_h5.h5"
  h5f <- H5Fopen(fn)
  nrows <- h5read(h5f, "mm10/shape")[1]
  df <- h5ls(h5f)   # data frame with description of 
  ndata <- as.numeric(df[df$group == "/mm10" & df$name == "indices","dim"])
  H5close()    # close any open file handles

  row.sums.3 <- rep(0,nrows)
  for (i in 1:nchunks)  {
    i0 <- 1+(i-1)*chunk_size
    i1 <- i*chunk_size
    res <- .Call("f0c", as.integer(i0), as.integer(i1), as.integer(nrows))
    row.sums.3 = row.sums.3 + res
  }
  return(row.sums.3)
}

