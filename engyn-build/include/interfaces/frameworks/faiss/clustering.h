#ifndef CLUSTER_H
#define CLUSTER_H

#ifdef ENABLE_FAISS

#include <faiss/IndexFlat.h>
#include <faiss/IndexIVFPQ.h>

#include <faiss/gpu/GpuIndexFlat.h>
#include <faiss/gpu/GpuIndexIVFFlat.h>
#include <faiss/gpu/StandardGpuResources.h>

#include <faiss/IndexIVF.h>
#include <faiss/AutoTune.h>
#include <faiss/index_factory.h>
#include <faiss/clone_index.h>
#include <faiss/IVFlib.h>

#include <faiss/gpu/GpuCloner.h>
#include <faiss/index_io.h>

#include <opencv2/opencv.hpp>
#include "../../../core/resource_manage.h"

typedef faiss::Index::idx_t idx_t;

class awiros_clustering : public awiros_resource{

  private:
    //int dimension = 1024; // Size of each vector
    //int database_size = 1; // Total database size
    //int nearest_neighbour = 1; // K in K-NN
    //bool use_gpu=false;

  public:
    int dimension = 1024; // Size of each vector
    int database_size = 1; // Total database size
    int nearest_neighbour = 1; // K in K-NN
    bool use_gpu=false;
    
    awiros_clustering(){};
    //awiros_clustering(int d, int d_size, int k, bool use_gpu);
    void init(bool use_gpu);

    float* create_input(cv::Mat input);
    //faiss::Index* create_db(cv::Mat data, int n_cent);
    void create_db(cv::Mat data, int n_cent, std::unique_ptr<faiss::Index>& index);
    void set_nearest_neighbour(int);

    // utility functions for helping the searching in cluster
    //std::vector<idx_t> threshing_searched_indices(std::vector<idx_t> ind, std::vector<float> dis, float dist_thresh);
    std::vector<long> threshing_searched_indices(long* ind, float* dis, float dist_thresh);
    
    std::unique_ptr<faiss::Index> create_cluster(const char*, float*);
    void search_cluster(faiss::Index *, float*, std::vector<int>&, std::vector<float>&, int);
    std::unique_ptr<faiss::Index> make_trained_index(const char *, float*);
    void save_cluster(faiss::Index *, const char *filename);
    faiss::Index* load_cluster(const char *filename);

};

#endif

#endif //CLUSTER_H
