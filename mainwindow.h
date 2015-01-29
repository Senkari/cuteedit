#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QActionGroup>
#include <QTranslator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setDocumentChanged();
    void setUndoButtonAvailable(bool available);
    void setRedoButtonAvailable(bool available);
    void setCutButtonAvailable(bool available);
    void setCopyButtonAvailable(bool available);
    void setCurrentTabName(QString name);
    void setCurrentTabTooltip(QString tooltip);

private slots:
    virtual void closeEvent(QCloseEvent* event);

    virtual void changeEvent(QEvent* event);

    void on_actionAbout_Qt_triggered();

    void on_actionNew_triggered();

    bool on_tabWidget_tabCloseRequested(int index);

    void on_actionQuit_triggered();

    void on_tabWidget_currentChanged(int index);

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionOpen_triggered();

    void on_actionPaste_triggered();

    void on_actionSelect_All_triggered();

    void on_languageGroup_triggered(QAction*);

    void on_highlightGroup_triggered(QAction*);

private:
    Ui::MainWindow *ui;
    QMenu *languageMenu;
    QActionGroup *highlightGroup;
    QActionGroup *languageGroup;
    QTranslator *qtTranslator;
    QTranslator *appTranslator;
    QString currentLanguage;

    void createHighlightMenu();
};

#endif // MAINWINDOW_H
