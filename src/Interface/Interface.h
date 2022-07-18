#include"../ImageProcess/ImageProcess.h"
#include"../DBProcess/DBProcess.h"

class Interface :public DBProcess, public ImageProcess {
public:
          Interface() = delete;
          Interface(int TrainningSetting);
          virtual ~Interface();

          /*
           *  QImage格式图像输出函数,图像在空间上显示
           * @name: QTVideoDisplayModul
           * @function：将MAT类型转换为QT的QImage类型
           * @param 输入QImage
          */
          void QTVideoDisplayModule(QLabel*& _qlabel, QImage*& _display);

private:

};