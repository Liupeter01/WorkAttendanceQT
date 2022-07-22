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
 * 设置数据已经被线程模块同步
 * @name:setDataSyncSignal;
 * @function: 用来指示其他线程有关于数据更新的状态
*/
void  ImageProcess::setDataSyncSignal()
{
          this->m_imageSync.second = true;
          this->m_faceRectSync.second = true;
}

/*
 * 设置数据当前的输入数据模块数据过时
 * @name: resetDataSyncSignal;
 * @function: 用来指示其他线程有关于数据更新的状态
*/
void  ImageProcess::resetDataSyncSignal()
{
          this->m_imageSync.second = false;
          this->m_faceRectSync.second = false;
}

/*
 * 获取当前数据是否为最新状态
 * @name: resetDataSyncSignal;
 * @function: 用来指示其他线程有关于数据更新的状态
 * @retValue: 1.true : 当前数据的状态为旧状态
 *                    2.false: 当前数据的状态为新状态
*/
bool  ImageProcess::getDataSyncStatus()
{
          return  this->m_imageSync.second && this->m_faceRectSync.second;
}

/*
 * 暂停屏幕显示线程的工作，等待用户对于图像的确认函数
 * @name: getUserConfirmStatus
 * @function: 当训练线程启动后，需要等待用户完成图像确认之后才可以继续显示
 * @retValue: 1.true : 允许继续的下一帧的显示
 *                    2.false: 不允许下一帧的显示
*/
bool  ImageProcess::getUserConfirmStatus()
{
          return true;
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
          //条件变量设置：可能需要等待其他模块的信号，才可以继续进行执行
          {
                    std::unique_lock<std::mutex> _lckg(this->m_CofirmUserInptutEvent);
                    this->m_UserInput.wait(_lckg, [=]() {
                              return getUserConfirmStatus();
                     });
          }
          this->resetDataSyncSignal();                                                                                                                                 //设定当前数据为过期状态
          this->m_imageSync.first = this->getImageFrame(this->m_imageRWLock);                                                        //共享人脸图像
          this->m_faceRectSync.first= this->getFaceRectangle(this->m_imageSync.first, this->m_faceRectRWLock);    //共享人脸位置

           //信号设置：基础数据已经准备完毕，其他线程模块可以访问
          this->setDataSyncSignal();                                                                                                                                      //设定当前数据为最新状态
          this->drawGeometryOnImage(this->m_imageRWLock, this->m_faceRectSync.first, "");
          return mat2Qimage(this->m_imageSync.first);
}

/*
 * 通过摄像头图像进行实时人脸训练功能模块
 * @name: realTimeFacialDisplay
 * @function：和实时摄像头图像协同进行人脸训练功能
*/
void ImageProcess::videoSyncFacialTranning()
{
          //do
          //{
          //          if (true) {
          //                    continue;
          //          }
          //} while (externalInput());
}