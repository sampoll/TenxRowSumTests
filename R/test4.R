# R function to process one chunk - wraps C function
run.test4 <- function(chunk_size, nchunks, fn)  {
  H5close()    # close any open file handles

  h5f <- H5Fopen(fn)
  nrows <- h5read(h5f, "mm10/shape")[1]
  df <- h5ls(h5f)   # data frame with description of 
  ndata <- as.numeric(df[df$group == "/mm10" & df$name == "indices","dim"])
  H5close()    # close any open file handles

  row.sums.4 <- rep(0,nrows)
  for (i in 1:nchunks)  {
    i0 <- 1+(i-1)*chunk_size
    i1 <- i*chunk_size
    if ( i1 > ndata )  {
      i1 <- ndata
    }
    res <- .Call("f1c", as.integer(i0), as.integer(i1), as.integer(nrows))
    row.sums.4 = row.sums.4 + res
  }
  return(row.sums.4)
}

