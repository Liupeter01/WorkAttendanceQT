#include"ImageProcess.h"

ImageProcess::ImageProcess(int TrainningSetting)
          :FaceDetecion(), ModelTrain(TrainningSetting), TakePicture() {
          m_threadPool.emplace_back(&ImageProcess::initImageProcess, this);
}

ImageProcess::~ImageProcess()
{
          //this->releaseImageProcess();
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
          return this->initFaceDetection() && this->initResnetModel();
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
 * @retValue : 返回位于本类中QImage&引用类型
 * @Correction: 2022-7-21 在类内部引入QImage临时存储结构，可以使用引用加速
 *                        2022-7-21 删除函数中多余的部分，仅仅保留CV8U_C3下的处理方式，并引入内联函数
*/
inline QImage& ImageProcess::mat2Qimage(const cv::Mat& mat)
{
          return  (m_qimageFrameStore = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped());
}

/*
 * 实时摄像头图像+人脸检测+人脸识别输出显示接口
 * @name: realTimeFacialDisplay
 * @function：其他的功能调用通过与该程序连接的线程进行操作
 * @param  1. 图像修改读写锁  std::mutex& _writeMutex
 *                  2. 输入原始图像   cv::Mat& mat
 * 
 * @retValue: QImage &
*/
QImage &ImageProcess::realTimeFacialDisplay()
{
          this->m_sharedData.first = this->getImageFrame(this->m_imageRWLock);             //共享人脸图像
          this->m_sharedData.second = this->getFaceRectangle(this->m_sharedData.first);    //共享人脸位置
          //信号设置
          this->drawGeometryOnImage(this->m_imageRWLock, this->m_sharedData.second);
          return mat2Qimage(this->m_sharedData.first);
}