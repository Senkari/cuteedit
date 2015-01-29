#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>

#include "mainwindow.h"

namespace Ui {
class QEditor;
}

class QEditor : public QWidget
{
    Q_OBJECT

public:
    explicit QEditor(MainWindow *parent = 0);
    ~QEditor();

    void undo();
    void redo();
    void copy();
    void cut();
    void paste();
    void open(QString file);
    void selectAll();

    bool hasChanged();
    bool save();
    bool saveAs();
    bool copyAvailable();
    bool undoAvailable();
    bool redoAvailable();

private slots:
    void on_textEdit_textChanged();
    void on_textEdit_copyAvailable(bool b);

private:
    Ui::QEditor *ui;
    bool changed;
    MainWindow *mainwindow;
    QString filePath;
    QString fileName;

};

#endif // EDITOR_H
