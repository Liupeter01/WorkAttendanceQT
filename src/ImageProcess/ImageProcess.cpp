#include"ImageProcess.h"

ImageProcess::ImageProcess(int TrainningSetting)
          :FaceDetecion(), ModelTrain(TrainningSetting), TakePicture() {
          m_threadPool.emplace_back(&ImageProcess::initImageProcess, this);
}

ImageProcess::~ImageProcess()
{
          this->releaseImageProcess();
          for (auto& i : m_threadPool) {
                    if (i.joinable()) {
                              i.join();
                    }
          }
}

/*
 * 初始化图形处理类的内存
 * @name: initImageProcess
*/
bool ImageProcess::initImageProcess()
{
          this->initFaceDetection();
          this->initResnetModel();
}

/*
 * 释放图形处理类的内存
 * @name:releaseImageProcess;
*/
void ImageProcess::releaseImageProcess()
{
          this->releaseResnetModel();
          this->releaseFaceDetection();
}

/*
 * cv::Mat ----> QImage
 * @name: mat2Qimage
 * @function：将MAT类型转换为QT的QImage类型
 * @param 输入原始图像  const cv::Mat& mat
*/
QImage ImageProcess::mat2Qimage(const cv::Mat& mat)
{
          switch (mat.type()){
                    case CV_8UC4:                   // 8-bit, 4 channel
                    {
                              QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32);
                              return image;
                    }
                    case CV_8UC3:                            // 8-bit, 3 channel
                    {
                              QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
                              return image.rgbSwapped();
                    }
                    case CV_8UC1:                              // 8-bit, 1 channel
                    {
                              static QVector<QRgb>  sColorTable;                    // only create our color table once
                              if (sColorTable.isEmpty()) {
                                        for (int i = 0; i < 256; ++i)
                                                  sColorTable.push_back(qRgb(i, i, i));
                              }
                              QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
                              image.setColorTable(sColorTable);
                              return image;
                    }

          default:
                    qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
                    break;
          }
          return QImage();
}