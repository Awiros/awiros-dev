#ifndef _TRIGGER_H_
#define _TRIGGER_H_

#include <map>
#include <iostream>
#include "types.h"
#include "../ext/json/json.hpp"
#include <mutex>


using namespace nlohmann;

typedef struct awiros_app_data{
  //awiros_app_state state = CONTINUE;
  awiros_app_state state = STOP;
  json payload;
} awiros_app_data;

extern std::map<int, awiros_app_data> app_registry;
extern std::mutex registry_mutex;
extern std::mutex id_mutex;
extern int g_app_id;

awiros_app_state get_desired_app_state(int app_id);
void set_desired_app_state(int app_id, awiros_app_state state);
void add_app(int app_id);
void remove_app(int app_id);
json get_new_acs(int app_id);
void set_new_acs(int app_id, json payload);
void print_app_registry_info();

#endif // _TRIGGER_H_
