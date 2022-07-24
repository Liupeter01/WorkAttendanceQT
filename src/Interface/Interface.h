#include"../ImageProcess/ImageProcess.h"
#include"../DBProcess/DBProcess.h"

class Interface :public DBProcess, public ImageProcess {
public:
          Interface() = delete;
          Interface(int TrainningSetting);
          virtual ~Interface();

          /*
           *  QTWidget����Ƶ�رպ���
           * @name: QTVideoOutput
           * @function�������ǹر���Ƶ�е�ѭ������
          */
          void  videoClose(); 

          /*
           *  QTWidget�㿪����ǰ֡���ղ���
           * @name: CameraShooting
           * @function�����������㵱ǰ�ĵ�֡����
          */
          void  videoCameraShooting();

          /*
           *  QTWidget�㿪����ǰ֡�ı������
           * @name: videoFrameSavingProcess
           * @function�������Ǳ��浱ǰ�ĵ�֡����
          */
          void  videoFrameSavingProcess();

          /*
           *  QTWidget�㿪����ǰ֡����������
           * @name: videoFrameIgnoreProcess
           * @function��������������ǰ�ĵ�֡����
          */
          void  videoFrameIgnoreProcess();

          /*
           *  QTWidget����Ƶ�����������
           * @name: QTVideoOutput
           * @function������������ʾͼ������ʾ��ͼ��
           * @param: 1.����QImage
          */
          void QTVideoOutput(QLabel*& _qlabel);

private:
          /*
           *  QLabelͼ�㸲д����
           * @name: setLabelImage
           * @function����MAT����ת��ΪQT��QImage���Ͳ����ǵ�QLabel������
           * @param 1.������ֵ���͵� QImage&
           *                2.������ʾͼ���ָ��_qlabel
          */
          void setLabelImage(QImage& _qimage, QLabel*& _qlabel);

protected:
          std::atomic<bool> m_videoFlag = false;                      //��Ƶ����
};