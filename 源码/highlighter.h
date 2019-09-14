#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE


class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;//关键字 规则
    QTextCharFormat classFormat;//类 规则
    QTextCharFormat singleLineCommentFormat;//注释 规则
    QTextCharFormat multiLineCommentFormat;//注释
    QTextCharFormat quotationFormat;//头文件
    QTextCharFormat functionFormat;//函数
};


#endif // HIGHLIGHTER_H
