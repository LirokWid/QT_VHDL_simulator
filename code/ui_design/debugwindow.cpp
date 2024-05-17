// debugwindow.cpp
#include "debugwindow.h"
#include "params.h"


DebugWindow::DebugWindow(QAction *openTrigger, QWidget *parent) :
    QWidget(parent),
    messageCount(0),
    autoScrollEnabled(true),
    openTrigger(openTrigger)
{
    /** ui setup ********************************************************************/
    setWindowTitle("Debug Window");
    setWindowFlags(Qt::Window);

    QVBoxLayout *vLayout = new QVBoxLayout(this);

    textEdit = new QTextEdit(this);
    textEdit->setStyleSheet("QTextEdit { background-color: #212121; color: white; }");
    scrollBar = textEdit->verticalScrollBar();
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
    connect(scrollBar, &QScrollBar::valueChanged, this, &DebugWindow::scrollBarModifiedHandle);
}


void DebugWindow::addMessage(const QString &message, Severity severity /* = Severity::Info */)
{
    QString color, strSeverity;
    switch (severity)
    {
    case Severity::Debug:
        color = "gray";
        strSeverity = "Debug: ";
        break;
    case Severity::Success:
        color = "green";
        strSeverity = "Success: ";
        break;
    case Severity::Warning:
        color = "orange";
        strSeverity = "Warning: ";
        break;
    case Severity::Error:
        color = "red";
        strSeverity = "ERROR: ";
        break;
    default: // Info
        color = "white";
        strSeverity = "";
        break;
    }

    QString formattedMessage = QString("<font color=\"%1\">%2</font>")
                                   .arg(color, strSeverity + message);

    if(messageCount >= MAX_DEBUG_MESSAGES_NB)
    {
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::Start);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.deleteChar();
        cursor.movePosition(QTextCursor::End);
        --messageCount;
    }

    textEdit->append(formattedMessage);
    qDebug().noquote() << strSeverity << message;
    ++messageCount;
    autoScrollIfEnabled();
}

void DebugWindow::autoScrollIfEnabled()
{
    if(autoScrollEnabled)
    {
        textEdit->ensureCursorVisible();
        scrollBar->setValue(scrollBar->maximum());
    }
}

void DebugWindow::autoScrollHandle(int state)
{
    autoScrollEnabled = (state == Qt::Checked);
}

void DebugWindow::scrollBarModifiedHandle()
{
    if (scrollBar->value() == scrollBar->maximum())
    {// If the user scrolls to the bottom, enable auto-scroll
        autoScrollEnabled = true;
        autoScrollBox->setChecked(true);
    }else
    {// If the user scrolls up, disable auto-scroll
        autoScrollEnabled = false;
        autoScrollBox->setChecked(false);
    }
}

void DebugWindow::openWindow()
{
    this->show();
    autoScrollIfEnabled();
}

void DebugWindow::clearMessages()
{
    textEdit->clear();
    messageCount = 0;
}
