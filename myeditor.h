#ifndef MYEDITOR_H
#define MYEDITOR_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <Qsci/qsciscintilla.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MyEditor; }
QT_END_NAMESPACE

class MyEditor : public QMainWindow
{
    Q_OBJECT

public:
    MyEditor(QWidget *parent = nullptr);
    ~MyEditor();

private slots:
    void open();

    void set_name();

    void save();

    bool unsaved_changes();

    void on_textEdit_textChanged();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionInclude_line_numbers_triggered();

    void on_actionHighlight_current_line_triggered();

private:
    Ui::MyEditor *ui;
    QFile file;
    /*I tried using the default 'isModified()' function provided, but it proved insufficient
     *  for some reason further investigation required*/
    bool edited;
    bool just_opened = true;
};
#endif // MYEDITOR_H
