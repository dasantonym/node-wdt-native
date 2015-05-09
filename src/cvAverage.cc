//
// Created by anton on 21.03.15.
//

#include "cvAverage.h"

#include "OpenCV.h"
#include "Matrix.h"
#include <nan.h>

v8::Persistent<FunctionTemplate> cvAverage::constructor;

cv::Mat mat;

void
cvAverage::Init(Handle<Object> target) {
    NanScope();

    //Class
    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(cvAverage::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("cvAverage"));

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(ctor, "process", Process);

    target->Set(NanNew("cvAverage"), ctor->GetFunction());
}

NAN_METHOD(cvAverage::New) {
        NanScope();

        NanReturnValue(args.Holder());
}


cvAverage::cvAverage(): ObjectWrap() {
    mat = cv::Mat::zeros(1, 1, CV_32FC3);
}

NAN_METHOD(cvAverage::Process) {
        NanScope();

        Matrix *src = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        cv::Mat srcMat = cv::Mat::zeros(src->mat.size(), CV_8UC3);
        src->mat.convertTo(srcMat, CV_8UC3);

        if (src->mat.size() != mat.size()) {
            mat = cv::Mat::zeros(src->mat.size(), CV_32FC3);
        }

        cv::accumulateWeighted(srcMat, mat, (double)args[1]->NumberValue());

        mat.convertTo(src->mat, CV_8UC3);

        NanReturnNull();
}