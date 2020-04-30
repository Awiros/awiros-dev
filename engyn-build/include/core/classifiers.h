#ifndef _CLASSIFIER_H_
#define _CLASSIFIER_H_

#include "../interfaces/frameworks/tensorflow/tf_utils.h"

#include <utility>
#include <string>

#ifdef ENABLE_TF

#include "star.h"

class classifier : public awi_star{
    std::string m_app_name;


 public:
     //classifier(std::string app_name) : awi_star(app_name){};
     classifier(std::string app_name): awi_star(app_name){
         m_app_name = app_name;
     };
     std::pair<std::string, float>  spit_char(tensorflow::Tensor &image);

};
//std::pair<std::string, float> char_ocr(tensorflow::Tensor &image);
#endif
#endif
