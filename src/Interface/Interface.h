#include"../ImageProcess/ImageProcess.h"
#include"../DBProcess/DBProcess.h"

class Interface :public DBProcess, public ImageProcess {
public:
          Interface() = delete;
          Interface(int TrainningSetting);
          virtual ~Interface();

          /*
           *  QImage��ʽͼ���������,ͼ���ڿռ�����ʾ
           * @name: QTVideoDisplayModul
           * @function����MAT����ת��ΪQT��QImage����
           * @param ����QImage
          */
          void QTVideoDisplayModule(QLabel*& _qlabel, QImage*& _display);

private:

};