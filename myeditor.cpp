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

QString MyEditor::get_filename()
{
    return filename;
}

void MyEditor::set_filename(QString fn)
{
    filename = fn;
}

void MyEditor::open_file()
{
    file.setFileName(get_filename());
    file.open(QIODevice::ReadWrite);
    set_writeable(file.isWritable());
    QTextStream in(&file);
    just_opened = true, edited = false;
    ui->textEdit->setText(in.readAll());
    file.close();
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
    set_filename(QFileDialog::getOpenFileName(this, "Open File", QDir::homePath()).toUtf8());
    open_file();
}

void MyEditor::save()
{
    file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    /*QTextStream out(&file);
    out << ui->textEdit->text().toUtf8();*/
    file.write(ui->textEdit->text().toUtf8());
    file.close();
    just_opened = false;
    edited = false;
    set_name();
}

void MyEditor::set_name()
{
    if (!get_filename().isEmpty())
    {
        if (just_opened)
            setWindowTitle(get_filename());
        else
        {
            if (!is_writeable() && edited)
                setWindowTitle("* " + get_filename() + " [Read-Only]");
            else if (!is_writeable())
                setWindowTitle(get_filename() + " [Read-Only]");
            else if (edited)
                setWindowTitle("* " + get_filename());
            else
                setWindowTitle(get_filename());
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

bool MyEditor::is_writeable()
{
    return writeable;
}

void MyEditor::set_writeable(bool write)
{
    writeable = write;
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
    if (edited && is_writeable())
        save();
    else if (get_filename().isEmpty())
        on_actionSave_as_triggered();
    else if (!is_writeable())
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
