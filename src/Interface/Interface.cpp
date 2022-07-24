#include"Interface.h"

Interface::Interface(int TrainningSetting)
          :ImageProcess(TrainningSetting), DBProcess()
{
          //��ʱ��ImageProcess���Ѿ����ڲ������������ٴγ�ʼ��!!!
}

Interface::~Interface()
{
}

/*
 *  QTWidget����Ƶ�رպ���
 * @name: QTVideoOutput
 * @function�������ǹر���Ƶ�е�ѭ������
*/
void  Interface::videoClose()
{
          m_videoFlag = true;
}

/*
 *  QTWidget�㿪����ǰ֡���Ų���
 * @name: CameraShooting
 * @function�����������㵱ǰ�ĵ�֡����
*/
void  Interface::videoCameraShooting()
{
          this->enableCameraShooting();
}

/*
 *  QLabelͼ�㸲д����
 * @name: setLabelImage
 * @function����MAT����ת��ΪQT��QImage���Ͳ����ǵ�QLabel������
 * @param 1.������ֵ���͵� QImage&
 *                2.������ʾͼ���ָ��_qlabel
*/
void Interface::setLabelImage(QImage& _qimage, QLabel*& _qlabel)
{
          _qimage = _qimage.scaled(_qlabel->width(), _qlabel->height());                 //����ؼ��ֱ���
          _qlabel->setPixmap(QPixmap::fromImage(_qimage));
}

/*
 *  QTWidget����Ƶ�����������
 * @name: QTVideoOutput
 * @function������������ʾͼ������ʾ��ͼ��
*/
void Interface::QTVideoOutput(QLabel*& _qlabel)
{
          while (!m_videoFlag) {
                    this->setLabelImage(realTimeFacialDisplay(), _qlabel);                       //����ؼ��ֱ���
          }
          _qlabel->clear();
}