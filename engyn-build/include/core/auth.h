#ifndef _AUTH_H_
#define _AUTH_H_

#include "term.h"
#include "consts.h"


auth_ret_t check_validity_key(std::string vk);
std::string compute_validity_key(int year, int mon, int mday, int hour, int min);


#endif //ifndef _AUTH_H_
