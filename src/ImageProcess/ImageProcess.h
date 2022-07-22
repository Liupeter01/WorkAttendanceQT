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

          /*
           * ���������Ѿ����߳�ģ��ͬ��
           * @name:setDataSyncSignal;
           * @function: ����ָʾ�����߳��й������ݸ��µ�״̬
          */
          void setDataSyncSignal();

          /*
           * �������ݵ�ǰ����������ģ�����ݹ�ʱ
           * @name: resetDataSyncSignal;
           * @function: ����ָʾ�����߳��й������ݸ��µ�״̬
          */
          void resetDataSyncSignal();

          /*
           * ��ȡ��ǰ�����Ƿ�Ϊ����״̬
           * @name: resetDataSyncSignal;
           * @function: ����ָʾ�����߳��й������ݸ��µ�״̬
           * @retValue: 1.true : ��ǰ���ݵ�״̬Ϊ��״̬
           *                    2.false: ��ǰ���ݵ�״̬Ϊ��״̬
          */
          bool getDataSyncStatus();

          /*
           * ��ͣ��Ļ��ʾ�̵߳Ĺ������ȴ��û�����ͼ���ȷ�Ϻ���
           * @name: getUserConfirmStatus
           * @function: ��ѵ���߳���������Ҫ�ȴ��û����ͼ��ȷ��֮��ſ��Լ�����ʾ
           * @retValue: 1.true : �����������һ֡����ʾ
           *                    2.false: ��������һ֡����ʾ
          */
          bool getUserConfirmStatus();

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

          /*
           * ͨ������ͷͼ�����ʵʱ����ѵ������ģ��
           * @name: realTimeFacialDisplay
           * @function����ʵʱ����ͷͼ��Эͬ��������ѵ������
          */
          void videoSyncFacialTranning();

private:
          /*ͨ�����ݽṹ*/
          QImage m_qimageFrameStore;                        //���ڲ�����QImage�洢�ṹ
          std::vector<std::thread> m_threadPool;            //�̳߳�(������ʼ���ȵȲ���)

          std::mutex m_imageRWLock;                           //ͼ�����д����
          std::mutex m_faceRectRWLock;                      //����λ�ø���д����
          std::pair<cv::Mat, std::atomic<bool>> m_imageSync;             //ͼ�����+ͼ����µ�ԭ��״̬
          std::pair<dlib::rectangle, std::atomic<bool>> m_faceRectSync;  //����λ�ø���+����λ�ø��µ�ԭ��״̬

          /*������ʾ�߳�*/
          std::mutex m_CofirmUserInptutEvent;                        //�ȴ��û��ڽ����ϵ����������¼�
          std::condition_variable m_UserInput;                         //�ȴ��û��ڽ����ϵ�����������������

          /*����ѵ���߳�*/

          /*����ʶ���߳�*/
};