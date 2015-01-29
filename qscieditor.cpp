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

#include "qscieditor.h"
#include "ui_qscieditor.h"

QsciEditor::QsciEditor(MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::QsciEditor),
    changed(false),
    mainwindow(parent)
{
    ui->setupUi(this);

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(on_textEdit_textChanged()));
    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), this, SLOT(on_textEdit_copyAvailable(bool)));
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

QsciEditor::~QsciEditor()
{
    delete ui;
}

void QsciEditor::on_textEdit_textChanged()
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

void QsciEditor::undo(){
    ui->textEdit->undo();
}

void QsciEditor::redo(){
    ui->textEdit->redo();
}

void QsciEditor::copy(){
    ui->textEdit->copy();
}

void QsciEditor::cut(){
    ui->textEdit->cut();
}

void QsciEditor::paste(){
    ui->textEdit->paste();
}

bool QsciEditor::copyAvailable(){

    return false;
}

bool QsciEditor::undoAvailable(){
    return ui->textEdit->isUndoAvailable();
}

bool QsciEditor::redoAvailable(){
    return ui->textEdit->isRedoAvailable();
}

void QsciEditor::on_textEdit_copyAvailable(bool b){
    mainwindow->setCutButtonAvailable(b);
    mainwindow->setCopyButtonAvailable(b);
}

void QsciEditor::open(QString file){
    filePath = file;
    QFile openFile(file);
    if(openFile.open(QIODevice::ReadOnly)){
        QTextStream readFile(&openFile);
        ui->textEdit->setText(readFile.readAll());
        openFile.close();
        changed = false;

    }
}

bool QsciEditor::save(){
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

bool QsciEditor::saveAs(){
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

void QsciEditor::selectAll(){
    ui->textEdit->selectAll();
}

bool QsciEditor::hasChanged(){
    return changed;
}

QString QsciEditor::getLexerLanguage(){
    return lexerLang;
}

void QsciEditor::setLexerLanguage(QString lang){

}

