__kernel void find_pattern(int inputL,
						 int  h_input_length,
						 int h_automate_length,
						__global int *h_input,
						__global int *h_automate,
						__global int *h_failureTable,
						__global int *pattern_found){
	int gid = get_global_id(0);
	int m = gid*inputL;//position in input
	int i = 0;//position in automate
	int counter = 0;
	 while (*pattern_found != 1 & counter < inputL+h_automate_length  & m+i < h_input_length) {
	 	if(h_automate[i] == h_input[m+i]) {
	 		if (i == h_automate_length - 1 ) {
	 			*pattern_found = 1;
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
}

