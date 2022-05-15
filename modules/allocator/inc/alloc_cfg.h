#pragma once


#define MEM_TRACE

#ifdef MEM_TRACE
	
	#define MEM_WRAP
	
	#ifdef MEM_WRAP
		#define WRAP_LEN 64 // bytes
	#endif 

#endif 


//#define MEM_ZEROING
