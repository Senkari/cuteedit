#include <QMessageBox>
#include <QClipboard>
#include <QMimeData>
#include <QFileDialog>
#include <QDirIterator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qscieditor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qtTranslator = new QTranslator();
    appTranslator = new QTranslator();

    qtTranslator->load(QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(qtTranslator);
    appTranslator->load("languages/" + QLocale::system().name());
    qApp->installTranslator(appTranslator);

    //New action group for our languages
    languageGroup = new QActionGroup(ui->actionLanguage);
    languageGroup->setExclusive(true);

    //Connect language group triggered signal to function
    connect(languageGroup, SIGNAL(triggered(QAction*)), this, SLOT(on_languageGroup_triggered(QAction*)));


    //Scan language directory for available languages
    languageMenu = new QMenu(this);
    ui->actionLanguage->setMenu(languageMenu);
    QDirIterator dirIt("languages");
    while(dirIt.hasNext()){
        QFileInfo fileInfo(dirIt.next());
        if(fileInfo.suffix() == "qm"){
            QString locale = fileInfo.baseName();
            QString language = QLocale::languageToString(QLocale(locale).language());
            QAction *action = new QAction(language, this);
            action->setCheckable(true);
            action->setData(locale);
            languageMenu->addAction(action);
            languageGroup->addAction(action);
            if(locale == QLocale::system().name()){
                currentLanguage = locale;
                action->setChecked(true);
            }

        }
    }

    //Set highlight
    highlightGroup = new QActionGroup(ui->menuHighlight);
    highlightGroup->setExclusive(true);

    //Connect highlight group triggered signal to function
    connect(highlightGroup, SIGNAL(triggered(QAction*)), this, SLOT(on_highlightGroup_triggered(QAction*)));

    this->createHighlightMenu();


    //Set all icons to be themed according to your linux desktop ixon theme. If this icon theme is not found, use fallback tango icons instead.
    ui->actionNew->setIcon(QIcon::fromTheme("document-new", QIcon(":/ui/document-new.svg")));
    ui->actionOpen->setIcon(QIcon::fromTheme("document-open", QIcon(":/ui/document-open.svg")));
    ui->actionSave->setIcon(QIcon::fromTheme("document-save", QIcon(":/ui/document-save.svg")));
    ui->actionSave_as->setIcon(QIcon::fromTheme("document-save-as", QIcon(":/ui/document-save-as.svg")));
    ui->actionCopy->setIcon(QIcon::fromTheme("edit-copy", QIcon(":/ui/edit-copy.svg")));
    ui->actionCut->setIcon(QIcon::fromTheme("edit-cut", QIcon(":/ui/edit-cut.svg")));
    ui->actionPaste->setIcon(QIcon::fromTheme("edit-paste", QIcon(":/ui/edit-paste.svg")));
    ui->actionPrint->setIcon(QIcon::fromTheme("document-print", QIcon(":/ui/document-print.svg")));
    ui->actionSelect_All->setIcon(QIcon::fromTheme("edit-select-all", QIcon(":/ui/edit-select-all.svg")));
    ui->actionUndo->setIcon(QIcon::fromTheme("edit-undo", QIcon(":/ui/edit-undo.svg")));
    ui->actionRedo->setIcon(QIcon::fromTheme("edit-redo", QIcon(":/ui/edit-redo.svg")));
    ui->actionFind->setIcon(QIcon::fromTheme("edit-find", QIcon(":/ui/edit-find.svg")));
    ui->actionFind_Replace->setIcon(QIcon::fromTheme("edit-find-replace", QIcon(":/ui/edit-find-replace.svg")));
    ui->actionQuit->setIcon(QIcon::fromTheme("system-log-out", QIcon(":/ui/System-log-out2.svg")));
    ui->actionZoom_In->setIcon(QIcon::fromTheme("view-zoom-in", QIcon(":/ui/View-zoom-in.svg")));
    ui->actionZoom_Out->setIcon(QIcon::fromTheme("view-zoom-out", QIcon(":/ui/View-zoom-out.svg")));
    ui->actionPreferences->setIcon(QIcon::fromTheme("preferences-system", QIcon(":/ui/preferences-system.svg")));

    //Set some buttons disabled
    ui->actionSave->setDisabled(true);
    ui->actionSave_as->setDisabled(true);
    ui->actionPrint->setDisabled(true);
    ui->actionFind->setDisabled(true);
    ui->actionFind_Replace->setDisabled(true);
    ui->actionCopy->setDisabled(true);
    ui->actionPaste->setDisabled(true);
    ui->actionCut->setDisabled(true);
    ui->actionSelect_All->setDisabled(true);
    ui->actionZoom_In->setDisabled(true);
    ui->actionZoom_Out->setDisabled(true);
    ui->actionUndo->setDisabled(true);
    ui->actionRedo->setDisabled(true);
    ui->menuHighlight->setDisabled(true);

}

MainWindow::~MainWindow()
{

    delete ui;
    delete languageMenu;
    delete languageGroup;
    delete qtTranslator;
    delete appTranslator;
    delete highlightGroup;
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_actionNew_triggered()
{
    QsciEditor *editor = new QsciEditor(this);
    ui->tabWidget->addTab(editor,tr("Unnamed"));

    this->setWindowTitle(tr("Unnamed - CuteEdit"));

    const QClipboard *clipboard = qApp->clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if(mimeData->hasText()){
        ui->actionPaste->setEnabled(true);
    }
    else{
        ui->actionPaste->setEnabled(false);
    }

    if(!ui->actionSave->isEnabled()) ui->actionSave->setEnabled(true);
    if(!ui->actionSave_as->isEnabled()) ui->actionSave_as->setEnabled(true);
    if(!ui->actionSelect_All->isEnabled()) ui->actionSelect_All->setEnabled(true);
    if(!ui->menuHighlight->isEnabled()) ui->menuHighlight->setEnabled(true);
}

bool MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    QsciEditor* editor = (QsciEditor*)ui->tabWidget->widget(index);
    if(editor->hasChanged()){
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Save document?"), tr("Do you want to save changes made to document ") + ui->tabWidget->tabToolTip(index) + tr("?"), QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(reply == QMessageBox::Yes){
            if(!editor->save()){
                return false;
            }
        }
        else if(reply == QMessageBox::No){
        }
        else{
            return false;
        }
    }
    delete editor;
    return true;
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::closeEvent(QCloseEvent *event){
    bool ignoreEvent = false;

    while(ui->tabWidget->count() > 0){
        if(!this->on_tabWidget_tabCloseRequested(0)){
            ignoreEvent = true;
            break;
        }

    }

    if(ignoreEvent){
        event->ignore();
    }
    else{
        event->accept();
    }
}

void MainWindow::setDocumentChanged(){
    QString tabText = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    tabText.append("*");
    this->setWindowTitle(tabText + tr(" - CuteEdit"));
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), tabText);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(ui->tabWidget->count() > 0){
        QsciEditor *editor = (QsciEditor*)ui->tabWidget->currentWidget();
        this->setWindowTitle(ui->tabWidget->tabText(ui->tabWidget->currentIndex()) + tr(" - CuteEdit"));
        this->setUndoButtonAvailable(editor->undoAvailable());
        this->setRedoButtonAvailable(editor->redoAvailable());
    }
    else{
        ui->actionSave->setDisabled(true);
        ui->actionSave_as->setDisabled(true);
        ui->actionUndo->setDisabled(true);
        ui->actionRedo->setDisabled(true);
        ui->actionCopy->setDisabled(true);
        ui->actionPaste->setDisabled(true);
        ui->actionCut->setDisabled(true);
        ui->actionSelect_All->setDisabled(true);
        ui->menuHighlight->setDisabled(true);
    }
}

void MainWindow::setUndoButtonAvailable(bool available){
    ui->actionUndo->setEnabled(available);
}

void MainWindow::setRedoButtonAvailable(bool available){
    ui->actionRedo->setEnabled(available);
}

void MainWindow::setCopyButtonAvailable(bool available){
    ui->actionCopy->setEnabled(available);
}

void MainWindow::setCutButtonAvailable(bool available){
    ui->actionCut->setEnabled(available);
}


void MainWindow::on_actionUndo_triggered()
{
    QsciEditor *editor = (QsciEditor*)ui->tabWidget->currentWidget();
    editor->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    QsciEditor *editor = (QsciEditor*)ui->tabWidget->currentWidget();
    editor->redo();
}

void MainWindow::on_actionCopy_triggered()
{
    QsciEditor *editor = (QsciEditor*)ui->tabWidget->currentWidget();
    editor->copy();

    const QMimeData *mimeData = qApp->clipboard()->mimeData();
    if(mimeData->hasText()){
        ui->actionPaste->setEnabled(true);
    }
    else{
        ui->actionPaste->setEnabled(false);
    }
}

void MainWindow::on_actionCut_triggered()
{
    QsciEditor *editor = (QsciEditor*)ui->tabWidget->currentWidget();
    editor->cut();

    const QMimeData *mimeData = qApp->clipboard()->mimeData();
    if(mimeData->hasText()){
        ui->actionPaste->setEnabled(true);
    }
    else{
        ui->actionPaste->setEnabled(false);
    }
}

void MainWindow::on_actionSave_triggered()
{
    QsciEditor *editor = (QsciEditor*)ui->tabWidget->currentWidget();
    editor->save();
}

void MainWindow::on_actionSave_as_triggered()
{
    QsciEditor *editor = (QsciEditor*)ui->tabWidget->currentWidget();
    editor->saveAs();
}

void MainWindow::setCurrentTabName(QString name){
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), name);
    this->setWindowTitle(name + tr(" - CuteEdit"));
}

void MainWindow::setCurrentTabTooltip(QString tooltip){
    ui->tabWidget->setTabToolTip(ui->tabWidget->currentIndex(), tooltip);
}

void MainWindow::on_actionOpen_triggered()
{
    QStringList fileList = QFileDialog::getOpenFileNames(this, tr("Open File(s)"), QString(), tr("All Files *.*"));

    if(!fileList.isEmpty()){
        for(int i = 0; i < fileList.count(); i++){
            QsciEditor *editor = new QsciEditor(this);
            editor->open(fileList.at(i));
            QFile file(fileList.at(i));
            QFileInfo fileInfo(file);
            int newTab = ui->tabWidget->addTab(editor, fileInfo.fileName());
            ui->tabWidget->setTabToolTip(newTab, file.fileName());
            file.close();

            const QClipboard *clipboard = qApp->clipboard();
            const QMimeData *mimeData = clipboard->mimeData();
            if(mimeData->hasText()){
                ui->actionPaste->setEnabled(true);
            }
            else{
                ui->actionPaste->setEnabled(false);
            }

            if(!ui->actionSave->isEnabled()) ui->actionSave->setEnabled(true);
            if(!ui->actionSave_as->isEnabled()) ui->actionSave_as->setEnabled(true);
            if(!ui->actionSelect_All->isEnabled()) ui->actionSelect_All->setEnabled(true);
        }

        this->setWindowTitle(ui->tabWidget->tabText(ui->tabWidget->currentIndex()) + tr(" - CuteEdit"));
    }
}

void MainWindow::on_actionPaste_triggered()
{
    QsciEditor *editor = (QsciEditor*)ui->tabWidget->currentWidget();
    editor->paste();
}

void MainWindow::on_actionSelect_All_triggered()
{
    QsciEditor *editor = (QsciEditor*)ui->tabWidget->currentWidget();
    editor->selectAll();
}

void MainWindow::on_languageGroup_triggered(QAction* action){
    if(action != 0){
       if(currentLanguage != action->data().toString()){
           currentLanguage = action->data().toString();
           QLocale locale = QLocale(currentLanguage);
           QLocale::setDefault(locale);
           QString languageName = QLocale::languageToString(locale.language());
           qApp->removeTranslator(appTranslator);
           if(appTranslator->load("languages/" + currentLanguage + ".qm")){
               qApp->installTranslator(appTranslator);
           }
           ui->statusBar->showMessage(tr("Language changed to ") + languageName);
       }
    }
}

void MainWindow::changeEvent(QEvent *event){
    if(event != 0){
        if(event->type() == QEvent::LanguageChange){
            ui->retranslateUi(this);
        }
        else if(event->type() == QEvent::LocaleChange){

        }
    }

    QMainWindow::changeEvent(event);
}

void MainWindow::on_highlightGroup_triggered(QAction* action){
    QsciEditor* editor = (QsciEditor*)ui->tabWidget->currentWidget();
    if(action != 0){
        if(action->data() != editor->getLexerLanguage()){
            editor->setLexerLanguage(action->data().toString());
        }
    }
}

void MainWindow::createHighlightMenu(){
    QAction *action;

    //None
    action = new QAction(tr("None"), this);
    action->setCheckable(true);
    action->setData("none");
    highlightGroup->addAction(action);
    ui->menuHighlight->addAction(action);

    //Separator
    ui->menuHighlight->addSeparator();

    //C++
    action = new QAction(tr("C++"), this);
    action->setCheckable(true);
    action->setData("cpp");
    highlightGroup->addAction(action);
    ui->menuHighlight->addAction(action);

    //C#
    action = new QAction(tr("C#"), this);
    action->setCheckable(true);
    action->setData("csharp");
    highlightGroup->addAction(action);
    ui->menuHighlight->addAction(action);

    //Separator
    ui->menuHighlight->addSeparator();

    //HTML
    action = new QAction(tr("HTML"), this);
    action->setCheckable(true);
    action->setData("html");
    highlightGroup->addAction(action);
    ui->menuHighlight->addAction(action);

    //Separator
    ui->menuHighlight->addSeparator();

    //Java
    action = new QAction(tr("Java"), this);
    action->setCheckable(true);
    action->setData("java");
    highlightGroup->addAction(action);
    ui->menuHighlight->addAction(action);

    //Javascript
    action = new QAction(tr("Javascript"), this);
    action->setCheckable(true);
    action->setData("javascript");
    highlightGroup->addAction(action);
    ui->menuHighlight->addAction(action);
}

