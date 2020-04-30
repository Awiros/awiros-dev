#ifndef _EXT_API_H
#define _EXT_API_H

#include "../ext/json/json.hpp"
#include "types.h"
#include <chrono>

using namespace nlohmann;

typedef struct awiros_api_res{
  json body;
  json header;
  int status_code;
  float latency;
} api_res;


class awiros_ext_api{
  public:
    std::string url;
    int hit_count;

    awiros_ext_api();
    awiros_ext_api(std::string in_url);
    awiros_status get_request(awiros_api_res&, std::string);
    awiros_status get_multiple_requests(awiros_api_res&,
        std::vector<std::string>);

};
#endif // _EXT_API_H
