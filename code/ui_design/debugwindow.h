// debugwindow.h
#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QCheckbox>

class DebugWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DebugWindow(QAction *openTrigger, QWidget *parent = nullptr);

    void addMessage(const QString& message);
    void openWindow();

private:
    void clearMessages();
    QAction *openTrigger;
    QTextEdit *textEdit;
    QList<QString> messages;
    QCheckBox *autoScrollBox;
    bool autoScrollEnabled;
    void autoScrollHandle(int state);

    const int MAX_MESSAGES_NB = 50;
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
};

#endif // DEBUGWINDOW_H
