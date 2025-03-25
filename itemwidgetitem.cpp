#include "stdafx.h"
#include "itemwidgetitem.h"

ItemWidgetItem::ItemWidgetItem( QWidget* parent ) : QWidget( parent ), ui( new Ui::ItemWidgetItemClass() )
{
	ui->setupUi( this );

	connect( ui->valueSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &ItemWidgetItem::ValueChanged );
}

ItemWidgetItem::~ItemWidgetItem()
{
	delete ui;
}

void ItemWidgetItem::UpdateWidget( const QJsonDocument& json, const QString& itemName )
{
	itemName_ = itemName;

	const int itemCount = json[ "dictionaryOfDictionaries" ].toObject().value( "value" ).toObject().value( "itemsPurchased" ).toObject().value( itemName ).toInt();

	// Block signals to prevent signals from being emitted when setting values
	QSignalBlocker blockers[ ] = { QSignalBlocker( ui->valueSpinBox ) };

	ui->itemNameLabel->setText( QString( itemName ).replace( "Item ", "" ) );
	ui->valueSpinBox->setValue( itemCount );
}

void ItemWidgetItem::SetJsonValues( QJsonDocument& json ) const
{
	QJsonObject root = json.object();
	QJsonObject itemsPurchased = root[ "dictionaryOfDictionaries" ].toObject().value( "value" ).toObject().value( "itemsPurchased" ).toObject();
	itemsPurchased.insert( itemName_, ui->valueSpinBox->value() );
	QJsonObject dictOfDicts = root[ "dictionaryOfDictionaries" ].toObject();
	QJsonObject dictOfDictsValue = dictOfDicts[ "value" ].toObject();
	dictOfDictsValue.insert( "itemsPurchased", itemsPurchased );
	dictOfDicts.insert( "value", dictOfDictsValue );
	root.insert( "dictionaryOfDictionaries", dictOfDicts );
	json.setObject( root );
}

void ItemWidgetItem::ValueChanged()
{
	emit Edited();
}
