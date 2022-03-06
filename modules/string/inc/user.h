

#include "common.h"

class str_user {

	class str_data* datap;

public:

	str_user();
	str_user(const char* in);
	str_user(const str_user& in);
	~str_user();

private:

	void refinc(str_data* dp);
	void refdec(str_data* dp);
	void assert_modifiable();


public: // editing interface	

};