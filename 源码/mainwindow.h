#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDebug>
#include <QStringList>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QString filename;
    QString savefilename;
    QString content;

private :
    QTextEdit *text1;
    QMenu *file;
    QMenu *edit;
    QMenu *build;
    QMenu *help;

    QStringList openedFiles;

    QAction *file_new;
    QAction *file_open;
    QAction *file_exit;
    QAction *help_about;

    QAction *edit_copy;
    QAction *edit_cut;
    QAction *edit_paste;

    QAction *select_all;
    QAction *file_save;
    QAction *file_save_as;

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
    void on_save_as();
    void on_new();
    void on_compile();
    void on_run();

};

#endif // MAINWINDOW_H
