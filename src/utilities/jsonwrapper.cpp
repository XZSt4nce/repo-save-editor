#include "stdafx.h"
#include "jsonwrapper.h"

QJsonDocument& JsonWrapper::Document()
{
	return document_;
}

const QJsonDocument& JsonWrapper::Document() const
{
	return document_;
}

QJsonValue JsonWrapper::Get( const QString& path ) const
{
	QStringList keys = PropertyPath::ToList( path );
	const QJsonObject obj = document_.object();
	QJsonValue val = obj;

	for ( const QString& key : keys )
	{
		if ( !val.isObject() )
			return {}; // Invalid path
		val = val.toObject().value( key );
	}
	return val;
}

bool JsonWrapper::Set( const QString& path, const QJsonValue& value )
{
	QStringList keys = PropertyPath::ToList( path );
	if ( keys.isEmpty() )
		return false;

	QJsonObject root = document_.object();
	QJsonObject* current = &root;

	QList < QJsonObject* > hierarchy; // for reconstitution

	// Descending the hierarchy
	for ( int i = 0; i < keys.size() - 1; ++i )
	{
		const QString& key = keys[ i ];
		const QJsonObject next = current->value( key ).toObject();
		hierarchy.append( current );
		current = new QJsonObject( next ); // new object to avoid modifying the original
	}

	// Insert value at the last level
	current->insert( keys.last(), value );

	// Rebuild the hierarchy
	for ( int i = static_cast < int >( keys.size() ) - 2; i >= 0; --i )
	{
		QJsonObject* parent = hierarchy[ i ];
		parent->insert( keys[ i ], *current );
		current = parent;
	}

	document_.setObject( root );
	return true;
}

bool JsonWrapper::Remove( const QString& path )
{
	QStringList keys = PropertyPath::ToList( path );
	if ( keys.isEmpty() )
		return false;

	QJsonObject root = document_.object();
	QJsonObject* current = &root;

	QList < QJsonObject* > hierarchy;

	// Descending the hierarchy
	for ( int i = 0; i < keys.size() - 1; ++i )
	{
		const QString& key = keys[ i ];
		if ( !current->contains( key ) || !( *current )[ key ].isObject() )
			return false;

		const QJsonObject next = ( *current )[ key ].toObject();
		hierarchy.append( current );
		current = new QJsonObject( next );
	}

	// Remove the last key
	current->remove( keys.last() );

	// Rebuild the hierarchy
	for ( int i = static_cast < int >( keys.size() ) - 2; i >= 0; --i )
	{
		QJsonObject* parent = hierarchy[ i ];
		parent->insert( keys[ i ], *current );
		current = parent;
	}

	document_.setObject( root );
	return true;
}

void JsonWrapper::Generate()
{
	document_ = QJsonDocument();

	Set("dateAndTime.__type", "string");
	Set(PropertyPath::DateAndTimePath(), QDateTime::currentDateTime().toString("yyyy-MM-dd"));

	Set("dictionaryOfDictionaries.__type", "System.Collections.Generic.Dictionary`2[[System.String, mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089],[System.Collections.Generic.Dictionary`2[[System.String, mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089],[System.Int32, mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089]], mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089]],mscorlib");

	for (const QString key : Items.keys())
	{
		Set(PropertyPath::SubItemPath("item", key), Items[key]);
		Set(PropertyPath::SubItemPath("itemBatteryUpgrades", key), Items[key]);
		Set(PropertyPath::SubItemPath("itemsPurchased", key), Items[key]);
		Set(PropertyPath::SubItemPath("itemsPurchasedTotal", key), Items[key]);
		Set(PropertyPath::SubItemPath("itemsUpgradesPurchased", key), Items[key]);
		Set(PropertyPath::ItemsPurchasedCountPath(key), Items[key]);
	}

	Set(PropertyPath::SubItemPath("itemsPurchased", "Item Cart Medium"), 1);
	Set(PropertyPath::SubItemPath("itemsPurchasedTotal", "Item Cart Medium"), 1);

	Set(PropertyPath::ItemPath("playerHasCrown"), QJsonObject());
	Set(PropertyPath::ItemPath("playerHealth"), QJsonObject());
	Set(PropertyPath::ItemPath("playerUpgradeExtraJump"), QJsonObject());
	Set(PropertyPath::ItemPath("playerUpgradeHealth"), QJsonObject());
	Set(PropertyPath::ItemPath("playerUpgradeLaunch"), QJsonObject());
	Set(PropertyPath::ItemPath("playerUpgradeMapPlayerCount"), QJsonObject());
	Set(PropertyPath::ItemPath("playerUpgradeRange"), QJsonObject());
	Set(PropertyPath::ItemPath("playerUpgradeSpeed"), QJsonObject());
	Set(PropertyPath::ItemPath("playerUpgradeStamina"), QJsonObject());
	Set(PropertyPath::ItemPath("playerUpgradeStrength"), QJsonObject());
	Set(PropertyPath::ItemPath("playerUpgradeThrow"), QJsonObject());

	Set(PropertyPath::RunStatsPath("chargingStationCharge"), 1);
	Set(PropertyPath::RunStatsPath("currency"), 0);
	Set(PropertyPath::RunStatsPath("level"), 0);
	Set(PropertyPath::RunStatsPath("lives"), 3);
	Set(PropertyPath::RunStatsPath("save level"), 0);
	Set(PropertyPath::RunStatsPath("totalHaul"), 0);

	Set("playerNames.__type", "System.Collections.Generic.Dictionary`2[[System.String, mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089],[System.String, mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089]],mscorlib");
	Set(PropertyPath::PlayerPath(), QJsonObject());

	Set("teamName.__type", "string");
	Set(PropertyPath::TeamNamePath(), "R.E.P.O");

	Set("timePlayed.__type", "float");
	Set(PropertyPath::TimePlayedPath(), 0);
}

QStringList JsonWrapper::GetPlayerNames() const
{
	QStringList playerNames;
	for ( const QString& playerId : GetPlayerIds() )
	{
		if ( const QJsonValue playerName = Get( PropertyPath::PlayerNamePath( playerId ) ); playerName.isString() )
			playerNames.append( playerName.toString() );
	}
	return playerNames;
}

QStringList JsonWrapper::GetPlayerIds() const
{
	if ( const QJsonValue playerNames = Get( PropertyPath::PlayerPath() ); playerNames.isObject() )
	{
		return playerNames.toObject().keys();
	}
	return {};
}

QString JsonWrapper::GetPlayerName( const QString& steamId ) const
{
	if ( const QJsonValue playerName = Get( PropertyPath::PlayerNamePath( steamId ) ); playerName.isString() )
	{
		return playerName.toString();
	}
	return {};
}

QString JsonWrapper::GetPlayerId( const QString& playerName ) const
{
	if ( const QJsonValue playerId = Get( PropertyPath::PlayerNamePath( playerName ) ); playerId.isString() )
	{
		return playerId.toString();
	}
	return {};
}

bool JsonWrapper::IsNull() const
{
	return document_.isNull();
}

bool JsonWrapper::IsEmpty() const
{
	return document_.isEmpty();
}

QByteArray JsonWrapper::ToJson( const QJsonDocument::JsonFormat format ) const
{
	return document_.toJson( format );
}

QStringList PropertyPath::ToList() const
{
	return split( '.' );
}

PropertyPath PropertyPath::PlayerPath()
{
	return PropertyPath( "playerNames.value" );
}

PropertyPath PropertyPath::PlayerNamePath( const QString& steamId )
{
	return PropertyPath( QString( "playerNames.value.%1" ).arg( steamId ) );
}

PropertyPath PropertyPath::ItemPath(const QString& itemName)
{
	return PropertyPath(QString("dictionaryOfDictionaries.value.%1").arg(itemName));
}

PropertyPath PropertyPath::SubItemPath(const QString& itemName, const QString& subItemName)
{
	return PropertyPath(QString("dictionaryOfDictionaries.value.%1.%2").arg(itemName).arg(subItemName));
}

PropertyPath PropertyPath::PlayerUpgrade(const QString& upgradeName)
{
	return ItemPath(upgradeName);
}

PropertyPath PropertyPath::PlayerIdUpgrade(const QString& upgradeName, const QString& steamId )
{
	return SubItemPath(upgradeName, steamId);
}

PropertyPath PropertyPath::RunStatsPath( const QString& key )
{
	return PropertyPath( QString( "dictionaryOfDictionaries.value.runStats.%1" ).arg( key ) );
}

PropertyPath PropertyPath::TeamNamePath()
{
	return PropertyPath( "teamName.value" );
}

PropertyPath PropertyPath::DateAndTimePath()
{
	return PropertyPath( "dateAndTime.value" );
}

PropertyPath PropertyPath::TimePlayedPath()
{
	return PropertyPath( "timePlayed.value" );
}

PropertyPath PropertyPath::ItemsPurchasedPath()
{
	return PropertyPath( "dictionaryOfDictionaries.value.itemsPurchased" );
}

PropertyPath PropertyPath::ItemsPurchasedCountPath( const QString& itemName )
{
	return PropertyPath( QString( "dictionaryOfDictionaries.value.itemsPurchased.%1" ).arg( itemName ) );
}

QStringList PropertyPath::ToList( const QString& path )
{
	return path.split( '.' );
}
