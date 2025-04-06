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
	QStringList keys = JsonPath::ToList( path );
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
	QStringList keys = JsonPath::ToList( path );
	QJsonObject obj = document_.object();
	QJsonValue val = obj;
	for ( int i = 0; i < keys.size() - 1; ++i )
	{
		const QString& key = keys[ i ];
		if ( !val.isObject() )
			return false; // Invalid path
		val = val.toObject().value( key );
	}
	if ( !val.isObject() )
		return false; // Invalid path
	QJsonObject newObj = val.toObject();
	newObj.insert( keys.last(), value );
	obj.insert( keys[ 0 ], newObj );
	document_.setObject( obj );
	return true;
}

bool JsonWrapper::Remove( const QString& path )
{
	QStringList keys = JsonPath::ToList( path );
	QJsonObject obj = document_.object();
	QJsonValue val = obj;
	for ( int i = 0; i < keys.size() - 1; ++i )
	{
		const QString& key = keys[ i ];
		if ( !val.isObject() )
			return false; // Invalid path
		val = val.toObject().value( key );
	}
	if ( !val.isObject() )
		return false; // Invalid path
	QJsonObject newObj = val.toObject();
	newObj.remove( keys.last() );
	obj.insert( keys[ 0 ], newObj );
	document_.setObject( obj );
	return true;
}

bool JsonWrapper::IsNull() const
{
	return document_.isNull();
}

QByteArray JsonWrapper::ToJson( const QJsonDocument::JsonFormat format ) const
{
	return document_.toJson( format );
}

QStringList JsonPath::ToList() const
{
	return split( '.' );
}

JsonPath JsonPath::PlayerPath( const QString& steamId )
{
	return JsonPath( JsonPath( "playerNames.value.%1" ).arg( steamId ) );
}

QStringList JsonPath::ToList( const QString& path )
{
	return path.split( '.' );
}
