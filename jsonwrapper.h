#pragma once

class JsonWrapper
{
public:
	JsonWrapper() = default;
	explicit JsonWrapper( QJsonDocument& doc ) : document_( std::move( doc ) ) {}

	[[nodiscard]] QJsonDocument& Document();
	[[nodiscard]] const QJsonDocument& Document() const;

	[[nodiscard]] QJsonValue Get( const QString& path ) const;
	[[nodiscard]] bool Set( const QString& path, const QJsonValue& value );
	[[nodiscard]] bool Remove( const QString& path );

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

	[[nodiscard]] static JsonPath PlayerPath( const QString& steamId );

	[[nodiscard]] static QStringList ToList( const QString& path );
};
