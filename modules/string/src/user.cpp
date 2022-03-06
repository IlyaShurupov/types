
#include "strings.h"

#include "strdata.h"

str_user::str_user() {
	datap = new str_data(this, NULL, true);
	refinc(datap);
}

str_user::str_user(const char* in) {
	datap = new str_data(this, in, true);
	refinc(datap);
}

str_user::str_user(const str_user& in) {
	datap = in.datap;
	refinc(datap);
}

str_user::~str_user() {
	refdec(datap);
}


void str_user::refinc(str_data* dp) {
	dp->refc++;
}

void str_user::refdec(str_data* dp) {
	dp->refc--;
	if (!dp->refc) {
		delete datap;
	}
}

void str_user::assert_modifiable() {
	// if have no rights to modify create new copy of string data
	if (!datap->modifiable(this)) {
		datap = new str_data(*datap);
	}
}
