//
// Created by anton on 21.03.15.
//

#include "cvCamShift.h"

#include "OpenCV.h"
#include "Matrix.h"
#include <nan.h>

v8::Persistent<FunctionTemplate> cvCamShift::constructor;

void
cvCamShift::Init(Handle<Object> target) {
    NanScope();

    //Class
    Local<FunctionTemplate> ctor = NanNew<FunctionTemplate>(cvCamShift::New);
    NanAssignPersistent(constructor, ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(NanNew("cvCamShift"));

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(ctor, "process", Process);

    target->Set(NanNew("cvCamShift"), ctor->GetFunction());
}

NAN_METHOD(cvCamShift::New) {
        NanScope();

        NanReturnValue(args.Holder());
}


cvCamShift::cvCamShift(): ObjectWrap() {

}

NAN_METHOD(cvCamShift::Process) {
        NanScope();

        static float lastangle = -10000;

        Matrix *src = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
        cv::Mat image = cv::Mat::zeros(src->mat.size(), CV_8UC1 );
        cv::cvtColor(src->mat, image, CV_RGB2GRAY, 1);
        //src->mat.convertTo(image, CV_8UC1);

        //CvRect ROI;
        //GetROI( &ROI, image->width, image->height );
        cv::Rect window;
        window.x = window.y = 0;
        window.width = image.cols;
        window.height = image.rows;

        cv::RotatedRect box = cv::CamShift(image, window, cv::TermCriteria(CV_TERMCRIT_EPS|CV_TERMCRIT_ITER, 10, 1.0 ));

        cv::Point2f ptf[4];
        box.angle *= -1;
        box.points(ptf);
        cv::Point pt[4];
        for( int i=0; i<4; i++ ) {
            pt[i].x = (int)ptf[i].x;
            pt[i].y = (int)ptf[i].y;
        }

        float sh=90;
        if( lastangle != -10000 ) {
            while( box.angle < lastangle-sh ) box.angle+=sh*2;
            while( box.angle > lastangle+sh ) box.angle-=sh*2;
        }
        while( box.angle < 0 ) box.angle += 360;
        box.angle = fmod( box.angle, 360 );

        lastangle = box.angle;

        float max = sqrt((image.cols*image.cols)+(image.rows*image.rows));

        v8::Local<v8::Array> arr = NanNew<Array>(5);

        arr->Set(0, NanNew<Number>((box.center.x + window.x)/image.cols)); // x
        arr->Set(1, NanNew<Number>((box.center.y + window.y)/image.rows)); // y
        arr->Set(2, NanNew<Number>((float)box.size.width/max)); // width
        arr->Set(3, NanNew<Number>((float)box.size.height/max)); // height
        arr->Set(4, NanNew<Number>(box.angle)); // angle

        NanReturnValue(arr);
}