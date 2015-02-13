__kernel void sum(__global const float *a_g, __global const float *b_g, __global float *res_g) 
{
  int gid = get_global_id(0);
  res_g[gid] = gid;

  printf("Device ID : %d \n", get_global_id(0)); 	
}

__kernel void CheckMatch(__global int *h_input){
	int id = get_global_id(0);

	printf("Device ID : %d , input : %d \r\n", get_global_id(0),h_input[get_global_id(0)]);
}