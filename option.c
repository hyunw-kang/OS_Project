#include "option.h"

bool option(char* option) {
	if (option[0] == '-') {
		return true;
	}
	else {
		return false;
	}
}