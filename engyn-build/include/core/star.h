#ifndef _STAR_
#define _STAR_

#include "../interfaces/frameworks/tensorflow/tf_utils.h"

#ifdef ENABLE_TF

#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/lib/io/path.h"

class awi_star{
  public:
        awi_star(std::string app_name);
        std::string app_name;
        std::string graph;
        std::string labels;
        tensorflow::int32 input_width;
        tensorflow::int32 input_height;
        float input_mean;
        float input_std;
        std::string input_layer;
        std::string output_layer;
        std::string root_dir;
        //TODO set the global state for tensorflow
        //loading and initializing the model.
        std::unique_ptr<tensorflow::Session> session;
        std::vector<tensorflow::Tensor> outputs;
};
#endif
#endif
