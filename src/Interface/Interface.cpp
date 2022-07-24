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
 *  QTWidget层视频关闭函数
 * @name: QTVideoOutput
 * @function：本质是关闭视频中的循环参数
*/
void  Interface::videoClose()
{
          m_videoFlag = true;
}

/*
 *  QTWidget层开启当前帧拍照操作
 * @name: CameraShooting
 * @function：本质是拍摄当前的单帧人脸
*/
void  Interface::videoCameraShooting()
{
          this->enableCameraShooting();
}

/*
 *  QTWidget层开启当前帧的保存操作
 * @name: videoFrameSavingProcess
 * @function：本质是保存当前的单帧人脸
*/
void  Interface::videoFrameSavingProcess()
{
          this->enableSavingProcess();
}

/*
 *  QTWidget层开启当前帧的舍弃操作
 * @name: videoFrameIgnoreProcess
 * @function：本质是舍弃当前的单帧人脸
*/
void  Interface::videoFrameIgnoreProcess()
{
          this->enableIgnoreProcess();
}

/*
 *  QLabel图层覆写函数
 * @name: setLabelImage
 * @function：将MAT类型转换为QT的QImage类型并覆盖到QLabel容器中
 * @param 1.输入右值类型的 QImage&
 *                2.输入显示图层的指针_qlabel
*/
void Interface::setLabelImage(QImage& _qimage, QLabel*& _qlabel)
{
          _qimage = _qimage.scaled(_qlabel->width(), _qlabel->height());                 //适配控件分辨率
          _qlabel->setPixmap(QPixmap::fromImage(_qimage));
}

/*
 *  QTWidget层视频连续输出函数
 * @name: QTVideoOutput
 * @function：连续更新显示图层所显示的图像
 * @param: 1. 视频输出pixmap : QLabel*& _qlabel
 *                 2. 输出窗口接口：QTextBrowser*& _systemOutput
*/
void Interface::QTVideoOutput(QLabel*& _qlabel, QTextBrowser*& _systemOutput)
{
          while (!m_videoFlag) {
                    this->setLabelImage(this->startVideoDisplay(_systemOutput), _qlabel);                       //适配控件分辨率
          }
          _qlabel->clear();
}