#include"Interface.h"

Interface::Interface(int TrainningSetting)
          :ImageProcess(TrainningSetting), DBProcess()
{

}

Interface::~Interface()
{

}

/*
 *  QImage��ʽͼ���������,ͼ���ڿռ�����ʾ
 * @name: QTVideoDisplayModul
 * @function����MAT����ת��ΪQT��QImage����
 * @param ����QImage
*/
void QTVideoDisplayModule(QLabel*& _qlabel, QImage*& _display)
{
          while (true) {
                    //QImage pp = mat2Qimage(getImageFrame(this->m_cameraMutex));
                    *_display = _display->scaled(_qlabel->width(), _qlabel->height());                 //����ؼ��ֱ���
                    _qlabel->setPixmap(QPixmap::fromImage(*_display));
          }
}