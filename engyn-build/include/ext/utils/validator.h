#include "ext/json/json.hpp"
#include "ext/json/json-schema.hpp"

#include <fstream>
#include <iostream>

using nlohmann::json;
using nlohmann::json_uri;
using nlohmann::json_schema_draft4::json_validator;

using json = nlohmann::json;


static void loader(const json_uri &uri, json &schema)
{
  std::fstream lf("." + uri.path());
  if (!lf.good())
    throw std::invalid_argument("could not open " + uri.url() + " tried with " + uri.path());

  try {
    lf >> schema;
  } catch (std::exception &e) {
    throw e;
  }
}

class awiros_validator {

  json m_schema;
  //json_validator m_validator(this->loader,   [](const std::string &, const std::string &){});
  json_validator m_validator;

  static void loader(const json_uri &uri, json &schema)
  {
    std::fstream lf("." + uri.path());
    if (!lf.good())
      throw std::invalid_argument("could not open " + uri.url() + " tried with " + uri.path());

    try {
      lf >> schema;
    } catch (std::exception &e) {
      throw e;
    }
  };

  public:
  awiros_validator(std::string filename){
    this->set_schema(filename);
  };

  int set_schema(std::string filename){
    std::ifstream inputfilestream(filename);
    if (!inputfilestream.good()){
      std::cerr << "awiros_validator: could not open schema file " << filename << "\n";
    }
    try {
      inputfilestream >> m_schema;
    }  catch (std::exception &e) {
      std::cerr << "awiros_validator: " << e.what() << " at " << inputfilestream.tellg() << " - while parsing the schema \n";
      return EXIT_FAILURE;
    }

    try {
      m_validator.set_root_schema(this->m_schema);
    } catch (std::exception &e) {
      std::cerr << "awiros_validator: setting root schema failed\n";
      std::cerr << e.what() << "\n";
    }
    return 1;
  };

  bool validate_json(json ads){
   try {
     m_validator.validate(ads);
   } catch (std::exception &e) {

      std::cerr << "awiros_validator: validating the json document failed\n";
      std::cerr << e.what() <<  " at offset: " <<   " "   << "\n";
      return false;
   }
   return true;
  };
};
