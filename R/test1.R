# R function to process one chunk
f0 <- function(h, i0, i1, nrows, rs)  {
  ii <- h5read(h, "/mm10/indices", index=list(i0:i1))
  dd <- h5read(h, "/mm10/data", index=list(i0:i1))
  df <- data.frame(ii=ii,dd=dd)
  aa <- aggregate(df, list(as.factor(df$ii)), sum)
  aa$jj <- as.integer(as.character(aa$Group.1))+1    # rows are 0:ncols-1 in input file
  rs[aa$jj] <- rs[aa$jj] + aa$dd    
  return(rs)
}

run.test1 <- function(chunk_size, nchunks, fn)  {
  library(rhdf5)
  H5close()    # close any open file handles

  h5f <- H5Fopen(fn)
  nrows <- h5read(h5f, "mm10/shape")[1]
  df <- h5ls(h5f)   # data frame with description of 
  ndata <- as.numeric(df[df$group == "/mm10" & df$name == "indices","dim"])

  row.sums.1 <- rep(0,nrows)
  for (i in 1:nchunks)  {
    row.sums.1 = f0(h5f, 1+(i-1)*chunk_size, i*chunk_size, nrows, row.sums.1)
  }
  return(row.sums.1)
}

