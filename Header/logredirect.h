#ifndef LOGREDIRECT_H
#define LOGREDIRECT_H

#include <QPlainTextEdit>
#include <streambuf>
#include <iostream>

class LogRedirect : public std::streambuf {
public:
    explicit LogRedirect(QPlainTextEdit* textWidget) : m_textWidget(textWidget) {}

protected:
    virtual int overflow(int c) override {
        if (c == '\n') {
            m_textWidget->appendPlainText(""); // Move to new line
        } else {
            m_textWidget->insertPlainText(QString(QChar(c)));
        }
        m_textWidget->moveCursor(QTextCursor::End);
        return c;
    }

private:
    QPlainTextEdit* m_textWidget;
};

#endif // LOGREDIRECT_H
