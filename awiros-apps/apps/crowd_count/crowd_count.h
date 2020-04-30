//#include "core/app.h"
//#include "src/core/app.h"
#include "engyn.h"

class awiros_crowd_count: public awiros_app{
    public:
        void run();
	
//        std::unique_ptr<std::map<std::string, anum_t>>
        //int    process_data(std::vector<std::string>&, std::vector<awiros_blob>&, int);
        std::unique_ptr<std::map<std::string, anum_t>>    process_data(std::vector<std::string>&, std::vector<awiros_blob>&, int);

        std::string write_data(std::map<std::string, anum_t>*);
};

extern "C" awiros_app* app_class(){
      return new awiros_crowd_count;
}


static double rule(awiros_blob&);
//static int count_threshold = 3;
