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
 * @brief The DebugWindow class provides a widget for displaying debug messages.
 */
class DebugWindow : public QWidget
{
    Q_OBJECT
public:

    /**
     * @brief The Severity enum
     */
    enum Severity
    {
        Info,
        Success,
        Warning,
        Error,
        Debug
    };

    static DebugWindow* getInstance(QAction *openTrigger = nullptr, QWidget *parent = nullptr);

    /**
     * @brief Adds a message to the debug window.
     * @param message The debug message to be added.
     * @param severity The severity of the message.
     */
    void addMessage(const QString& message, Severity severity = Info);

    /**
     * @brief addDebug
     * @param message
     */
    void addDebug(const QString& message);

    /**
     * @brief addSuccess
     * @param message
     */
    void addSuccess(const QString& message);

    /**
     * @brief addWaring
     * @param message
     */
    void addWaring(const QString& message);
    /**
     * @brief addError
     * @param message
     */
    void addError(const QString& message);
    /**
     * @brief addInfo
     * @param message
     */
    void addInfo(const QString& message);

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
    QList<QString> messages;
    QList<Severity> severities;

    bool autoScrollEnabled = true;     /**< Flag indicating whether auto-scrolling is enabled. */
    QCheckBox *autoScrollBox;   /**< Checkbox for enabling/disabling auto-scrolling. */
    QScrollBar *scrollBar;      /**< Scrollbar for the QTextEdit widget. */

    QAction *openTrigger;       /**< The QAction that triggers opening the debug window. */
    QTextEdit *textEdit;        /**< The QTextEdit widget for displaying debug messages. */

    QCheckBox *infoCheckBox;
    QCheckBox *successCheckBox;
    QCheckBox *warningCheckBox;
    QCheckBox *errorCheckBox;
    QCheckBox *debugCheckBox;


    void filterMessages();

    bool shouldAppend(Severity severity);

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
