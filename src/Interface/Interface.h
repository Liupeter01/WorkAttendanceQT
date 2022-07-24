#include"../ImageProcess/ImageProcess.h"
#include"../DBProcess/DBProcess.h"

class Interface :public DBProcess, public ImageProcess {
public:
          Interface() = delete;
          Interface(int TrainningSetting);
          virtual ~Interface();

          /*
           *  QTWidget层视频关闭函数
           * @name: QTVideoOutput
           * @function：本质是关闭视频中的循环参数
          */
          void  videoClose(); 

          /*
           *  QTWidget层开启当前帧拍照操作
           * @name: CameraShooting
           * @function：本质是拍摄当前的单帧人脸
          */
          void  videoCameraShooting();

          /*
           *  QTWidget层开启当前帧的保存操作
           * @name: videoFrameSavingProcess
           * @function：本质是保存当前的单帧人脸
          */
          void  videoFrameSavingProcess();

          /*
           *  QTWidget层开启当前帧的舍弃操作
           * @name: videoFrameIgnoreProcess
           * @function：本质是舍弃当前的单帧人脸
          */
          void  videoFrameIgnoreProcess();

          /*
           *  QTWidget层视频连续输出函数
           * @name: QTVideoOutput
           * @function：连续更新显示图层所显示的图像
           * @param: 1.输入QImage
          */
          void QTVideoOutput(QLabel*& _qlabel);

private:
          /*
           *  QLabel图层覆写函数
           * @name: setLabelImage
           * @function：将MAT类型转换为QT的QImage类型并覆盖到QLabel容器中
           * @param 1.输入右值类型的 QImage&
           *                2.输入显示图层的指针_qlabel
          */
          void setLabelImage(QImage& _qimage, QLabel*& _qlabel);

protected:
          std::atomic<bool> m_videoFlag = false;                      //视频开关
};