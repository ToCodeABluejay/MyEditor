#include "myeditor.h"
#include "ui_myeditor.h"

MyEditor::MyEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyEditor)
{
    ui->setupUi(this);
    ui->centralwidget->layout()->setMargin(0);
    ui->textEdit->setMarginLineNumbers(1, true);
    ui->textEdit->setCaretLineBackgroundColor(QColorConstants::DarkGray);
}

MyEditor::~MyEditor()
{
    delete ui;
}

void MyEditor::open_file(QString filename)
{
    file.setFileName(filename);
    file.open(QIODevice::ReadWrite);
    QTextStream in(&file);
    just_opened = true, edited = false;
    ui->textEdit->setText(in.readAll());
    set_name();
}

void MyEditor::load_settings()
{
    if(settings.value("highlight").toBool())
        ui->actionHighlight_current_line->setChecked(true);
    else
        ui->actionHighlight_current_line->setChecked(false);
    if(settings.value("linenum").toBool())
        ui->actionInclude_line_numbers->setChecked(true);
    else
        ui->actionInclude_line_numbers->setChecked(false);
    on_actionHighlight_current_line_triggered();
    on_actionInclude_line_numbers_triggered();
}

void MyEditor::open()
{
    QString of = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath());
    open_file(of);
}

void MyEditor::save()
{
    file.write(ui->textEdit->text().toUtf8());
    just_opened = false;
    edited = false;
    set_name();
}

void MyEditor::set_name()
{
    if (file.isOpen())
    {
        if (just_opened)
            setWindowTitle(file.fileName());
        else
        {
            if (!file.isWritable() && edited)
                setWindowTitle("* " + file.fileName() + " [Read-Only]");
            else if (!file.isWritable())
                setWindowTitle(file.fileName() + " [Read-Only]");
            else if (edited)
                setWindowTitle("* " + file.fileName());
            else
                setWindowTitle(file.fileName());
        }
    }
    else if (edited)
        setWindowTitle("* " + MyEditor::objectName());
}

bool MyEditor::unsaved_changes()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Unsaved changes!", "You have unsaved changes! Are you sure you want to proceed?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
        return true;
    else
        return false;
}

void MyEditor::resize_margins()
{
    /*Dynamically resize line margins according
     * to document length*/
    int i=1, j=10;

    while (floor(ui->textEdit->lines()/j)>9)
    {
        i++, j*=10;
    }

    j=i*10;
    ui->textEdit->setMarginWidth(1,10+j);
}

void MyEditor::on_textEdit_textChanged()
{
    if (!just_opened)
        edited = true;
    else
        edited = false;
    set_name();
    just_opened = false;

    resize_margins();
}

void MyEditor::on_actionNew_triggered()
{
    if (unsaved_changes())
    {
        file.close();
        ui->textEdit->clear();
        set_name();
    }
}

void MyEditor::on_actionOpen_triggered()
{
    if (!edited)
        open();
    else
        if (unsaved_changes())
            open();
}

void MyEditor::on_actionSave_triggered()
{
    if (edited && file.isWritable())
        save();
    else if (!file.isOpen())
        on_actionSave_as_triggered();
    else if (!file.isWritable())
    {
        QMessageBox messageBox;
        messageBox.warning(0, "Read-Only!", "You do not have permission to write to this file! Try saving to a different file or open again with root privileges!");
    }
}

void MyEditor::on_actionSave_as_triggered()
{
    file.close();
    file.setFileName(QFileDialog::getSaveFileName(this, "Save as...", QDir::homePath()));
    file.open(QIODevice::ReadWrite);
    save();
}

void MyEditor::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MyEditor::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MyEditor::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MyEditor::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MyEditor::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MyEditor::on_actionInclude_line_numbers_triggered()
{
    if (ui->actionInclude_line_numbers->isChecked())
    {
        resize_margins();
        settings.setValue("linenum", true);
    }
    else
    {
        ui->textEdit->setMarginWidth(1,0);
        settings.setValue("linenum", false);
    }
}

void MyEditor::on_actionHighlight_current_line_triggered()
{
    if (ui->actionHighlight_current_line->isChecked())
    {
        ui->textEdit->setCaretLineVisible(true);
        settings.setValue("highlight", true);
    }
    else
    {
        ui->textEdit->setCaretLineVisible(false);
        settings.setValue("highlight", false);
    }
}
