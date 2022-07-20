#include"../ImageProcess/ImageProcess.h"
#include"../DBProcess/DBProcess.h"

class Interface :public DBProcess, public ImageProcess {
public:
          Interface() = delete;
          Interface(int TrainningSetting);
          virtual ~Interface();


          void Videoclose() {
                    m_videoFlag = true;
          }

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
           * @param 1.����QImage
           *                2.������ʾͼ���ָ��_qlabel
          */
          void setLabelImage(QLabel*& _qlabel);

private:
          std::atomic<bool> m_videoFlag = false;
          std::mutex m_writeLock;                 //ͼ�����д����
          std::mutex m_cameraMutex;           //��ȡ����Ļ�����
};