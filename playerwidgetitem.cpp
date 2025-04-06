#include "stdafx.h"
#include "playerwidgetitem.h"

PlayerWidgetItem::PlayerWidgetItem( QWidget* parent ) : QWidget( parent ), ui( new Ui::PlayerWidgetItemClass() )
{
	ui->setupUi( this );

	connect( ui->playerHealthSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &PlayerWidgetItem::ValueChanged );
	connect( ui->healthSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &PlayerWidgetItem::ValueChanged );
	connect( ui->staminaSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &PlayerWidgetItem::ValueChanged );
	connect( ui->extraJumpSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &PlayerWidgetItem::ValueChanged );
	connect( ui->launchSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &PlayerWidgetItem::ValueChanged );
	connect( ui->mapPlayerCountSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &PlayerWidgetItem::ValueChanged );
	connect( ui->speedSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &PlayerWidgetItem::ValueChanged );
	connect( ui->strengthSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &PlayerWidgetItem::ValueChanged );
	connect( ui->rangeSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &PlayerWidgetItem::ValueChanged );
	connect( ui->throwSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &PlayerWidgetItem::ValueChanged );
	connect( ui->hasCrownCheckBox, &QCheckBox::checkStateChanged, this, &PlayerWidgetItem::ValueChanged );
}

PlayerWidgetItem::~PlayerWidgetItem()
{
	delete ui;
}

void PlayerWidgetItem::UpdateWidgets( const QJsonDocument& json, const QString& steamId )
{
	steamId_ = steamId;

	// Block signals to prevent signals from being emitted when setting values
	QSignalBlocker blockers[ ] = {
		QSignalBlocker( ui->playerGroupBox ),
		QSignalBlocker( ui->playerHealthSpinBox ),
		QSignalBlocker( ui->healthSpinBox ),
		QSignalBlocker( ui->staminaSpinBox ),
		QSignalBlocker( ui->extraJumpSpinBox ),
		QSignalBlocker( ui->launchSpinBox ),
		QSignalBlocker( ui->mapPlayerCountSpinBox ),
		QSignalBlocker( ui->speedSpinBox ),
		QSignalBlocker( ui->strengthSpinBox ),
		QSignalBlocker( ui->rangeSpinBox ),
		QSignalBlocker( ui->throwSpinBox ),
		QSignalBlocker( ui->hasCrownCheckBox )
	};

	const QJsonObject& globalObj = json[ "dictionaryOfDictionaries" ].toObject().value( "value" ).toObject();

	ui->playerHealthSpinBox->setValue( globalObj.value( "playerHealth" ).toObject().value( steamId ).toInt() );
	ui->healthSpinBox->setValue( globalObj.value( "playerUpgradeHealth" ).toObject().value( steamId ).toInt() );
	ui->staminaSpinBox->setValue( globalObj.value( "playerUpgradeStamina" ).toObject().value( steamId ).toInt() );
	ui->extraJumpSpinBox->setValue( globalObj.value( "playerUpgradeExtraJump" ).toObject().value( steamId ).toInt() );
	ui->launchSpinBox->setValue( globalObj.value( "playerUpgradeLaunch" ).toObject().value( steamId ).toInt() );
	ui->mapPlayerCountSpinBox->setValue( globalObj.value( "playerUpgradeMapPlayerCount" ).toObject().value( steamId ).toInt() );
	ui->speedSpinBox->setValue( globalObj.value( "playerUpgradeSpeed" ).toObject().value( steamId ).toInt() );
	ui->strengthSpinBox->setValue( globalObj.value( "playerUpgradeStrength" ).toObject().value( steamId ).toInt() );
	ui->rangeSpinBox->setValue( globalObj.value( "playerUpgradeRange" ).toObject().value( steamId ).toInt() );
	ui->throwSpinBox->setValue( globalObj.value( "playerUpgradeThrow" ).toObject().value( steamId ).toInt() );
	ui->hasCrownCheckBox->setCheckState( globalObj.value( "playerHasCrown" ).toObject().value( steamId ).toBool() == true ? Qt::Checked : Qt::Unchecked );
}

void PlayerWidgetItem::SetJsonValues( QJsonDocument& json ) const
{
	// Copy QJsonDocument to QJsonObject
	QJsonObject root = json.object();

	// Modify "playerNames"
	QJsonObject playerNamesObj = json[ "playerNames" ].toObject();
	QJsonObject playerNamesValue = playerNamesObj[ "value" ].toObject();
	playerNamesValue.insert( steamId_, ui->playerGroupBox->title().split( ' ' ).first() );
	playerNamesObj.insert( "value", playerNamesValue );
	root.insert( "playerNames", playerNamesObj );

	// Modify dictionaryOfDictionaries
	QJsonObject dictOfDicts = json[ "dictionaryOfDictionaries" ].toObject();
	QJsonObject dictValue = dictOfDicts[ "value" ].toObject();

	auto updatePlayerStat = [&] ( const QString& key, int value )
	{
		QJsonObject statObj = dictValue[ key ].toObject();
		statObj.insert( steamId_, value );
		dictValue.insert( key, statObj );
	};

	// Insert new values
	updatePlayerStat( "playerHealth", ui->playerHealthSpinBox->value() );
	updatePlayerStat( "playerUpgradeHealth", ui->healthSpinBox->value() );
	updatePlayerStat( "playerUpgradeStamina", ui->staminaSpinBox->value() );
	updatePlayerStat( "playerUpgradeExtraJump", ui->extraJumpSpinBox->value() );
	updatePlayerStat( "playerUpgradeLaunch", ui->launchSpinBox->value() );
	updatePlayerStat( "playerUpgradeMapPlayerCount", ui->mapPlayerCountSpinBox->value() );
	updatePlayerStat( "playerUpgradeSpeed", ui->speedSpinBox->value() );
	updatePlayerStat( "playerUpgradeStrength", ui->strengthSpinBox->value() );
	updatePlayerStat( "playerUpgradeRange", ui->rangeSpinBox->value() );
	updatePlayerStat( "playerUpgradeThrow", ui->throwSpinBox->value() );

	// playerHasCrown (bool)
	QJsonObject playerHasCrownObj = dictValue[ "playerHasCrown" ].toObject();
	playerHasCrownObj.insert( steamId_, ui->hasCrownCheckBox->isChecked() ? 1 : 0 );
	dictValue.insert( "playerHasCrown", playerHasCrownObj );

	// Reassign modified root object to QJsonDocument
	dictOfDicts.insert( "value", dictValue );
	root.insert( "dictionaryOfDictionaries", dictOfDicts );

	// Reassign modified root object to QJsonDocument
	json.setObject( root );
}

void PlayerWidgetItem::ValueChanged()
{
	emit Edited();
}
