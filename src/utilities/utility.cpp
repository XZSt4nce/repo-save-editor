#include "stdafx.h"
#include "utility.h"

JsonHighlighter::JsonHighlighter( QTextDocument* parent ) : QSyntaxHighlighter(parent)
{
	initializeRules();
	updateColors(QGuiApplication::styleHints()->colorScheme());
}

void JsonHighlighter::initializeRules()
{
	// Rule for JSON keys
	highlightingRules.append({ QRegularExpression(R"(\".*?\"(?=\s*:))"), QTextCharFormat() });

	// Rule for JSON string values
	highlightingRules.append({ QRegularExpression(R"(:\s*\".*?\")"), QTextCharFormat() });

	// Rule for JSON numbers
	highlightingRules.append({ QRegularExpression(R"(:\s*\d+(\.\d+)?)"), QTextCharFormat() });

	// Rule for JSON boolean values and null (no used in repo json yet)
	highlightingRules.append({ QRegularExpression(R"(\b(true|false|null)\b)"), QTextCharFormat() });
}

void JsonHighlighter::updateColors(Qt::ColorScheme currentTheme)
{
	QVector < QColor > themeColors;
	if (currentTheme == Qt::ColorScheme::Light)
	{
		themeColors = lightThemeColors;
	}
	else if (currentTheme == Qt::ColorScheme::Dark)
	{
		themeColors = darkThemeColors;
	}
	else
	{
		qDebug() << "Unknown theme";
		return;
	}

	for (int i = 0; i < highlightingRules.size(); i++)
	{
		QTextCharFormat format;
		format.setForeground(themeColors.at(i));
		highlightingRules[i].Format = format;
	}
}

void JsonHighlighter::highlightBlock( const QString& text )
{
	for (const HighlightingRule& rule : std::as_const(highlightingRules))
	{
		QRegularExpressionMatchIterator matchIterator = rule.Pattern.globalMatch(text);
		while (matchIterator.hasNext())
		{
			QRegularExpressionMatch match = matchIterator.next();
			setFormat(match.capturedStart(), match.capturedLength(), rule.Format);
		}
	}
}
