// debugwindow.cpp
#include "debugwindow.h"
#include "params.h"

#include <QLabel>

DebugWindow* DebugWindow::instance = nullptr;


DebugWindow::DebugWindow(QAction *openTrigger, QWidget *parent) :
    QWidget(parent),
    messageCount(0),
    openTrigger(openTrigger)
{
    /** ui setup ********************************************************************/
    setWindowTitle("Debug Window");
    setWindowFlags(Qt::Window);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    textEdit = new QTextEdit(this);
    textEdit->setStyleSheet("QTextEdit { background-color: #212121; color: white; }");
    scrollBar = textEdit->verticalScrollBar();
    textEdit->setReadOnly(true);
    mainLayout->addWidget(textEdit);


    QHBoxLayout *checkBoxLayout = new QHBoxLayout(this);

    autoScrollBox = new QCheckBox(tr("Autoscroll"), this);
    autoScrollBox->setChecked(true);
    checkBoxLayout->addWidget(autoScrollBox, 0, Qt::AlignLeft);

    QPushButton *clearButton = new QPushButton(tr("Clear"), this);
    checkBoxLayout->addWidget(clearButton, 0, Qt::AlignLeft);

    // Spacer item to push severity checkboxes to the right
    QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    checkBoxLayout->addItem(spacer);

    infoCheckBox = new QCheckBox("Show Info", this);
    infoCheckBox->setChecked(true);
    checkBoxLayout->addWidget(infoCheckBox, 0, Qt::AlignRight);

    successCheckBox = new QCheckBox("Show Success", this);
    successCheckBox->setChecked(true);
    checkBoxLayout->addWidget(successCheckBox, 0, Qt::AlignRight);

    debugCheckBox = new QCheckBox("Show debug", this);
    debugCheckBox->setChecked(true);
    checkBoxLayout->addWidget(debugCheckBox, 0, Qt::AlignRight);

    warningCheckBox = new QCheckBox("Show Warning", this);
    warningCheckBox->setChecked(true);
    checkBoxLayout->addWidget(warningCheckBox, 0, Qt::AlignRight);

    errorCheckBox = new QCheckBox("Show Error", this);
    errorCheckBox->setChecked(true);
    checkBoxLayout->addWidget(errorCheckBox, 0, Qt::AlignRight);

    mainLayout->addLayout(checkBoxLayout);
    /********************************************************************************/

    connect(this->openTrigger,  &QAction::triggered,        this, &DebugWindow::openWindow);
    connect(clearButton,        &QPushButton::clicked,      this, &DebugWindow::clearMessages);
    connect(autoScrollBox,      &QCheckBox::stateChanged,   this, &DebugWindow::autoScrollHandle);
    connect(scrollBar,          &QScrollBar::valueChanged,  this, &DebugWindow::scrollBarModifiedHandle);

    connect(infoCheckBox,       &QCheckBox::stateChanged,   this, &DebugWindow::filterMessages);
    connect(successCheckBox,    &QCheckBox::stateChanged,   this, &DebugWindow::filterMessages);
    connect(debugCheckBox,      &QCheckBox::stateChanged,   this, &DebugWindow::filterMessages);
    connect(warningCheckBox,    &QCheckBox::stateChanged,   this, &DebugWindow::filterMessages);
    connect(errorCheckBox,      &QCheckBox::stateChanged,   this, &DebugWindow::filterMessages);

    autoScrollIfEnabled();
}

DebugWindow* DebugWindow::getInstance(QAction *openTrigger, QWidget *parent)
{
    if (instance == nullptr) {
        instance = new DebugWindow(openTrigger, parent);
    }
    return instance;
}

bool DebugWindow::shouldAppend(Severity severity)
{
    bool shouldAppend = false;
    switch (severity)
    {
    case Debug:
        shouldAppend = debugCheckBox->isChecked();
        break;
    case Success:
        shouldAppend = successCheckBox->isChecked();
        break;
    case Warning:
        shouldAppend = warningCheckBox->isChecked();
        break;
    case Error:
        shouldAppend = errorCheckBox->isChecked();
        break;
    default: // Info
        shouldAppend = infoCheckBox->isChecked();
        break;
    }
    return shouldAppend;
}
void DebugWindow::filterMessages()
{
    textEdit->clear();
    for (int i = 0; i < messages.size(); ++i)
    {
        if (shouldAppend(severities[i]))
        {
            QString color, strSeverity;
            switch (severities[i])
            {
            case Debug:
                color = "gray";
                strSeverity = "Debug: ";
                break;
            case Success:
                color = "green";
                strSeverity = "Success: ";
                break;
            case Warning:
                color = "orange";
                strSeverity = "Warning: ";
                break;
            case Error:
                color = "red";
                strSeverity = "ERROR: ";
                break;
            default: // Info
                color = "white";
                strSeverity = "";
                break;
            }

            QString formattedMessage = QString("<font color=\"%1\">%2</font>")
                                           .arg(color, strSeverity + messages[i]);

            textEdit->append(formattedMessage);
        }
    }
}

void DebugWindow::addMessage(const QString &message, Severity severity /* = Info */)
{
    if (messages.size() >= MAX_DEBUG_MESSAGES_NB)
    {
        messages.removeFirst();
        severities.removeFirst();
        --messageCount;
    }

    messages.append(message);
    severities.append(severity);
    ++messageCount;

    QString color, strSeverity;
    switch (severity)
    {
    case Debug:
        color = "gray";
        strSeverity = "Debug: ";
        break;
    case Success:
        color = "green";
        strSeverity = "Success: ";
        break;
    case Warning:
        color = "orange";
        strSeverity = "Warning: ";
        break;
    case Error:
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


    if (shouldAppend(severity))
    {
        textEdit->append(formattedMessage);
    }

    qDebug().noquote() << strSeverity << message;
    autoScrollIfEnabled();
}

void DebugWindow::addDebug(const QString &message)
{
    addMessage(message,Debug);
}

void DebugWindow::addSuccess(const QString &message)
{
    addMessage(message,Success);
}

void DebugWindow::addWaring(const QString &message)
{
    addMessage(message,Warning);
}

void DebugWindow::addError(const QString &message)
{
    addMessage(message,Error);
}

void DebugWindow::addInfo(const QString &message)
{
    addMessage(message,Info);
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
