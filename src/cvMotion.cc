//
// Created by anton on 21.03.15.
//

#include "cvMotion.h"
#include "Matrix.h"
#include <nan.h>

v8::Persistent<FunctionTemplate> cvMotion::constructor;

float weight, scale;
double shift;
int mode;

cv::Mat accImage, tmpImage;
cv::Size tmpSize;

void
cvMotion::Init(Handle<Object> target) {
    NanScope();

    //Class
    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(cvMotion::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("cvMotion"));

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(ctor, "process", Process);

    target->Set(NanNew("cvMotion"), ctor->GetFunction());
}

NAN_METHOD(cvMotion::New) {
        NanScope();

        tmpSize.width = tmpSize.height = 0;

        NanReturnValue(args.Holder());
}


cvMotion::cvMotion(): ObjectWrap() {

}

NAN_METHOD(cvMotion::Reset) {
        NanScope();

        if (!accImage.empty()) {
            accImage.release();
        }
        tmpSize.width = tmpSize.height = 0;

        NanReturnNull();
}

NAN_METHOD(cvMotion::Process) {
        NanScope();

        Matrix *src = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        cv::Mat srcMat = cv::Mat::zeros(src->mat.size(), CV_8UC3);
        src->mat.convertTo(srcMat, CV_8UC3);

        Local<Object> outMatrixWrap = NanNew(Matrix::constructor)->GetFunction()->NewInstance();
        Matrix *outMatrix = ObjectWrap::Unwrap<Matrix>(outMatrixWrap);

        weight = (float)args[1]->NumberValue();
        scale = (float)args[2]->NumberValue();
        shift = (double)args[3]->NumberValue();
        mode = args[4]->IntegerValue();

        if (srcMat.size() != accImage.size()) {
            accImage = cv::Mat::zeros(srcMat.size(), CV_32FC3);
            srcMat.convertTo(accImage, CV_32FC3);
            tmpImage = cv::Mat::zeros(srcMat.size(), CV_8UC3 );
        }

        cv::accumulateWeighted( srcMat, accImage, weight );
        accImage.convertTo(tmpImage, CV_8UC3);


        switch ( mode ) {
            case 0:
                cv::absdiff( srcMat, tmpImage, outMatrix->mat );
            break;
            case 1:
                cv::subtract( srcMat, tmpImage, outMatrix->mat );
            break;
            case 2:
                cv::subtract( tmpImage, srcMat, outMatrix->mat );
            break;
        }


        if ( scale != 1 || shift != 0 ) {
            outMatrix->mat.convertTo(src->mat, CV_8UC3, scale, shift);
        } else {
            src->mat = outMatrix->mat;
        }

        NanReturnNull();

        //NanReturnValue(outMatrixWrap);
}
