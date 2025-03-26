#pragma once

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>

class JsonHighlighter final : public QSyntaxHighlighter
{
public:
	explicit JsonHighlighter( QTextDocument* parent = nullptr ) : QSyntaxHighlighter( parent )
	{
		QTextCharFormat keyFormat;
		keyFormat.setForeground( QColor( 140, 220, 254 ) );
		highlightingRules.append( { QRegularExpression( R"(\".*?\"(?=\s*:))" ), keyFormat } );

		QTextCharFormat stringFormat;
		stringFormat.setForeground( QColor( 206, 139, 85 ) );
		highlightingRules.append( { QRegularExpression( R"(:\s*\".*?\")" ), stringFormat } );

		QTextCharFormat numberFormat;
		numberFormat.setForeground( QColor( 181, 192, 120 ) );
		highlightingRules.append( { QRegularExpression( R"(:\s*\d+(\.\d+)?)" ), numberFormat } );

		QTextCharFormat boolFormat; // no used in repo json
		boolFormat.setForeground( Qt::darkRed );
		highlightingRules.append( { QRegularExpression( R"(\b(true|false|null)\b)" ), boolFormat } );
	}

protected:
	void highlightBlock( const QString& text ) override
	{
		for ( const HighlightingRule& rule : std::as_const( highlightingRules ) )
		{
			QRegularExpressionMatchIterator matchIterator = rule.Pattern.globalMatch( text );
			while ( matchIterator.hasNext() )
			{
				QRegularExpressionMatch match = matchIterator.next();
				setFormat( match.capturedStart(), match.capturedLength(), rule.Format );
			}
		}
	}

private:
	struct HighlightingRule
	{
		QRegularExpression Pattern;
		QTextCharFormat Format;
	};

	QVector < HighlightingRule > highlightingRules;
};
