// debugwindow.cpp
#include "debugwindow.h"
#include "qscrollbar.h"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QAction>
#include <QMouseEvent>
#include <QPushButton>
#include <QCheckbox>


DebugWindow::DebugWindow(QAction *openTrigger, QWidget *parent) :
    QWidget(parent),
    openTrigger(openTrigger)
{
    /** ui setup ********************************************************************/
    setWindowTitle("Debug Window");
    setWindowFlags(Qt::Window);

    QVBoxLayout *vLayout = new QVBoxLayout(this);

    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    vLayout->addWidget(textEdit);


    QHBoxLayout *hLayout = new QHBoxLayout(this);

    autoScrollBox = new QCheckBox(tr("Autoscroll"), this);
    autoScrollBox->setChecked(true);

    hLayout->addWidget(autoScrollBox, 0, Qt::AlignLeft);
    QPushButton *clearButton = new QPushButton(tr("Clear"), this);
    hLayout->addWidget(clearButton, 0, Qt::AlignRight);

    vLayout->addLayout(hLayout);
    /********************************************************************************/

    connect(this->openTrigger, &QAction::triggered, this, &DebugWindow::openWindow);
    connect(clearButton, &QPushButton::clicked, this, &DebugWindow::clearMessages);
    connect(autoScrollBox, &QCheckBox::stateChanged, this, &DebugWindow::autoScrollHandle);
}


void DebugWindow::addMessage(const QString &message)
{
    QString newText = textEdit->toPlainText() + message + "\n";
    // Truncate if necessary to keep only the last MAX_MESSAGES_NB lines
    int extraLines = newText.count('\n') - MAX_MESSAGES_NB;
    if (extraLines > 0)
    {
        int index = newText.indexOf('\n', extraLines) + 1;
        newText = newText.mid(index);
    }
    textEdit->setPlainText(newText);

    if (autoScrollEnabled)
    {
        /*
        // Scroll to the bottom
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::Start);
        textEdit->setTextCursor(cursor);
*/
        QScrollBar *scrollBar = textEdit->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
        //TODO finish this
    }
}

void DebugWindow::autoScrollHandle(int state)
{
    autoScrollEnabled = (state == Qt::Checked);
}

void DebugWindow::openWindow()
{
    this->show();
    if(autoScrollEnabled)
    {
        // Scroll to the bottom
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::Start);
        textEdit->setTextCursor(cursor);
    }
}

void DebugWindow::clearMessages()
{
    textEdit->clear();
}
