#pragma once

class JsonWrapper
{
public:
	JsonWrapper() = default;
	explicit JsonWrapper( const QJsonDocument& doc ) : document_( doc ) {}

	[[nodiscard]] QJsonDocument& Document();
	[[nodiscard]] const QJsonDocument& Document() const;

	[[nodiscard]] QJsonValue Get( const QString& path ) const;
	bool Set( const QString& path, const QJsonValue& value );
	bool Remove( const QString& path );

	[[nodiscard]] QStringList GetPlayerNames() const;
	[[nodiscard]] QStringList GetPlayerIds() const;
	[[nodiscard]] QString GetPlayerName( const QString& steamId ) const;
	[[nodiscard]] QString GetPlayerId( const QString& playerName ) const;

	[[nodiscard]] bool IsNull() const;

	// Export JSON
	[[nodiscard]] QByteArray ToJson( QJsonDocument::JsonFormat format = QJsonDocument::Indented ) const;

private:
	QJsonDocument document_;
};

class JsonPath : public QString
{
public:
	explicit JsonPath( const QString& path ) : QString( path ) {}

	[[nodiscard]] QStringList ToList() const;

	[[nodiscard]] static JsonPath PlayerPath();
	[[nodiscard]] static JsonPath PlayerNamePath( const QString& steamId );
	[[nodiscard]] static JsonPath PlayerUpgrade( const QString& steamId, const QString& upgradeName );
	[[nodiscard]] static JsonPath RunStatsPath( const QString& key );
	[[nodiscard]] static JsonPath TeamNamePath();
	[[nodiscard]] static JsonPath DateAndTimePath();
	[[nodiscard]] static JsonPath TimePlayedPath();
	[[nodiscard]] static JsonPath ItemsPurchasedPath();
	[[nodiscard]] static JsonPath ItemsPurchasedCountPath( const QString& itemName );

	[[nodiscard]] static QStringList ToList( const QString& path );
};
