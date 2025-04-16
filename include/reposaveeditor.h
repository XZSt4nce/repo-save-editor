#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_reposaveeditor.h"
#include "utility.h"
#include "jsonwrapper.h"

#include <cryptopp/sha.h>

QT_BEGIN_NAMESPACE namespace Ui
{
	class RepoSaveEditorClass;
};

QT_END_NAMESPACE const inline QString DEFAULT_SAVES_LOCATION = QString( R"(%1\AppData\LocalLow\semiwork\Repo\saves)" ).arg( QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) );

class RepoSaveEditor final : public QMainWindow
{
	Q_OBJECT public:
	explicit RepoSaveEditor( QWidget* parent = nullptr );
	~RepoSaveEditor() override;

	static void SetWidgetsVisible( const QLayout* layout, bool enabled );

signals:
	void WindowMovedOrResized();

private slots:
	void NewFile();
	void OpenFile();
	void SaveFile( const QString& filePath);
	void SaveOpenedFile();
	void SaveFileAs();

	void SelectLanguage(const QString& suffix);
	void SelectEnglishLanguage();
	void SelectRussianLanguage();

	void UpdateWindow() const;
	void UpdateHighlighter(const Qt::ColorScheme) const;
	void SelectSystemTheme() const;
	void SelectLightTheme() const;
	void SelectDarkTheme() const;

	void GenerateJson();
	void LoadJson( const QString& filePath );
	void UpdateWidgets() const;
	void UpdateJsonText();

	void CheckJson();

protected:
	void changeEvent(QEvent* e);

private:
	Ui::RepoSaveEditorClass* ui;

	JsonHighlighter* jsonHighlighter = nullptr;

	JsonWrapper json;

	QString openedFile;

	QTranslator qtTranslator;
	QTranslator appTranslator;

	QPalette lightPalette;
	QPalette darkPalette;

	void HideUi() const;
	void SetupShortcuts() const;

	static QString DecryptFile( const QString& filePath );
	static QByteArray EncryptData( const QString& data, bool useGzip = false );

	static void DeriveKey( const std::string& password, const CryptoPP::byte* iv, CryptoPP::byte* key );

	void moveEvent( QMoveEvent* event ) override;
	void resizeEvent( QResizeEvent* event ) override;

	inline static const QString Password = "Why would you want to cheat?... :o It's no fun. :') :'D";
	inline static const QString Version = "1.1.3";
};
