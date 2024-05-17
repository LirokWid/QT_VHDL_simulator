#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QMouseEvent>
#include <QPushButton>
#include <QScrollBar>
#include <QWidget>
#include <QTextEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QAction>

/**
 * @brief The Severity enum
 */
enum class Severity {
    Info,
    Success,
    Warning,
    Error,
    Debug
};


/**
 * @brief The DebugWindow class provides a widget for displaying debug messages.
 */
class DebugWindow : public QWidget
{
    Q_OBJECT
public:

    static DebugWindow* getInstance(QAction *openTrigger = nullptr, QWidget *parent = nullptr);

    /**
     * @brief Adds a debug message to the window.
     * @param message The debug message to be added.
     * @param severity The severity of the message.
     */
    void addMessage(const QString& message, Severity severity = Severity::Info);

    /**
     * @brief Opens the debug window.
     */
    void openWindow();

protected:
    /**
     * @brief Scrolls down to show the latest message.
     */
    void scrollDown();

private:
    explicit DebugWindow(QAction *openTrigger, QWidget *parent = nullptr);
    static DebugWindow* instance;


    unsigned int messageCount;  /**< The number of messages currently displayed. */
    bool autoScrollEnabled;     /**< Flag indicating whether auto-scrolling is enabled. */

    QAction *openTrigger;       /**< The QAction that triggers opening the debug window. */
    QTextEdit *textEdit;        /**< The QTextEdit widget for displaying debug messages. */
    QList<QString> messages;    /**< List of debug messages. */
    QCheckBox *autoScrollBox;   /**< Checkbox for enabling/disabling auto-scrolling. */
    QScrollBar *scrollBar;      /**< Scrollbar for the QTextEdit widget. */

    /**
     * @brief Scrolls to the bottom if auto-scrolling is enabled.
     */
    void autoScrollIfEnabled();

    /**
     * @brief Handles the state change of the auto-scroll checkbox.
     * @param state The new state of the checkbox.
     */
    void autoScrollHandle(int state);

    /**
     * @brief Handles the scrollbar value change event.
     */
    void scrollBarModifiedHandle();

    /**
     * @brief Clears all messages from the debug window.
     */
    void clearMessages();
};

#endif // DEBUGWINDOW_H
