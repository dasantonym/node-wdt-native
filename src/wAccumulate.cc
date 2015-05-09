//
// Created by anton on 21.03.15.
//

#include "wAccumulate.h"

#include "OpenCV.h"
#include "Matrix.h"
#include <nan.h>

v8::Persistent<FunctionTemplate> wAccumulate::constructor;

float acceleration, friction;
cv::Mat buffer;

void
wAccumulate::Init(Handle<Object> target) {
    NanScope();

    //Class
    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(wAccumulate::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("wAccumulate"));

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(ctor, "process", Process);

    target->Set(NanNew("wAccumulate"), ctor->GetFunction());
}

NAN_METHOD(wAccumulate::New) {
        NanScope();

        buffer = cv::Mat::zeros(1, 1, CV_8UC1);

        NanReturnValue(args.Holder());
}


wAccumulate::wAccumulate(): ObjectWrap() {

}

NAN_METHOD(wAccumulate::Process) {
        NanScope();

        Matrix *src = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        cv::Mat srcMat = cv::Mat::zeros(src->mat.size(), CV_32FC1);
        src->mat.convertTo(srcMat, CV_32FC1);

        Local<Object> outMatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *outMatrix = ObjectWrap::Unwrap<Matrix>(outMatrixWrap);
        outMatrix->mat = cv::Mat::zeros(srcMat.size(), CV_8UC1 );

        acceleration = (float)args[1]->NumberValue();
        friction = (float)args[2]->NumberValue();

        if (buffer.empty() || buffer.size() != srcMat.size()) {
            buffer = cv::Mat::zeros(srcMat.size(), CV_32FC1);
            srcMat.convertTo(buffer, CV_32FC1);
        }

        int channels = buffer.channels();

        int nRows = srcMat.rows;
        int nCols = srcMat.cols * channels;

        if (buffer.isContinuous()) {
            nCols *= nRows;
            nRows = 1;
        }

        int i,j;
        float* bp;
        float* sp;
        float sv = acceleration;
        float bv = friction;
        for( i = 0; i < nRows; ++i) {
            bp = buffer.ptr<float>(i);
            sp = srcMat.ptr<float>(i);
            for ( j = 0; j < nCols; ++j) {
                bp[j] *= bv;
                bp[j] += sp[j] * sv;
                if( bp[j]>0xff ) bp[j]=0xff;
                sp[j] = bp[j];
            }
        }

        srcMat.convertTo(src->mat, CV_8UC3);

        NanReturnNull();
}
