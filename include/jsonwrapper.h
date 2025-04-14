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
	void Generate();


	[[nodiscard]] QStringList GetPlayerNames() const;
	[[nodiscard]] QStringList GetPlayerIds() const;
	[[nodiscard]] QString GetPlayerName( const QString& steamId ) const;
	[[nodiscard]] QString GetPlayerId( const QString& playerName ) const;

	[[nodiscard]] bool IsNull() const;
	[[nodiscard]] bool IsEmpty() const;

	// Export JSON
	[[nodiscard]] QByteArray ToJson( QJsonDocument::JsonFormat format = QJsonDocument::Indented ) const;

private:
	QJsonDocument document_;

	inline static const QMap < QString, int > Items = {
		{ "Item Cart Medium", 0 },
		{ "Item Cart Small", 0 },
		{ "Item Drone Battery", 0 },
		{ "Item Drone Feather", 0 },
		{ "Item Drone Indestructible", 0 },
		{ "Item Drone Zero Gravity", 0 },
		{ "Item Drone Torque", 0 },
		{ "Item Extraction Tracker", 0 },
		{ "Item Grenade Duct Taped", 0 },
		{ "Item Grenade Explosive", 0 },
		{ "Item Grenade Human", 0 },
		{ "Item Grenade Shockwave", 0 },
		{ "Item Grenade Stun", 0 },
		{ "Item Gun Handgun", 0 },
		{ "Item Gun Shotgun", 0 },
		{ "Item Gun Tranq", 0 },
		{ "Item Health Pack Large", 0 },
		{ "Item Health Pack Medium", 0 },
		{ "Item Health Pack Small", 0 },
		{ "Item Melee Baseball Bat", 0 },
		{ "Item Melee Frying Pan", 0 },
		{ "Item Melee Inflatable Hammer", 0 },
		{ "Item Melee Sledge Hammer", 0 },
		{ "Item Melee Sword", 0 },
		{ "Item Mine Explosive", 0 },
		{ "Item Mine Shockwave", 0 },
		{ "Item Mine Stun", 0 },
		{ "Item Orb Zero Gravity", 0 },
		{ "Item Power Crystal", 0 },
		{ "Item Rubber Duck", 0 },
		{ "Item Upgrade Map Player Count", 0 },
		{ "Item Upgrade PlayerEnergy", 0 },
		{ "Item Upgrade PlayerExtra Jump", 0 },
		{ "Item Upgrade PlayerGrab Range", 0 },
		{ "Item Upgrade PlayerGrab Strength", 0 },
		{ "Item Upgrade PlayerHealth", 0 },
		{ "Item Upgrade PlayerSprint Speed", 0 },
		{ "Item Upgrade PlayerTumble Launch", 0 },
		{ "Item Valuable Tracker", 0 }
	};
};

class PropertyPath : public QString
{
public:
	explicit PropertyPath( const QString& path ) : QString( path ) {}

	[[nodiscard]] QStringList ToList() const;

	[[nodiscard]] static PropertyPath PlayerPath();
	[[nodiscard]] static PropertyPath PlayerNamePath( const QString& steamId );
	[[nodiscard]] static PropertyPath ItemPath( const QString& itemName );
	[[nodiscard]] static PropertyPath SubItemPath( const QString& itemName, const QString& subItemName );
	[[nodiscard]] static PropertyPath PlayerUpgrade( const QString& upgradeName );
	[[nodiscard]] static PropertyPath PlayerIdUpgrade(const QString& upgradeName, const QString& steamId );
	[[nodiscard]] static PropertyPath RunStatsPath( const QString& key );
	[[nodiscard]] static PropertyPath TeamNamePath();
	[[nodiscard]] static PropertyPath DateAndTimePath();
	[[nodiscard]] static PropertyPath TimePlayedPath();
	[[nodiscard]] static PropertyPath ItemsPurchasedPath();
	[[nodiscard]] static PropertyPath ItemsPurchasedCountPath( const QString& itemName );

	[[nodiscard]] static QStringList ToList( const QString& path );
};
