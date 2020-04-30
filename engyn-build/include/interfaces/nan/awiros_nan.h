#ifndef NATIVE_EXTENSION_GRAB_H
#define NATIVE_EXTENSION_GRAB_H

#include <iostream>
//#include "src/consts.h"
#include "../../core/handle_acs.h"

#include <nan.h>
#include <unistd.h>

// Example top-level functions. These functions demonstrate how to return various js types.
// Implementations are in functions.cc

NAN_METHOD(EngynProgressWithACS);

#ifdef COMPILE_WITH_ADD_WATERMARK_FUNC
NAN_METHOD(AddWatermarkToImage);
#endif

class EngynProgressWorker : public Nan::AsyncProgressQueueWorker<char> {
 public:
  EngynProgressWorker(
      Nan::Callback *callback
    , Nan::Callback *progress
    , std::string acs
    , int func_name)
    : AsyncProgressQueueWorker(callback), progress(progress)
    , acs(acs), func_name(func_name) {}

  ~EngynProgressWorker() {
    delete progress;
  }

  void Execute (const Nan::AsyncProgressQueueWorker<char>::ExecutionProgress& progress_test) {
    handle_acs_function(acs, ads, func_name, progress_test);
  }

  void HandleProgressCallback(const char *data, size_t count) {
    Nan::HandleScope scope;

    std::vector<char> sdata(count+1);
    for (size_t i=0; i<count; ++i){
      sdata[i] = data[i];
    }
    sdata[count] = '\0';
    std::string sads(sdata.begin(), sdata.end());

    v8::Local<v8::Value> argv[] = { Nan::New<v8::String>(sads.c_str()).ToLocalChecked()}; 
    progress->Call(1, argv, async_resource);
  }

  void HandleOKCallback() {
    Nan::HandleScope scope;
    v8::Local<v8::Value> argv[] = { Nan::New<v8::String>(ads).ToLocalChecked() };
    this->callback->Call(1, argv, async_resource);
  }

 private:
  Nan::Callback *progress;
  std::string acs;
  std::string ads;
  int func_name;
  int milliseconds;
};

#endif //#define NATIVE_EXTENSION_GRAB_H
