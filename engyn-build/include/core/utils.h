#ifndef _UTILS_H_
#define _UTILS_H_

#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include "../ext/utils/base64.h"
#include "frame.h"
#include <chrono>

extern int g_log_level;
#define AWI_DEBUG(fmt, ...)                 if (g_log_level >= 2) fprintf(stderr, "DEBUG: " fmt "\n", ##__VA_ARGS__)
#define AWI_INFO(fmt, ...)                  if (g_log_level >= 1) fprintf(stderr, "INFO : " fmt "\n", ##__VA_ARGS__)
#define AWI_ERROR(fmt, ...)                 if (g_log_level >= 0) fprintf(stderr, "ERROR: " fmt "\n", ##__VA_ARGS__)

namespace awiros_utils {
  std::string get_date_string(std::chrono::system_clock::time_point t);
  std::size_t get_current_ms();
  std::string get_file_contents(std::string filename);
  int get_ethernet_interfaces(std::vector<std::string> &ifaces);

  std::string get_hwaddr_interface(const char * iface);
  std::size_t get_int(std::string str);

  std::string convert_to_base64(cv::Mat frame);

  std::vector<std::string> line_parser(std::string name, std::string symbol);
  int isSubstring(std::string s1, std::string s2);
  int mac_interface_checker(std::vector<std::string> int_loc, std::string r_int[]);
  int parsing_interface_file(std::string filename, std::string rej_interface[]);
  std::string get_ip_ethernet();
  static std::string ip_ethernet = "";

  int sign_func(int num);

  class awiros_clock{

    private:
      int64_t tick_count = 0;

    public:
      bool is_time_interval(int secs);
      void reset();
      void start();
      void stop();
      void duration();
  };
  class vote_strings{
    public:
      std::vector<std::string> consume;
      std::string yeild();
  
  };


}
#endif// _UTILS_H_
