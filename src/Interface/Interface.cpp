#include"Interface.h"

Interface::Interface(int TrainningSetting)
          :ImageProcess(TrainningSetting), DBProcess()
{
          //此时的ImageProcess类已经被内部创建，无须再次初始化!!!
}

Interface::~Interface()
{
}

/*
 *  QLabel图层覆写函数
 * @name: setLabelImage
 * @function：将MAT类型转换为QT的QImage类型并覆盖到QLabel容器中
 * @param 1.输入QImage
 *                2.输入显示图层的指针_qlabel
*/
void Interface::setLabelImage(QLabel*& _qlabel)
{
          cv::Mat& r_image(this->getImageFrame(this->m_writeLock));
          if (true) {
                    this->detectFaceScaleOnly(m_writeLock, r_image);                        //在默认没有摁键操作的情况下检测人脸
          }
          QImage _display = mat2Qimage(r_image);
          _display = _display.scaled(_qlabel->width(), _qlabel->height());                 //适配控件分辨率
          _qlabel->setPixmap(QPixmap::fromImage(_display));
}

/*
 *  QTWidget层视频连续输出函数
 * @name: QTVideoOutput
 * @function：连续更新显示图层所显示的图像
*/
void Interface::QTVideoOutput(QLabel*& _qlabel)
{
          while (!m_videoFlag) {
                    this->setLabelImage(_qlabel);                                                     //适配控件分辨率
          }
          _qlabel->clear();
}