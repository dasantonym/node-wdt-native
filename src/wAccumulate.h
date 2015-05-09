//
// Created by Anton on 24/04/15.
//

#ifndef OPENCV_WACCUMULATE_H
#define OPENCV_WACCUMULATE_H


#include "OpenCV.h"

class wAccumulate : public node::ObjectWrap {
public:

    static Persistent<FunctionTemplate> constructor;
    static void Init(Handle<Object> target);
    static NAN_METHOD(New);
    wAccumulate();

    JSFUNC(Process);

};


#endif //OPENCV_WACCUMULATE_H
