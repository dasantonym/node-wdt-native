//
// Created by Anton on 28/04/15.
//

#include "cvDisplayFormat.h"
#include "Matrix.h"
#include <nan.h>

v8::Persistent<FunctionTemplate> cvDisplayFormat::constructor;

void
cvDisplayFormat::Init(Handle<Object> target) {
    NanScope();

    //Class
    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(cvDisplayFormat::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("cvDisplayFormat"));

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(ctor, "process", Process);

    target->Set(NanNew("cvDisplayFormat"), ctor->GetFunction());
}

NAN_METHOD(cvDisplayFormat::New) {
        NanScope();

        NanReturnValue(args.Holder());
}


cvDisplayFormat::cvDisplayFormat(): ObjectWrap() {

}

NAN_METHOD(cvDisplayFormat::Reset) {
        NanScope();

        NanReturnNull();
}

NAN_METHOD(cvDisplayFormat::Process) {
        NanScope();

        Matrix *src = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());

        if (src->mat.channels() != 4) {
            cv::Mat srcMat = cv::Mat::zeros(src->mat.size(), CV_8UC3);
            src->mat.convertTo(src->mat, CV_8U);
            if (src->mat.channels() < 3) {
                cv::cvtColor(src->mat, srcMat, CV_GRAY2RGB, 3);
            } else if (src->mat.channels() == 3) {
                srcMat = src->mat;
            }
            cv::Mat alphaMat = cv::Mat(srcMat.size(), CV_8UC1, cv::Scalar(255));
            std::vector<cv::Mat> sourceMats;
            sourceMats.push_back(srcMat);
            sourceMats.push_back(alphaMat);
            cv::Mat alphaImg = cv::Mat(srcMat.size(), CV_8UC4);
            std::vector<cv::Mat> dstMats;
            dstMats.push_back(alphaImg);

            int from_to[] = { 0,0, 1,1, 2,2, 3,3 };

            cv::mixChannels(sourceMats, dstMats, from_to, 4);

            src->mat = dstMats[0];
        }

        NanReturnNull();
}
