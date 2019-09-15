#ifndef COMPLETE_H
#define COMPLETE_H


#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include <QDebug>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPlainTextEdit>
#include <QApplication>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>

using std::string;
using std::vector;
using std::min;

class CompleteListWidget : public QListWidget
{
public:
  CompleteListWidget(QWidget *parent=0);
  static int ldistance(const string source, const string target);
protected:
  void keyPressEvent(QKeyEvent *event) override;
private:
  QPlainTextEdit* p;
  QColor backgroundColor;
  QColor highlightColor;
};


class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);

    void setUpCompleteList();

protected:
    //void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void showCompleteWidget();

private:
    QWidget *lineNumberArea;
    QColor lineColor;
    QColor editorColor;
    QStringList completeList;//储存自动填充的关键字
    //QListWidget *completeWidget;
    CompleteListWidget *completeWidget;
    QString getWordOfCursor();
    int completeState;
    int getCompleteWidgetX();
};


enum CompleteState{
  Ignore=0,
  Showing=1,
  Hide=2
};








#endif // COMPLETE_H
