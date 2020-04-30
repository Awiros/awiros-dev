#ifndef _TERM_HPP_
#define _TERM_HPP_

#include <string>
#include "types.h"
#include "utils.h"
#include <iostream>

//TODO: Take implementation to term.cpp


class awiros_term {
  aid_t         m_tid;
  aid_t         m_client_id;

  aid_t         m_valid_year;
  aid_t         m_valid_month;
  aid_t         m_valid_mday;
  aid_t         m_valid_hour;
  aid_t         m_valid_min;
  std::string   m_valid_key;

  std::size_t   m_n_eints;
  std::size_t   m_valid_until_hrs; //hrs since epoch

  public:

  //awiros_term();

  awiros_term(aid_t tid, aid_t client_id, std::size_t vu);
  std::vector<std::string>   ifaces;
  std::vector<std::string>   mac_ids;
  std::string   machine_id;
  std::string   serial_key;

  aid_t get_tid();
  void set_tid(aid_t tid);

  //std::string get_mac_id(){return m_mac_id;}
  void        get_my_mac_ids();
  void get_my_machine_id();
 
  void set_validity_time(aid_t year,
                         aid_t month,
                         aid_t mday,
                         aid_t hour,
                         aid_t min);

  void set_validity_key(std::string key);
  


  std::string get_validity_key();
  


  aid_t get_valid_year();

  aid_t get_valid_month();

  aid_t get_valid_mday();
	
  aid_t get_valid_hour();

  aid_t get_valid_min();
     
  
};


#endif //_TERM_HPP_
