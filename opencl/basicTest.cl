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
	 
	//const
	int currentState = 0;
	int nextState = 0;
	int id = get_global_id(0);
	int currentPosition = 0;
	__local int local_input [128];
	int i = 0;

	 	
	while((i< inputL) && (nextState>= 0)){
		currentPosition = (id)*inputL+i;
		nextState = h_automate[currentPosition];
		printf("Device ID : %d , position : %d, value : %d \n", id,currentPosition,h_automate[currentPosition]);
		++i;
	}
}