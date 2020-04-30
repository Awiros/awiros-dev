#ifndef _TF_UTILS_H_
#define _TF_UTILS_H_

#ifdef ENABLE_TF

#include <fstream>
#include <string>
#include <chrono>
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/lib/core/status.h"

namespace tf_utils {

  tensorflow::Status LoadGraph(const std::string& graph_file_name, std::unique_ptr<tensorflow::Session>* session);
  std::pair<std::string, float> spitTopLabel(const std::vector<tensorflow::Tensor>& outputs, const std::string& labels_file_name);


}

#endif // ENABALE_TF

#endif // _TF_UTILS_H_
