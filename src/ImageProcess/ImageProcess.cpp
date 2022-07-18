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
 * ��ʼ��ͼ�δ�������ڴ�
 * @name: initImageProcess
*/
bool ImageProcess::initImageProcess()
{
          this->initFaceDetection();
          this->initResnetModel();
}

/*
 * �ͷ�ͼ�δ�������ڴ�
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
 * @function����MAT����ת��ΪQT��QImage����
 * @param ����ԭʼͼ��  const cv::Mat& mat
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