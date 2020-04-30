#ifndef _FITLER_H_
#define _FITLER_H_

#include "blob.h"

typedef std::vector<awiros_blob> ablobs_t;

class awiros_filter{
    public:
      double (*rule)(awiros_blob& blob);
      awiros_filter();
      awiros_filter(double rule(awiros_blob& blob));
      void print_info();
      awiros_blob& get_best_match(ablobs_t& blobs);
      awiros_blob& get_best_match_ppe(ablobs_t& blobs);
      std::vector<awiros_blob> get_min_thresholded_blobs(ablobs_t& blobs);

      /* For FRS */
      awiros_blob_class get_best_class(std::vector<awiros_blob_class> vec_class);
      std::vector<awiros_blob_class> get_n_best_class(std::vector<awiros_blob_class> vec_class, int n, int order);
      std::vector<awiros_blob_class> get_best_frequent_class(std::vector<awiros_blob_class> vec_class);
};

#endif// _FITLER_H_
