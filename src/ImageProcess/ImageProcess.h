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
           * @retValue : ����λ�ڱ�����QImage&��������
           * @Correction: 2022-7-21 �����ڲ�����QImage��ʱ�洢�ṹ������ʹ�����ü���
           *                        2022-7-21 ɾ�������ж���Ĳ��֣���������CV8U_C3�µĴ���ʽ����������������
          */
          inline QImage &mat2Qimage(const cv::Mat& mat);

protected:
          /*
           * ʵʱ����ͷͼ��+�������+����ʶ�������ʾ�ӿ�
           * @name: realTimeFacialDisplay
           * @function�������Ĺ��ܵ���ͨ����ó������ӵ��߳̽��в���
           * @param  1. ͼ���޸Ķ�д��  std::mutex& _writeMutex
           *                  2. ����ԭʼͼ��   cv::Mat& mat
           *
           * @retValue: QImage &
          */
          QImage& realTimeFacialDisplay();

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
          QImage m_qimageFrameStore;                        //���ڲ�����QImage�洢�ṹ
          std::mutex m_imageRWLock;                           //ͼ�����д����
          std::pair<cv::Mat, dlib::rectangle> m_sharedData;  //�̹߳�����������λ��
          std::vector<std::thread> m_threadPool;            //�̳߳�(������ʼ���ȵȲ���)
};