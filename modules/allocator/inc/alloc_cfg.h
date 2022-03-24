#pragma once


//#define MEM_TRACE

#ifdef MEM_TRACE
	
	#define MEM_WRAP
	
	#ifdef MEM_WRAP
		#define WRAP_LEN 8 // bytes
		#define WRAP_FILL_VAL 3
	#endif 

#endif 


//#define MEM_ZEROING
