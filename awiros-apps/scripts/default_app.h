#ifndef _DEFAULT_APP_H_
#define _DEFAULT_APP_H_

#include "engyn.h"

class awiros_default_app: public awiros_app{
    public:
        void run();
};

extern "C" awiros_app* app_class(){
      return new awiros_default_app;
}

#endif
