#include"../HeaderFile/Common.h"
#include"../FaceDetection/FaceDetection.h"
#include"../TakePicture/TakePicture.h"
#include"../ModelTrain/ModelTrain.h"

#include<QtGui/qimage.h>
#include<QtWidgets/qlabel.h>

class ImageProcess :public TakePicture, public FaceDetecion,public ModelTrain {
public:
          ImageProcess() = delete;                           //���캯��Ĭ�Ϻ�����
          ImageProcess(int TrainningSetting);
          virtual ~ImageProcess();
public:
          /*
           * cv::Mat ----> QImage
           * @name: mat2Qimage
           * @function����MAT����ת��ΪQT��QImage����
           * @param ����ԭʼͼ��  const cv::Mat& mat
          */
          static QImage mat2Qimage(const cv::Mat& mat);

private:
          /*
           * ��ʼ��ͼ�δ�������ڴ�
           * @name: initImageProcess
          */
          bool initImageProcess();

          /*
           * �ͷ�ͼ�δ�������ڴ�
           * @name:releaseImageProcess;
          */
          void releaseImageProcess();

private:
          std::mutex m_cameraMutex;               //�������ȡ������
          std::vector<std::thread> m_threadPool;
};