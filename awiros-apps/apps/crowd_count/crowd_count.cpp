#include "crowd_count.h"
//#include "src/core/utils.h"
//#include "src/core/triggers.h"
//#include "src/core/resource_manage.h"
//#include "src/interfaces/frameworks/darknet/dnet.h"
//#include "src/core/tracker.h"
#include <memory>
//#include "src/core/filter.h"
//#include "src/core/annotator.h"

int old_frame_index_cc = -999;
std::size_t old_frame_round_cc = 0;

double get_overlap_area(cv::Rect parent, cv::Rect child){
  
  // For finding the intersection area
  double x_left = std::max(parent.x, child.x);
  double y_top = std::max(parent.y, child.y);
  double x_right = std::min(parent.x+parent.width, child.x+child.width);
  double y_bottom = std::min(parent.y+parent.height, child.y+child.height);

  if(x_right<x_left || y_bottom < y_top){
    return 0.0;
  }

  double intersection_area = (x_right - x_left) * (y_bottom - y_top);
  double overlap = intersection_area / (parent.width*parent.height);
  return overlap;

}

void awiros_crowd_count::run(){
awiros_tracker trackers;

#ifdef ENABLE_DARKNET


	char* datacfg = "model/common/coco.data";
	char* cfgfile = "model/common/yolov3.cfg";
	char* weightfile = "model/common/yolov3.weights";

	awiros_darknet* darknet;
	darknet = g_resource_registry.get_resource<awiros_darknet>("darknet");

	if(!darknet){

		/* Generate resource */
		darknet = new awiros_darknet();

		/* Publish to all threads */
		g_resource_registry.register_resource<awiros_darknet>("darknet", darknet);
	}

	/* Initialize resource */
	darknet->init(this->detector_config, datacfg, cfgfile, weightfile);
	darknet->conf = this->blob_config.conf_classification;
#endif

	// debug
	//this->cam.init_writer("out_crowd_count.avi", this->aframe);

	awiros_utils::awiros_clock clock;
	awiros_utils::awiros_clock cooling_clock; bool cooling = false;
	awiros_tracker tracker;


	/* Labels of interest */
	std::vector<std::string> labels{"person"};

	awiros_filter filter(rule);

	awiros_annotator annotator(*this, tracker);

  awiros_camera *global_cam;
  global_cam = g_resource_registry.get_camera_resource<awiros_camera>(std::to_string(this->camera_id));
  if (!global_cam){ 
    std::cout << " Camera is not already registered !! Run Simple Camera App first for now. " << std::endl;
#ifdef ENABLE_DARKNET
    g_resource_registry.deregister_resource("darknet");
#endif   
    return;                                                                                            
  }


	while(1){

    //std::cout << " A " << std::endl;
		/* Application logic */

    awiros_camera *global_cam;
    global_cam = g_resource_registry.get_camera_resource<awiros_camera>(std::to_string(this->camera_id));
    if (!global_cam){ 
      std::cout << " Common Decoder is not running !! " << std::endl;                                  
#ifdef ENABLE_DARKNET
      g_resource_registry.deregister_resource("darknet");
#endif   
      return;
    }

    if (global_cam->app_frames.find(this->app_id) == global_cam->app_frames.end()){
      global_cam->app_frames.insert(std::pair<int, frame_struct>(this->app_id, global_cam->fs_caller));
    }

    this->print_app_fps(10, this->app_id);

		if(clock.is_time_interval(1)){
			awiros_app_state state = get_desired_app_state(this->id);
			if(state != CONTINUE || global_cam->cam_ready == 0){
#ifdef ENABLE_DARKNET
				g_resource_registry.deregister_resource("darknet");
#endif
        //global_cam->app_frames.at(this->app_id).thread_stop = 1;
        return;
			}
		}

/*
    std::cout << " ----------------------------------------------------------- " << std::endl;
    std::cout << "Frame index Crowd :: " <<  global_cam->app_frames.at(this->app_id).frame_ind_new << std::endl;
    std::cout << " Old index Crowd :: " << old_frame_index_cc << std::endl;
    std::cout << " ############################################################# " << std::endl;
*/

    if (old_frame_index_cc > global_cam->app_frames.at(this->app_id).frame_ind_new){
      if (old_frame_round_cc < global_cam->frame_bank_round){
        old_frame_index_cc = -999;
        old_frame_round_cc = global_cam->frame_bank_round;
      }
    }

    if (global_cam->app_frames.at(this->app_id).frame_ind_new > old_frame_index_cc){
      old_frame_index_cc = global_cam->app_frames.at(this->app_id).frame_ind_new;
      if (old_frame_index_cc >=0 && old_frame_index_cc<=(global_cam->frame_limit-1)){
        this->aframe.set_frame(global_cam->app_frames.at(this->app_id).frame_bank[old_frame_index_cc].clone());
        global_cam->app_frames.at(this->app_id).processed_frame++;
      }
      else{
        old_frame_index_cc = -999;
        continue;
      }
    }
    else{
      continue;
    }

		if (this->aframe.get_frame().rows < 1 || this->aframe.get_frame().cols < 1) {
			this->print_message(msg_t::ERROR, "process_camera" , "Invalid Frame Received");
			continue;
		}

		/* wait for cooling period to get over */
		if(cooling){
			if(!cooling_clock.is_time_interval(5))
				continue;

			cooling = false;
		}
		int severity_thresh;
		/*
		   if(this->motion.regions.size() > 0) {
		   cv::Rect mask_rect = this->motion.regions[0].get_bounding_rect();
		   severity_thresh = 5;
		   cv::Mat this_mat;
		   this->aframe.get_frame().copyTo(this_mat);
		   this->aframe.set_frame(this_mat(mask_rect));
		   }
		 */

		/* Preprocess frame  */
		this->aframe.preprocess_frame();

		/* Detect blobs*/
#ifdef ENABLE_DARKNET
		int nblobs = tracker.detect_with_darknet(this->aframe, *darknet);
#endif

		/* Filter blobs*/
		std::vector<awiros_blob> filtered_blobs = filter.get_min_thresholded_blobs(tracker.curr_blobs);

/*
    if ( old_frame_round_cc == global_cam->frame_bank_round){
      std::cout << " Same Round !! " << std::endl;
      std::cout << " frame_ind_new :: " << global_cam->app_frames.at(this->app_id).frame_ind_new << std::endl;
      std::cout << " old frame ind :: " << old_frame_index_cc << std::endl;
		
      if ((old_frame_index_cc >= global_cam->app_frames.at(this->app_id).frame_ind_new-5 ) || (old_frame_index_cc < global_cam->app_frames.at(this->app_id).frame_ind_new+4)){
        std::cout << " Going for drawing" << std::endl;
*/
    cv::Mat binary_mask, color_mask;
    tracker.create_blobs_mask(this->aframe, this->motion.regions, this->motion.lines, filtered_blobs, binary_mask, color_mask);
    global_cam->app_frames.at(this->app_id).mask_box_binary = binary_mask.clone();
    global_cam->app_frames.at(this->app_id).mask_box_color = color_mask.clone();

/*
		global_cam->app_frames.at(this->app_id).app_regions = this->motion.regions;
		global_cam->app_frames.at(this->app_id).app_lines = this->motion.lines;
		global_cam->app_frames.at(this->app_id).app_width = this->aframe.dwidth;
		global_cam->app_frames.at(this->app_id).app_height = this->aframe.dheight;
		global_cam->app_frames.at(this->app_id).app_blobs = filtered_blobs;
*/


		/* Get count map */
		std::unique_ptr<std::map<std::string, anum_t>> count_ht = this->process_data(labels, filtered_blobs, severity_thresh);

		/* Write data on frame */
		std::string str_count = this->write_data(count_ht.get());
		int total_count;
		std::istringstream ff(str_count);
		ff >> total_count;
		//std::cout<<"Count : "<< total_count<< "\n";

		/*-- Master Blob --*/

		awiros_blob blob;
		blob.myclass.heatmap = str_count;
		blob.myclass.name = "crowd count";
		aid_t blob_id = 0;
		blob.set_id(blob_id);
		blob.myclass.id = (long)blob_id;
		this->aframe.get_frame().copyTo(blob.bmat);
		blob.bmat = this->aframe.frame.clone();
		this->aframe.get_frame().copyTo(blob.mat);
		if(this->motion.regions.size() > 0) {
			this->motion.draw_motion_events(this->aframe);
			std::istringstream ss(this->motion.regions[0].label);
			ss >> severity_thresh;
			/*
			   for(auto it = count_ht.get()->begin();
			   it != count_ht.get()->end();
			   it++){
			   total_count = it.second;
			   }
			 */
			if(total_count < severity_thresh)
				blob.myclass.severity = LOW;
			else if(total_count> severity_thresh && total_count < severity_thresh+15)
				blob.myclass.severity = MID;
			else if( total_count > severity_thresh+15 && total_count < severity_thresh+30)
				blob.myclass.severity = HIGH;
			else if( total_count > severity_thresh+30 )
				blob.myclass.severity = CRITICAL;
		}
		blob.myclass.label = std::to_string(total_count);
		this->blobs.push_back(blob);


		/* send event */
		if(this->events.size() > 0){
			/*
			   for(auto fit = filtered_blobs.begin(); fit != filtered_blobs.end(); ++fit)
			   fit->draw_on_frame(this->aframe, false, 1);
			 */
			this->send_event();
			//this->events.clear();

			/* start cooling period */
			cooling = true;
			cooling_clock.reset();
		}

		//annotator.annotate();

		/* Write video*/
		//this->cam.write(this->aframe);
		this->clean_up(); 
	}
}

double rule(awiros_blob& blob){
	std::string person = "person";

	/* match check */
	if(blob.myclass.label.compare(person) == 0 && (1.0*blob.height)/blob.width > 1)
		return 1;

	return 0;
}


std::unique_ptr<std::map<std::string, anum_t>> awiros_crowd_count::process_data(std::vector<std::string>& labels, std::vector<awiros_blob>& blobs, int severity_thresh){
	std::unique_ptr<std::map<std::string, anum_t>> table(new std::map<std::string, anum_t>);

	for(auto bit = blobs.begin(); bit != blobs.end(); bit++){
		if(this->motion.regions.size() > 0) {
			for(int riter = 0; riter < this->motion.regions.size(); riter++){
				cv::Rect region = this->motion.regions[riter].get_bounding_rect();
				int x1 = region.x;
				int x2 = region.x+region.width;
				int y1 = region.y;
				int y2 = region.height+region.y;

				cv::Rect blob_rect = bit->get_cv_rect();
				double overlap = get_overlap_area(region, blob_rect);
				//std::cout << " Overlap :: " << overlap << std::endl;
				//if(bit->cx > x1 && bit->cx < x2 && bit->cy > y1 && bit->cy < y2){

				if (overlap > 0){

					anum_t& val = (*table.get())[bit->myclass.label];

					if(!val){
						val = 1;
						continue;
					}

					val++;

					bit->draw_on_frame(this->aframe, false, 1);
					/* check for event */
					if(val > this->space_senstivity_x){
						static awiros_event event;
						this->events.push_back(event);
					}
				}
			}

		}
		else{


			anum_t& val = (*table.get())[bit->myclass.label];

			if(!val){
				val = 1;
				continue;
			}

			val++;

			/* check for event */
			if(val > this->space_senstivity_x){
				static awiros_event event;
				this->events.push_back(event);
			}
		}
	}
	return table;
}


std::string awiros_crowd_count::write_data(std::map<std::string, anum_t>* table ){
	std::string count;
	/* Draw detections */


	/* Show counter */
	cv::rectangle(this->aframe.frame, cv::Rect(0, 0, 150, 80), cv::Scalar(255, 255, 255), cv::FILLED);
	for(auto it = table->begin();
			it != table->end();
			it++){
		count = std::to_string(it->second);

		/* Write on frame */
		cv::putText(this->aframe.frame, "PERSON: " + std::to_string(it->second) , cv::Point(15 ,  20), cv::FONT_HERSHEY_PLAIN, 1.1, cv::Scalar(255, 0, 0), 1);
	}
	return count;
}

