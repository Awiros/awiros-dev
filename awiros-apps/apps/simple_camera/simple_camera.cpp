//#include "core/conversion_layer.h"
//#include "core/types.h"
//#include "core/blob.h"
//#include "core/frame.h"
//#include "core/cascade.h"
//#include "core/tracker.h"
#include "simple_camera.h"
#include "interfaces/webstreamer/webstream.h"


#include <unistd.h>

awiros_simple_camera::awiros_simple_camera(){

}

awiros_simple_camera::awiros_simple_camera(awiros_app &app){
  this->id = app.id;
  this->name = app.name;
  this->active_time = app.active_time;
  this->active_mask = app.active_mask;
  this->severity = app.severity;
  //this->cam = app.cam;
  this->aframe = app.aframe;
  this->motion = app.motion;
  this->print_message(msg_t::INFO, "run()" , "constructed app");
}

void update_line_for_streaming(int local_col, int local_row, int global_col, int global_row, awiros_line &line){

  line.startp.x = ((float)(line.startp.x)/(float)(local_col)) * global_col;            
  line.startp.y = ((float)(line.startp.y)/(float)(local_row)) * global_row;            
  line.endp.x = ((float)(line.endp.x)/(float)(local_col)) * global_col;            
  line.endp.y = ((float)(line.endp.y)/(float)(local_row)) * global_row;            

}

void draw_event_for_streaming(awiros_line line, awiros_frame &frame){

  cv::Point cv_startp(line.startp.x, line.startp.y);
  cv::Point cv_endp(line.endp.x, line.endp.y);
  cv::line(frame.frame, cv_startp, cv_endp, cv::Scalar(0,0,255), 2);
}

void awiros_simple_camera::run() {
  
  awiros_camera *global_cam;
  global_cam = g_resource_registry.get_camera_resource<awiros_camera>(std::to_string(this->camera_id));
  if (!global_cam){
    std::cout << " Global registering the camera. " << std::endl;
    global_cam = new awiros_camera();
    g_resource_registry.register_resource<awiros_camera>(std::to_string(this->camera_id), global_cam);
  }

  this->print_message(msg_t::INFO, "run()" , "in app run");
  if (!global_cam->is_initialized()){
    this->print_message(msg_t::ERROR, "run()" , "Could not initialize camera");
    return;
  }

  this->print_message(msg_t::INFO, "run()" , "starting processing");
  
  int port = 8000 + this->camera_id;
  awiros_stream webstreamer(port, "localhost");
  awiros_utils::awiros_clock clock1;
 
  while(1){
    std::size_t check_cam = global_cam->get_global_frame_new(this->aframe);
    //g_resource_registry.print_info();

    if (check_cam == -1){
      std::cout << " Deregistering the global registry as no new frame we are getting " << std::endl;
      global_cam->cam_ready = 0;
      usleep(10000000);
      g_resource_registry.delete_resource(std::to_string(this->camera_id));
      return;
    }

    this->print_fps(10);

    if(clock1.is_time_interval(app_registry_recheck_time)){
      awiros_app_state state = get_desired_app_state(this->id);
      if(state != CONTINUE){
        global_cam->cam_ready = 0;
        usleep(10000000);
        g_resource_registry.delete_resource(std::to_string(this->camera_id));
        return; 
      }

    }

    if (this->aframe.get_rows() < 1 || this->aframe.get_cols() < 1) {
      continue;
    }

    awiros_frame temp_frame;

    temp_frame.set_frame(this->aframe.frame.clone());


    int global_rows = temp_frame.frame.rows;
    int global_cols = temp_frame.frame.cols;    
    for(global_cam->app_frame_itr = global_cam->app_frames.begin(); global_cam->app_frame_itr != global_cam->app_frames.end(); global_cam->app_frame_itr++){

      cv::Mat temp_bin_mask = global_cam->app_frame_itr->second.mask_box_binary.clone();
      cv::Mat temp_color_mask = global_cam->app_frame_itr->second.mask_box_color.clone();

      if (temp_bin_mask.empty() || temp_color_mask.empty())
        continue;

      cv::resize(temp_bin_mask, temp_bin_mask, cv::Size(global_cols, global_rows), 0, 0, cv::INTER_LINEAR);
      cv::resize(temp_color_mask, temp_color_mask, cv::Size(global_cols, global_rows), 0, 0, cv::INTER_LINEAR);

      cv::Mat temp_masked = temp_frame.frame.mul(temp_bin_mask);
      temp_masked = temp_masked + temp_color_mask;
      temp_frame.set_frame(temp_masked.clone());
    }


/*
    for(global_cam->app_frame_itr = global_cam->app_frames.begin(); global_cam->app_frame_itr != global_cam->app_frames.end(); global_cam->app_frame_itr++){

      int local_row = global_cam->app_frame_itr->second.app_height;
      int local_col = global_cam->app_frame_itr->second.app_width;

      for (int i=0; i<global_cam->app_frame_itr->second.app_lines.size(); i++){

        update_line_for_streaming(local_col, local_row, temp_frame.get_cols(), temp_frame.get_rows(), global_cam->app_frame_itr->second.app_lines[i]);

        draw_event_for_streaming(global_cam->app_frame_itr->second.app_lines[i], temp_frame);

      }

      for (int i=0; i<global_cam->app_frame_itr->second.app_regions.size(); i++){

        for (int j=0; j<global_cam->app_frame_itr->second.app_regions[i].lines.size(); j++){

          update_line_for_streaming(local_col, local_row, temp_frame.get_cols(), temp_frame.get_rows(), global_cam->app_frame_itr->second.app_regions[i].lines[j]);

          draw_event_for_streaming(global_cam->app_frame_itr->second.app_regions[i].lines[j], temp_frame);
        }

      }

      for (int i=0; i<global_cam->app_frame_itr->second.app_blobs.size(); i++){

        cv::Rect old_blob = global_cam->app_frame_itr->second.app_blobs[i].get_cv_rect();
        cv::Rect new_blob;

        new_blob.x = ((float)(old_blob.x)/(float)(local_col)) * temp_frame.get_cols();
        new_blob.y = ((float)(old_blob.y)/(float)(local_row)) * temp_frame.get_rows();
        new_blob.width = ((float)(old_blob.width)/(float)(local_col)) * temp_frame.get_cols();
        new_blob.height = ((float)(old_blob.height)/(float)(local_row)) * temp_frame.get_rows();
        cv::rectangle(temp_frame.frame, new_blob ,cv::Scalar(0, 255, 0), 2, 8, 0 );

      }

      global_cam->app_frame_itr->second.app_blobs.clear();
      global_cam->app_frame_itr->second.app_lines.clear();
      global_cam->app_frame_itr->second.app_regions.clear();
    }
*/
    webstreamer.publish_frame(temp_frame);

  }
}


void awiros_simple_camera::send_event(){
//  p_conv_layer_obj->process_frs_event(*this);
}
