#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include <QResizeEvent>
#include <QSize>
#include <QPaintEvent>
#include <QAction>
<<<<<<< Updated upstream
=======
#include <QtDebug>
#include <QDebug>
#include <qpainter.h>
#include <QTextBlock>
#include <QFile>
#include <QTextStream>
#include "highlighter.h"
#include <QMessageBox>
#include <QFileDialog>

class LineNumberArea;
class CodeEditor;

>>>>>>> Stashed changes
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    CodeEditor *text1;
    Highlighter *highlighter;
    void setUpHighlighter();

private:
    QString filename;
    QString savefilename;


private :
<<<<<<< Updated upstream
    QTextEdit *text1;
=======
>>>>>>> Stashed changes
    QMenu *file;
    QMenu *edit;
    QMenu *build;
    QMenu *help;

    QAction *file_open;
    QAction *file_exit;
    QAction *help_about;
    QAction *edit_copy;
    QAction *edit_cut;
    QAction *edit_paste;
    QAction *select_all;
    QAction *file_save;
    QAction *build_compile;
    QAction *build_run;

private slots:
    void on_open();
    void on_about();
    void on_exit();
    void on_copy();
    void on_cut();
    void on_paste();
    void on_selectall();
    void on_save();

    void on_compile();
    void on_run();

};
//我把文本编辑框从TextEdit换成了PlainTextEdit
class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    CodeEditor(QWidget *parent = 0);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    QWidget *lineNumberArea;

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

};
//行数框
class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        CodeEditor = editor;
    }

    QSize sizeHint() const Q_DECL_OVERRIDE {
        return QSize(CodeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE {
    CodeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *CodeEditor;
};

#endif // MAINWINDOW_H
