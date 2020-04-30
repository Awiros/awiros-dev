#ifndef _CONV_CYCLE_LAYER_H_
#define _CONV_CYCLE_LAYER_H_

#include "../ext/json/json.hpp"
#include "app.h"
#include <nan.h>
#include "term.h"
//#include "heatmap.h"
//#include "src/aband.h"
#include "camera.h"
#include "motion.h"
#include "auth.h"
#include "utils.h"
/*
#include "src/tracking.h"
#include "src/tracking_plusplus.h"
#include "src/region_check.h"
#include "src/footfall.h"
#include "src/simple_camera.h"
#include "src/speed.h"
#include "src/speed_plusplus.h"
#include "src/wrong_way.h"
#include "src/number_plate.h"
#include "src/helmet.h"
#include "src/detector.h"
#include "src/wrong_way_plusplus.h"
#include "src/person_loitering.h"
#include "src/street_view.h"
*/
//#include "src/trainer.h"
/*
#include "src/crowd_count.h"
#include "src/debris.h"
#include "src/person_collapse.h"
#include "src/reid.h"
#include "src/robust_tampering.h"
#include "src/vehicle_count.h"
//#include "src/rebel.h"
#include "src/general_count.h"
#include "src/crowd_plus.h"
#include "src/fire_detection.h"
#include "src/rebel.h"
#include "src/zebra_violation.h"
#include "src/smoke_detection.h"
#include "src/hard_reid.h"
#include "src/reid_multiblob.h"
#include "src/parking_violation.h"
#include "src/parking_manager.h"
#include "src/wrong_way_vehicle.h"
#include "src/worker_protection.h"
#include "src/person_count.h"
#include "src/npr_on_zebra.h"
#include "src/npr_on_parking.h"

#include "src/car_damage.h"
#include "src/dynamic_gpu_app.h"
#include "src/face_count.h"
#include "src/graffiti.h"
#include "src/griffin.h"
#include "src/liveliness_detection.h"
#include "src/pothole.h"
#include "src/recog.h"
#include "src/tampering.h"
#include "src/tailgate.h"
#include "src/faiss_test.h"
#include "src/frs_test.h"
#include "src/opencv_mtcnn.h"
*/

#include <dlfcn.h>

using namespace nlohmann;

class conversion_cycle_layer{
  public:

    json m_jacs;
    ~conversion_cycle_layer();

    const Nan::AsyncProgressQueueWorker<char>::ExecutionProgress *p_prg;

    void set_progress_worker(const Nan::AsyncProgressQueueWorker<char>::ExecutionProgress *p_prg_in);

    void set_jacs(json jacs);

    void send_event_to_node(std::string sads);
    void send_json_event_to_node(json jads);

    void check_camera_working();
    //void check_camera_working(awiros_app &app);
    void check_camera_working(awiros_app *app);
    void check_camera_working(awiros_camera& cam);
    void get_validity_key();
    void check_validity_key();
    //void get_heatmap();
    //void  process_app_event(awiros_app& app);
    void  process_app_event(awiros_app* app);
    void  process_app_event_with_heat(awiros_app* app);
    //void  process_report_event(awiros_trainer& app);

    // void process_training_app_event(awiros_trainer::train_event_data);
    void check_actual_camera_working(awiros_camera *cam);

    //json process_heatmap_event(awiros_heatmap& heatmap);
    json get_json_db(awiros_blob &blob);
    json get_json_classification(awiros_blob &blob);
    //json process_state_check_event(awiros_region_check &check_state);

    //void get_awiros_app_from_acs(json jacs, awiros_app &app);
    void get_awiros_app_from_acs(json jacs, awiros_app* app);
    
    // void get_awiros_training_app_from_acs(json jacs, awiros_app &app);


    void action_on_app();

    /*
    template<typename T>
    void start_app(){
      // make state available out of scope
      awiros_app_state state = CONTINUE;

      do{
        // run app
        T app;

        // for all other apps
        get_awiros_app_from_acs(m_jacs, app);
        this->check_camera_working(app);

        app.p_conv_layer_obj = this;
        std::cout << "Starting app : " << app.name <<  std::endl;
        app.run();

        // update state
        state = get_desired_app_state(app.id);
    	//TODO: SID INTERNAL RESTART
        // m_jacs = get_new_acs(app.id);

      } while(state == RESTART);

      return;
    }
    */

    void start_dynamic_app(std::string app_name){

      // make state available out of scope
      awiros_app_state state = CONTINUE;

      do{

        void *hndl = dlopen(app_name.c_str(), RTLD_LAZY);
        if(!hndl){
          std::cout << " Dlopen not loading the library " << std::endl;
          exit(1);
        }

        create_t* mkr = (create_t*) dlsym(hndl, "app_class");
        awiros_app* app = mkr();

        //T app;
        // for all other apps
        get_awiros_app_from_acs(m_jacs, app);
        this->check_camera_working(app);

        app->p_conv_layer_obj = this;
        std::cout << "Starting app : " << app->name <<  std::endl;
        app->run();

        // update state
        state = get_desired_app_state(app->id);

      } while(state == RESTART);

      return;
    }


  private:
    int test;

    awiros_camera temp_cam;

    void            get_awiros_timeslots(json &j_time, std::vector<awiros_timeslot> &dst);

    //awiros_camera   get_awiros_camera_from_acs(json jacs);
    void            get_awiros_camera_from_acs(json jacs, awiros_camera &cam);
    awiros_term     get_awiros_term_from_acs(json jacs);
    awiros_frame    get_awiros_frame_from_acs(json jacs);
    awiros_motion   get_awiros_motion_from_acs(json jacs);


    void get_ads_from_awiros_blobs(json &jads, std::vector<awiros_blob> &blobs);

    void get_ads_from_awiros_blob(json &jads, awiros_blob blob);

    awiros_blob_config get_awiros_blob_config_from_acs(json jacs);
    awiros_tracker_config get_awiros_tracker_config_from_acs(json jacs);
    awiros_detector_config get_awiros_detector_config_from_acs(json jacs);

    //awiros_trainer_config get_awiros_trainer_config_from_acs(json jacs); 

    void get_ads_from_awiros_camera(json &jads, awiros_camera cam);
    void get_ads_from_awiros_camera(json &jads, awiros_camera *cam);
    //void get_ads_from_event_severity(json &ads_object,  awiros_app app);
    void get_ads_from_awiros_frame(json &jads, awiros_frame aframe);
    void get_ads_from_server_heat(json &jads, awiros_frame aframe);
    void get_ads_from_awiros_term(json &jads, awiros_term term);
    void get_ads_from_awiros_motion(json &jads, awiros_motion &motion);
    void get_ads_from_awiros_event(json &jads, std::vector<awiros_event>& events);
    void get_ads_from_event_severity(json &jads, awiros_app *app);
    void embed_app_info(json &jads, awiros_app *app);
    awiros_app_state extract_app_state(json jacs);
    void fill_lines(json &j_lines, json &j_frame, std::vector<awiros_line> &dst);
    void fill_regions(json &j_regions, json &j_frame, std::vector<awiros_region> &dst);

    // awiros_blob_config get_awiros_trainer_blob_config_from_acs(json jacs, int);
};

json get_json_bb(awiros_blob &blob);

#endif // _CONV_CYCLE_LAYER_H_
