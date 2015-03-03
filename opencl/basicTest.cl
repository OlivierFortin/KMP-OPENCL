__kernel void sum(__global const float *a_g, __global const float *b_g, __global float *res_g) 
{
  int gid = get_global_id(0);
  res_g[gid] = gid;

  printf("Device ID : %d \n", get_global_id(0)); 	
}

__kernel void CheckMatch(int AlphabetSize,
						 int inputL, 
						__global int *h_input,
						__global int *h_automate){
	 int m = 0;//position in input
	 int i = 0;//position in automate
	 while (m+i < length(h_input)) {
	 	if(h_automate[i] == S[m+i]) {
	 		if i = length(h_automate) - 1 
	 			return m;
	 	}
	 }
}