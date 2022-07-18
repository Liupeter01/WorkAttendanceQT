#pragma once
#include<future>
#include<queue>
#include <iostream>
#include<fstream>
#include<string>
#include<sstream>

#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/videoio.hpp>
#include<opencv2/highgui/highgui_c.h>
#include<opencv2/opencv.hpp>

#include<opencv2/face.hpp>                                  //PCA人脸识别算法
#include<opencv2/face/facerec.hpp>
#include<dlib/opencv.h>
#include<dlib/image_processing/frontal_face_detector.h>
#include<dlib/image_processing/render_face_detections.h>
#include<dlib/image_processing.h>
#include<dlib/gui_widgets.h>
#include<dlib/dnn.h>
#include<dlib/clustering.h>