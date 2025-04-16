#pragma once

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>

class RepoSaveEditor;

class Global
{
public:
	static RepoSaveEditor* Application()
	{
		return application;
	}

	static void SetApplication( RepoSaveEditor* app )
	{
		application = app;
	}

private:
	inline static RepoSaveEditor* application = nullptr;
};

class JsonHighlighter final : public QSyntaxHighlighter
{
public:
	explicit JsonHighlighter( QTextDocument* parent = nullptr );

public slots:
	void updateColors( Qt::ColorScheme currentTheme );

protected:
	void highlightBlock( const QString& text ) override;

private slots:
	void initializeRules();

private:
	struct HighlightingRule
	{
		QRegularExpression Pattern;
		QTextCharFormat Format;
	};

	QVector < HighlightingRule > highlightingRules;

	inline static const QVector < QColor > lightThemeColors = {
		Qt::black,
		QColor(206, 139, 85),
		QColor(86, 126, 162),
		Qt::darkRed
	};

	inline static const QVector < QColor > darkThemeColors = {
		QColor(140, 220, 254),
		QColor(206, 139, 85),
		QColor(181, 192, 120),
		Qt::darkRed
	};
};
