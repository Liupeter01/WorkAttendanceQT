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
 *  QTWidget�㿪����ǰ֡���ղ���
 * @name: CameraShooting
 * @function�����������㵱ǰ�ĵ�֡����
*/
void  Interface::videoCameraShooting()
{
          this->enableCameraShooting();
}

/*
 *  QTWidget�㿪����ǰ֡�ı������
 * @name: videoFrameSavingProcess
 * @function�������Ǳ��浱ǰ�ĵ�֡����
*/
void  Interface::videoFrameSavingProcess()
{
          this->enableSavingProcess();
}

/*
 *  QTWidget�㿪����ǰ֡����������
 * @name: videoFrameIgnoreProcess
 * @function��������������ǰ�ĵ�֡����
*/
void  Interface::videoFrameIgnoreProcess()
{
          this->enableIgnoreProcess();
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
 * @param: 1. ��Ƶ���pixmap : QLabel*& _qlabel
 *                 2. ������ڽӿڣ�QTextBrowser*& _systemOutput
*/
void Interface::QTVideoOutput(QLabel*& _qlabel, QTextBrowser*& _systemOutput)
{
          while (!m_videoFlag) {
                    this->setLabelImage(this->startVideoDisplay(_systemOutput), _qlabel);                       //����ؼ��ֱ���
          }
          _qlabel->clear();
}