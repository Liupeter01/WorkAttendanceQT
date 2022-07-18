#include"../HeaderFile/Common.h"
#include"../FaceDetection/FaceDetection.h"
#include"../TakePicture/TakePicture.h"
#include"../ModelTrain/ModelTrain.h"

class ImageProcess :public TakePicture, public FaceDetecion {
public:
          ImageProcess();
          virtual ~ImageProcess();
private:
};