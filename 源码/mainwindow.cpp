#include "mainwindow.h"
#include "QMessageBox"
#include "QFileDialog"
#include <QStringList>
#include <QTextDocumentWriter>
#include <QTabWidget>

int flag_isNew = 0;

int flag_isOpen = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //设置字体大小
    text1=new QTextEdit;
    QFont f;
    f.setPixelSize(24);
    text1->setFont(f);

    //设置字体颜色
    QColor c;
    c.setRgb(169,169,169);
    text1->setTextColor(c);

    //将text1放到对话框中
    this->setCentralWidget(text1);

    //在菜单栏中添加功能
    file=this->menuBar()->addMenu("文件");
    edit=this->menuBar()->addMenu("编辑");
    build=this->menuBar()->addMenu("构建");
    help=this->menuBar()->addMenu("帮助");

    file_new=new QAction("新建",this);
    file_new->setShortcut(tr("Ctrl+N"));
    file->addAction(file_new);

    file_open=new QAction("打开",this);
    file_open->setShortcut(tr("Ctrl+O"));
    file->addAction(file_open);

    file_save=new QAction("保存",this);
    file_save->setShortcut(tr("Ctrl+S"));
    file->addAction(file_save);

    file_save_as=new QAction("另存为",this);
    file->addAction(file_save_as);

    file_exit=new QAction("退出",this);
    file->addSeparator();
    file->addAction(file_exit);

    //建立一个子菜单选项-关于
    help_about=new QAction("关于",this);
    help_about->setShortcut(tr("Ctrl+H"));
    help->addAction(help_about);

    edit_copy=new QAction("复制",this);
    edit->addAction(edit_copy);

    //建立一个子菜单选项-剪切
    edit_cut=new QAction("剪切",this);
    edit->addAction(edit_cut);

    //建立一个子菜单选项-粘贴
    edit_paste=new QAction("粘贴",this);
    edit->addAction(edit_paste);

    //建立一个子菜单选项-全选
    select_all=new QAction("全选",this);
    edit->addAction(select_all);

    //建立一个子菜单选项-编译
    build_compile=new QAction("编译",this);
    build->addAction(build_compile);

    //建立一个子菜单选项-运行
    build_run=new QAction("运行",this);
    build->addAction(build_run);
    build_run->setShortcut(tr("Ctrl+R"));

    //Qt的消息槽机制
    connect(file_open,SIGNAL(triggered()),this,SLOT(on_open()));
    //第一个参数是触发事件的控件，第二个参数是对于Action来说的固定写法
    //第三个参数固定写this，第四个参数指定点击Action后执行那个函数
    connect(help_about,SIGNAL(triggered()),this,SLOT(on_about()));
    connect(file_exit,SIGNAL(triggered()),this,SLOT(on_exit()));
    connect(edit_copy,SIGNAL(triggered()),this,SLOT(on_copy()));
    connect(edit_cut,SIGNAL(triggered()),this,SLOT(on_cut()));
    connect(edit_paste,SIGNAL(triggered()),this,SLOT(on_paste()));
    connect(select_all,SIGNAL(triggered()),this,SLOT(on_selectall()));
    connect(file_save,SIGNAL(triggered()),this,SLOT(on_save()));
    connect(build_compile,SIGNAL(triggered()),this,SLOT(on_compile()));
    connect(build_run,SIGNAL(triggered()),this,SLOT(on_run()));
    connect(file_save_as,SIGNAL(triggered()),this,SLOT(on_save_as()));
    connect(file_new,SIGNAL(triggered()),this,SLOT(on_new()));
}
MainWindow::~MainWindow()
{
    delete text1;
}

void MainWindow::on_new()
{
    text1->clear();
    text1->setHidden(false);
    flag_isNew = 1;
    flag_isOpen = 1;
}

void MainWindow::on_open()
{
    QStringList files;
    files = QFileDialog::getOpenFileNames(this, tr("Open files..."), QString(),
                                          tr("All Files(*);;Text Files(*.txt);;Cpp Files(*.cpp *.c *.h"));

    if(files.isEmpty())
    {
        return;
    }
    filename = files.at(0);
    if (QFile::exists(filename))
    {
        QFile file(filename);
        if (file.open(QFile::ReadOnly))
        {
            QByteArray data = file.readAll();
            text1->setPlainText(QString::fromLocal8Bit(data));
        }
    }
    flag_isOpen = 1;
}
void MainWindow::on_about()
{
    QMessageBox::information(this,"帮助","QQ：2031295812");
}

void MainWindow::on_exit()
{
    exit(0);
}

void MainWindow::on_copy()
{
    text1->copy();
}

void MainWindow::on_cut()
{
   text1->cut();
}

void MainWindow::on_paste()
{
    text1->paste();
}

void MainWindow::on_selectall()
{
    text1->selectAll();
}

void MainWindow::on_save_as()
{
     savefilename=QFileDialog::getSaveFileName(this, tr("Save as..."), QString(),
                                               tr("All Files (*);;Plain text Files(*.txt);;Cpp Files(*.cpp);;C Files(*.c);;Head Files(*.h)"));
     if(savefilename.isEmpty())
     {
         return;
     }

     QFile file(savefilename);
     if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
     {
         QMessageBox::information(this,"错误","打开文件失败");
         return;
     }
     else
     {
         QTextStream textStream(&file);
         QString str = text1->toPlainText();
         textStream<<str;
         QMessageBox::warning(this,"提示","保存成功");
         flag_isNew = 0;
         file.close();
     }
}

void MainWindow::on_save()
{
    if(flag_isNew)
    {
        if(text1->toPlainText()=="")
        {
            QMessageBox::warning(this,"error","content can not be none!",QMessageBox::Ok);
        }
        else
        {
            QFileDialog fileDialog;
            QString str = fileDialog.getSaveFileName(this,"Open File","","Text File(*.txt)");
            if(str == "")
            {
                return;
            }
            QFile filename(str);
            if(!filename.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(this,"error","Open File Error!");
                return;
             }
             else
            {
                QTextStream textStream(&filename);
                QString str = text1->toPlainText();
                textStream << str;
                content =str;
            }
            QMessageBox::information(this,"Ssve File","Save File Success",QMessageBox::Ok);
            filename.close();
            flag_isNew = 0;
            savefilename = str;
        }
    }
    else
    {
        if(flag_isOpen)
        {
            QFile file(filename);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(this,"error","Open File Faile");
                return;
            }
            QTextStream textString(&file);
            QString str = text1->toPlainText();
            textString << str;
            content = str;
            file.close();
        }
        else
        {
            QMessageBox::warning(this,"Warning","Please new or open a file");
            return;
        }
    }
}

void MainWindow::on_compile()
{
    QString dest=filename;

    QString dest1 =  dest.replace(".c","");
    //qDebug() << dest;

    int i = system(("gcc -o "+ dest +" "+ filename).toStdString().data());
    //qDebug()<<i;
    QMessageBox::information(this,"notify","build success!");

}
void MainWindow::on_run()
{
    QString desfilename=filename;
    //qDebug() << desfilename;
    desfilename.replace(".c","");
    system(("start "+desfilename).toLatin1().data());

}
