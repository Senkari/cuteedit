#ifndef QSCIEDITOR_H
#define QSCIEDITOR_H

#include <QWidget>

#include "mainwindow.h"

namespace Ui {
class QsciEditor;
}

class QsciEditor : public QWidget
{
    Q_OBJECT

public:
    explicit QsciEditor(MainWindow *parent = 0);
    ~QsciEditor();
    void undo();
    void redo();
    void copy();
    void cut();
    void paste();
    void open(QString file);
    void selectAll();
    void setLexerLanguage(QString lang);

    bool hasChanged();
    bool save();
    bool saveAs();
    bool copyAvailable();
    bool undoAvailable();
    bool redoAvailable();

    QString getLexerLanguage();

private slots:
    void on_textEdit_textChanged();
    void on_textEdit_copyAvailable(bool b);

private:
    Ui::QsciEditor *ui;
    bool changed;
    MainWindow *mainwindow;
    QString filePath;
    QString fileName;
    QString lexerLang;
};

#endif // QSCIEDITOR_H
