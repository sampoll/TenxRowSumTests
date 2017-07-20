#include <stdlib.h>
#include "hdf5.h"
/* #define FILE "/Users/spollack/Downloads/1M_neurons_filtered_gene_bc_matrices_h5.h5" */
      
int c_getchunk(int *chunk_offset_p, int *chunk_count_p, int *indices_out, int *data_out, 
  const char *fname)  {
    
   int chunk_offset = *chunk_offset_p;
   int chunk_count = *chunk_count_p;

   hid_t       file, dataset, memspace, dataspace; 

   hsize_t dims_data_d[1], dims_data_i[1];
   hsize_t dims_mem[1];
   hsize_t offset_data[1], offset_mem[1], count_data[1], count_mem[1];

   /* unsigned *data_out = (unsigned *)malloc(chunk_count*sizeof(unsigned));     */
   /* unsigned *indices_out = (unsigned *)malloc(chunk_count*sizeof(unsigned));  */

   herr_t status;

   file = H5Fopen(fname, H5F_ACC_RDONLY, H5P_DEFAULT);

   /* Get chunk from indices */

   dims_data_d[0] = dims_data_i[0] = 2624828308ULL;

   /* ----- get indices ----- */

   dataset = H5Dopen2(file, "/mm10/indices", H5P_DEFAULT);
   dataspace = H5Dget_space(dataset);

   /* data hyperslab */
   offset_data[0] = chunk_offset;
   count_data[0] = chunk_count;
   status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset_data, NULL, count_data, NULL);

   /* memory hyperslab */
   dims_mem[0] = chunk_count;
   memspace = H5Screate_simple(1, dims_mem, NULL);
   offset_mem[0] = 0; 
   count_mem[0] = chunk_count;
   status = H5Sselect_hyperslab(memspace, H5S_SELECT_SET, offset_mem, NULL, count_mem, NULL);

   /* read */
   status = H5Dread(dataset, H5T_NATIVE_INT, memspace, dataspace, H5P_DEFAULT, indices_out);

   /* ----- get data ----- */
   dataset = H5Dopen2(file, "/mm10/data", H5P_DEFAULT);
   dataspace = H5Dget_space(dataset);
   status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset_data, NULL, count_data, NULL);
   memspace = H5Screate_simple(1, dims_mem, NULL);
   status = H5Sselect_hyperslab(memspace, H5S_SELECT_SET, offset_mem, NULL, count_mem, NULL);
   status = H5Dread(dataset, H5T_NATIVE_INT, memspace, dataspace, H5P_DEFAULT, data_out);

   /* show data */
   /*
   int i;
   for(i=0;i<chunk_count;i++)  {
     printf("%d     %d\n", indices_out[i], data_out[i]);
   }
   */

   /* clean up */
   status = H5Dclose(dataset);
   status = H5Fclose(file);

   return 1;

}

