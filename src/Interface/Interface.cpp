#include"Interface.h"

Interface::Interface(int TrainningSetting)
          :ImageProcess(TrainningSetting), DBProcess()
{

}

Interface::~Interface()
{

}

/*
 *  QImage格式图像输出函数,图像在空间上显示
 * @name: QTVideoDisplayModul
 * @function：将MAT类型转换为QT的QImage类型
 * @param 输入QImage
*/
void QTVideoDisplayModule(QLabel*& _qlabel, QImage*& _display)
{
          while (true) {
                    //QImage pp = mat2Qimage(getImageFrame(this->m_cameraMutex));
                    *_display = _display->scaled(_qlabel->width(), _qlabel->height());                 //适配控件分辨率
                    _qlabel->setPixmap(QPixmap::fromImage(*_display));
          }
}