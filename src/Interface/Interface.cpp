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
 *  QLabelͼ�㸲д����
 * @name: setLabelImage
 * @function����MAT����ת��ΪQT��QImage���Ͳ����ǵ�QLabel������
 * @param 1.����QImage
 *                2.������ʾͼ���ָ��_qlabel
*/
void Interface::setLabelImage(QLabel*& _qlabel)
{
          cv::Mat& r_image(this->getImageFrame(this->m_writeLock));
          if (true) {
                    this->detectFaceScaleOnly(m_writeLock, r_image);                        //��Ĭ��û����������������¼������
          }
          QImage _display = mat2Qimage(r_image);
          _display = _display.scaled(_qlabel->width(), _qlabel->height());                 //����ؼ��ֱ���
          _qlabel->setPixmap(QPixmap::fromImage(_display));
}

/*
 *  QTWidget����Ƶ�����������
 * @name: QTVideoOutput
 * @function������������ʾͼ������ʾ��ͼ��
*/
void Interface::QTVideoOutput(QLabel*& _qlabel)
{
          while (!m_videoFlag) {
                    this->setLabelImage(_qlabel);                                                     //����ؼ��ֱ���
          }
          _qlabel->clear();
}