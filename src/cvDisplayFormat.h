//
// Created by Anton on 28/04/15.
//

#ifndef OPENCV_CVDISPLAYFORMAT_H
#define OPENCV_CVDISPLAYFORMAT_H


#include "OpenCV.h"

class cvDisplayFormat : public node::ObjectWrap {
public:

    static Persistent<FunctionTemplate> constructor;
    static void Init(Handle<Object> target);
    static NAN_METHOD(New);
    cvDisplayFormat();

    JSFUNC(Reset);
    JSFUNC(Process);

};


#endif //OPENCV_CVDISPLAYFORMAT_H
