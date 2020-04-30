#ifndef _HANDLE_ACS_H_
#define _HANDLE_ACS_H_

#include <nan.h>
#include <string>

#include "consts.h"
#include "../ext/json/json.hpp"
#include "conversion_layer.h"




void handle_acs_function(std::string &acs, std::string &ads, int func_name, const Nan::AsyncProgressQueueWorker<char>::ExecutionProgress &prg);


#endif// _HANDLE_ACS_H_
