#include <opencv2/opencv.hpp>

extern "C" {
  cv::Mat image_to_mat(image im);
  image mat_to_image(cv::Mat m);

}
