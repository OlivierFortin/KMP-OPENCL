__kernel void sum(__global const float *a_g, __global const float *b_g, __global float *res_g) 
{
  int gid = get_global_id(0);
  res_g[gid] = gid;

  printf("Device ID : %d \n", get_global_id(0)); 	
}

__kernel void find_pattern(int AlphabetSize,
						 int inputL,
						 int  h_input_length,
						 int h_automate_length,
						__global int *h_input,
						__global int *h_automate,
						__global int *h_failureTable){
	int gid = get_global_id(0);
	int m = gid*inputL;//position in input
	int i = 0;//position in automate
	int counter = 0;
	 while (m+i < h_input_length && counter < inputL+h_automate_length) {
	 	if(h_automate[i] == h_input[m+i]) {
	 		if (i == h_automate_length - 1 ) {
	 			printf("Found pattern at index %d \n", m);
	 			return;
	 		}
	 		++i;
	 	} else {
	 		if (h_failureTable[i] > -1) {
	 			m = m + i - h_failureTable[i];
	 			i = h_failureTable[i];
	 		} else {
	 			i = 0;
	 			++m;
	 			++counter;
	 		}
	 	}
	 }
	 return;//No pattern found
}

/*
algorithm kmp_search:
    input:
        an array of characters, S (the text to be searched)
        an array of characters, W (the word sought)
    output:
        an integer (the zero-based position in S at which W is found)

    define variables:
        an integer, m ← 0 (the beginning of the current match in S)
        an integer, i ← 0 (the position of the current character in W)
        an array of integers, T (the table, computed elsewhere)

    while m + i < length(S) do
        if W[i] = S[m + i] then
            if i = length(W) - 1 then
                return m
            let i ← i + 1
        else
            if T[i] > -1 then
                let m ← m + i - T[i], i ← T[i]
            else
                let i ← 0, m ← m + 1
            
    (if we reach here, we have searched all of S unsuccessfully)
    return the length of S
 */