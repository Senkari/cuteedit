#include <QFont>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QTextEdit>

#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexercsharp.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerjavascript.h>

#include "editor.h"
#include "ui_editor.h"

QEditor::QEditor(MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::QEditor),
    changed(false),
    mainwindow(parent)
{
    ui->setupUi(this);
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(on_textEdit_textChanged()));
    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), this, SLOT(on_textEdit_copyAvailable(bool b)));
    QFont font("Courier", 10, QFont::Normal);
    ui->textEdit->setFont(font);
    ui->textEdit->setMarginsFont(font);
    QFontMetrics fontmetrics = QFontMetrics(font);
    ui->textEdit->setMarginWidth(0, fontmetrics.width("00000"));
    ui->textEdit->setMarginLineNumbers(0, true);
    ui->textEdit->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    ui->textEdit->setTabWidth(4);
    ui->textEdit->setAutoIndent(true);
    ui->textEdit->setLexer(new QsciLexerCPP(this));
}

QEditor::~QEditor()
{
    qDebug() << "Deleting QEditor at " << this;
    delete ui;
}

void QEditor::on_textEdit_textChanged()
{
    if(!changed){
        mainwindow->setDocumentChanged();
        changed = true;
    }

    if(ui->textEdit->isUndoAvailable()){
        mainwindow->setUndoButtonAvailable(true);
    }
    else{
        mainwindow->setUndoButtonAvailable(false);
    }

    if(ui->textEdit->isRedoAvailable()){
        mainwindow->setRedoButtonAvailable(true);
    }
    else{
        mainwindow->setRedoButtonAvailable(false);
    }

}

void QEditor::undo(){
    ui->textEdit->undo();
}

void QEditor::redo(){
    ui->textEdit->redo();
}

void QEditor::copy(){
    ui->textEdit->copy();
}

void QEditor::cut(){
    ui->textEdit->cut();
}

void QEditor::paste(){
    ui->textEdit->paste();
}

bool QEditor::copyAvailable(){

    return false;
}

bool QEditor::undoAvailable(){
    return ui->textEdit->isUndoAvailable();
}

bool QEditor::redoAvailable(){
    return ui->textEdit->isRedoAvailable();
}

void QEditor::on_textEdit_copyAvailable(bool b){
    mainwindow->setCutButtonAvailable(b);
    mainwindow->setCopyButtonAvailable(b);
}

void QEditor::open(QString file){
    filePath = file;
    QFile openFile(file);
    if(openFile.open(QIODevice::ReadOnly)){
        QTextStream readFile(&openFile);
        ui->textEdit->setText(readFile.readAll());
        openFile.close();
        changed = false;

    }
}

bool QEditor::save(){
    QString savePath;
    if(filePath == QString()){
        savePath = QFileDialog::getSaveFileName(mainwindow, tr("Save"), QString(), tr("All Files *.*"));
    }
    else savePath = filePath;
    if(!savePath.isEmpty()){
        QFile file(savePath);
        if(file.open(QIODevice::WriteOnly)){
            QTextStream writeFile(&file);
            writeFile << ui->textEdit->text();
            file.close();
            filePath = savePath;
            changed = false;

            QFileInfo fileInfo(file);
            mainwindow->setCurrentTabName(fileInfo.fileName());
            mainwindow->setCurrentTabTooltip(file.fileName());
            return true;
        }
        else{
            QMessageBox::critical(mainwindow, tr("Error"), tr("Couldn't save. Do you have permissions to the file?"));
            return false;
        }
    }
    else return false;
}

bool QEditor::saveAs(){
    QString savePath = QFileDialog::getSaveFileName(mainwindow, tr("Save"), QString(), tr("All Files *.*"));
    if(!savePath.isEmpty()){
        QFile file(savePath);
        if(file.open(QIODevice::WriteOnly)){
            QTextStream writeFile(&file);
            writeFile << ui->textEdit->text();
            file.close();
            filePath = savePath;
            changed = false;

            QFileInfo fileInfo(file);
            mainwindow->setCurrentTabName(fileInfo.fileName());
            mainwindow->setCurrentTabTooltip(file.fileName());
            return true;
        }
        else{
            QMessageBox::critical(mainwindow, tr("Error"), tr("Couldn't save. Do you have permissions to the file?"));
            return false;
        }
    }
    else return false;
}

void QEditor::selectAll(){
    ui->textEdit->selectAll();
}

bool QEditor::hasChanged(){
    return changed;
}
