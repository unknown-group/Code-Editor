#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    this->setFixedSize(1000,800);

    //文本框
    text1 = new CodeEditor(this);
    text1->setGeometry(0,30,1000,600);

    //信息返回框
    browser = new QPlainTextEdit(this);
    browser->setGeometry(0,630,1000,170);

    //设置高亮
    setUpHighlighter();

    //设置字体大小
    QFont f;
    f.setPixelSize(24);
    text1->setFont(f);
    browser->setFont(f);

    //设置字体颜色
    QColor c;
    c.setRgb(255,255,255);
    text1->setPalette(c);
    browser->setPalette(c);

    //设置背景颜色
    text1->setStyleSheet("background:#ffffff;");
    browser->setStyleSheet("background:#ffffff;");

    //在菜单栏中添加功能
    file=this->menuBar()->addMenu("文件");
    edit=this->menuBar()->addMenu("编辑");
    build=this->menuBar()->addMenu("构建");
    help=this->menuBar()->addMenu("帮助");

    //建立一个子菜单选项-打开
    file_open=new QAction("打开",this);
    //添加快捷键
    file_open->setShortcut(tr("Ctrl+O"));
    //将file_open这个action加入到file菜单中
    file->addAction(file_open);

    //建立一个子菜单选项-保存
    file_save=new QAction("保存",this);
    //添加快捷键
    file_save->setShortcut(tr("Ctrl+S"));
    //将file_save这个action加入到file菜单中
    file->addAction(file_save);


    file_exit=new QAction("退出",this);
    //添加分隔符
    file->addSeparator();
    file->addAction(file_exit);

    //建立一个子菜单选项-关于
    help_about=new QAction("关于",this);
    //添加快捷键
    help_about->setShortcut(tr("Ctrl+H"));
    //将help_about
    help->addAction(help_about);

    //建立一个子菜单选项—复制
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

}
MainWindow::~MainWindow()
{
    delete text1;
}
void MainWindow::on_open()
{
    filename=QFileDialog::getOpenFileName();
    //函数返回路径+文件名
    qDebug()<<filename;
    if(filename.isEmpty())
    {
        return;
    }

    QString content;
    //QMessageBox::information(this,"提示","test");

    //filename.toStdString().data();
    //这个语句将QString类型转化为const char *类型
    //需要将QString转换为const char *
    FILE *p=fopen(filename.toStdString().data(),"r");//需要将QString转换为const char *
    if(p==NULL)
    {
        QMessageBox::information(this,"错误","打开文件失败");
    }
    else
    {
        while(!feof(p))
        {
            //将读取到的内容追加到content后面
            char buf[1024]={0};
            fgets(buf,sizeof(buf),p);
            content+=buf;
        }
      fclose(p);
      //将字符串的值放到text里面
      text1->setPlainText(content);
    }
}
void MainWindow::on_about()
{
    QMessageBox::information(this,"帮助","QQ：123");
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

void MainWindow::on_save()
{
     savefilename=QFileDialog::getSaveFileName();
     if(savefilename==NULL)
     {
         return;
     }
     FILE *p=fopen(savefilename.toStdString().data(),"w");
     if(p==NULL)
     {
         QMessageBox::information(this,"错误","打开文件失败");
         return;
     }
     else
     {
         // text1->toPlainText().toStdString().data();
         //将用户在控件中输入的字符串转化为const char *
         fputs(text1->toPlainText().toStdString().data(),p);
         fclose(p);
     }
}
void MainWindow::on_compile()
{
    browser->clear();
    QString dest=filename;
    dest.replace(".c","");
    cmd = new QProcess;
    //QStringList arguments;
    //arguments<<"gcc -o "<<dest<<" "<<filename<<"\n";

    cmd->start("cmd");
    cmd->waitForStarted();
    cmd->write(("gcc -o "+ dest +" "+ filename+"\n").toStdString().data());
    cmd->waitForFinished(3000);//阻塞3秒后退出，程序不能编译太久
    QString strTemp=QString::fromLocal8Bit(cmd->readAllStandardError());
    cmd->kill();

    //qDebug() << dest;
    //int i = system(("gcc -o "+ dest +" "+ filename).toStdString().data());
    //qDebug()<<i;

    if (!strTemp.isEmpty())
        browser->appendPlainText(strTemp.toStdString().data());
    else
        browser->appendPlainText("build success!");
}
//不能再外部程序运行中输入，如果代码有scanf之类的就会有问题
void MainWindow::on_run()
{
    browser->clear();
    QString desfilename=filename;
    QString strTemp;

    //qDebug() << desfilename;

    desfilename.replace(".c",".exe");
    cmd->start("cmd");
    cmd->waitForStarted();
    cmd->write((desfilename+"\n").toStdString().data());

    cmd->waitForFinished(3000);
    strTemp=QString::fromLocal8Bit(cmd->readAll());
    cmd->kill();
    browser->appendPlainText(strTemp.toStdString().data());

    //system(("start "+desfilename).toLatin1().data());

}
//新文本框的定义
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
}

//行高亮
void CodeEditor::highlightCurrentLine()
{
   QList<QTextEdit::ExtraSelection> extraSelections;
   QTextEdit::ExtraSelection selection;
   QColor lineColor = QColor(Qt::red).lighter(160);
   selection.format.setBackground(lineColor);
   selection.format.setProperty(QTextFormat::FullWidthSelection, true);
   selection.cursor = textCursor();
   selection.cursor.clearSelection();
   extraSelections.append(selection);
   setExtraSelections(extraSelections);
}
//更新行号显示栏的宽度设置
void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}
//更新行号显示栏的宽度
void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}
//计算行号显示栏的宽度
int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}
//放大的时候从新定位行号显示栏
void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}
//绘制行数显示框
void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(-2, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
//设置语法高亮
void MainWindow::setUpHighlighter(){
  QFont font;
  font.setFamily("Courier");
  font.setFixedPitch(true);
  //font.setPointSize(20);
  text1->setFont(font);
  text1->setTabStopWidth(fontMetrics().width(QLatin1Char('9'))*4);
  highlighter=new Highlighter(text1->document());
}
