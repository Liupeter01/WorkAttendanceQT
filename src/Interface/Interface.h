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

private:
          std::atomic<bool> m_videoFlag = false;                      //视频开关
};