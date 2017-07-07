get.chunk <- function(offset, count)  {
  data = c();
  indices = c();
  res <- .Call("getchunk", as.integer(offset), as.integer(count))
  print(res)
}




