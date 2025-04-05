#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_reposaveeditor.h"
#include "utility.h"

#include <cryptopp/sha.h>

QT_BEGIN_NAMESPACE namespace Ui
{
	class RepoSaveEditorClass;
};

QT_END_NAMESPACE class RepoSaveEditor final : public QMainWindow
{
	Q_OBJECT public:
	explicit RepoSaveEditor( QWidget* parent = nullptr );
	~RepoSaveEditor() override;

	static void SetWidgetsVisible( const QLayout* layout, bool enabled );

private slots:
	void OpenFile();
	void SaveFile(QString filePath);
	void SaveOpenedFile();
	void SaveFileAs();

	void UpdateWidgets( const QString& json );
	void UpdateJsonText();

	void CheckJson();

private:
	Ui::RepoSaveEditorClass* ui;

	JsonHighlighter* jsonHighlighter = nullptr;

	QJsonDocument json_;

	void HideUi() const;

	static QString DecryptFile( const QString& filePath );
	static QByteArray EncryptData( const QString& data, bool useGzip = false );

	static void SaveData( const QJsonObject& jsonData, QWidget* parent );

	static void DeriveKey( const std::string& password, const CryptoPP::byte* iv, CryptoPP::byte* key );

	inline static const QString Password = "Why would you want to cheat?... :o It's no fun. :') :'D";
	inline static const QString Version = "1.0.1";
};
