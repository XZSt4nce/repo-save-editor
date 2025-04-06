#include "stdafx.h"
#include "playerwidget.h"


PlayerWidget::PlayerWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::PlayerWidgetClass() )
{
	ui->setupUi( this );

	connect( ui->playerComboBox, &QComboBox::currentIndexChanged, this, &PlayerWidget::UpdatePlayerInfo );

	connect( ui->resetButton, &QPushButton::clicked, this, &PlayerWidget::UpdatePlayerInfo );
	connect( ui->saveButton, &QPushButton::clicked, this, &PlayerWidget::SavePlayerInfo );
}

PlayerWidget::~PlayerWidget()
{
	delete ui;
}

void PlayerWidget::UpdateWidgets( const QJsonDocument& json )
{
	json_ = json;
	ui->playerComboBox->clear();

	for ( const QString& steamId : json[ "playerNames" ].toObject().value( "value" ).toObject().keys() )
	{
		const QString playerName = json[ "playerNames" ].toObject().value( "value" ).toObject().value( steamId ).toString();
		ui->playerComboBox->addItem( playerName, steamId );
	}

	if ( ui->playerComboBox->count() > 0 )
	{
		ui->playerComboBox->setCurrentIndex( 0 );
		UpdatePlayerInfo();
	}

	SetVisible( true );
}

void PlayerWidget::UpdatePlayerInfo() const
{
	const QString steamId = ui->playerComboBox->currentData().toString();
	const QJsonObject& globalObj = json_[ "dictionaryOfDictionaries" ].toObject().value( "value" ).toObject();

	ui->steamIdLabel->setText( QString( "Steam ID: %1" ).arg( steamId ) );

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
	ui->hasCrownCheckBox->setCheckState( globalObj.value( "playerHasCrown" ).toObject().value( steamId ).toInt() == 1 ? Qt::Checked : Qt::Unchecked );
}

void PlayerWidget::SetJsonValue( QJsonDocument& json ) const
{
	// Copy QJsonDocument to QJsonObject
	QJsonObject root = json.object();

	QStringList steamIds = json_[ "playerNames" ].toObject().value( "value" ).toObject().keys();

	// Modify dictionaryOfDictionaries
	QJsonObject dictOfDicts = json_[ "dictionaryOfDictionaries" ].toObject();
	QJsonObject dictValue = dictOfDicts[ "value" ].toObject();

	for ( const QString& steamId : steamIds )
	{
		// Modify "playerNames"
		QJsonObject playerNamesObj = json_[ "playerNames" ].toObject();
		QJsonObject playerNamesValue = playerNamesObj[ "value" ].toObject();
		playerNamesValue.insert( steamId, playerNamesObj.value( "value" ).toObject().value( steamId ).toString() );
		playerNamesObj.insert( "value", playerNamesValue );
		root.insert( "playerNames", playerNamesObj );

		const QJsonObject& globalObj = json_[ "dictionaryOfDictionaries" ].toObject().value( "value" ).toObject();

		auto updatePlayerStat = [&] ( const QString& key, const int value )
		{
			QJsonObject statObj = dictValue[ key ].toObject();
			statObj.insert( steamId, value );
			dictValue.insert( key, statObj );
		};

		// Insert new values
		updatePlayerStat( "playerHealth", globalObj.value( "playerHealth" ).toObject().value( steamId ).toInt() );
		updatePlayerStat( "playerUpgradeHealth", globalObj.value( "playerUpgradeHealth" ).toObject().value( steamId ).toInt() );
		updatePlayerStat( "playerUpgradeStamina", globalObj.value( "playerUpgradeStamina" ).toObject().value( steamId ).toInt() );
		updatePlayerStat( "playerUpgradeExtraJump", globalObj.value( "playerUpgradeExtraJump" ).toObject().value( steamId ).toInt() );
		updatePlayerStat( "playerUpgradeLaunch", globalObj.value( "playerUpgradeLaunch" ).toObject().value( steamId ).toInt() );
		updatePlayerStat( "playerUpgradeMapPlayerCount", globalObj.value( "playerUpgradeMapPlayerCount" ).toObject().value( steamId ).toInt() );
		updatePlayerStat( "playerUpgradeSpeed", globalObj.value( "playerUpgradeSpeed" ).toObject().value( steamId ).toInt() );
		updatePlayerStat( "playerUpgradeStrength", globalObj.value( "playerUpgradeStrength" ).toObject().value( steamId ).toInt() );
		updatePlayerStat( "playerUpgradeRange", globalObj.value( "playerUpgradeRange" ).toObject().value( steamId ).toInt() );
		updatePlayerStat( "playerUpgradeThrow", globalObj.value( "playerUpgradeThrow" ).toObject().value( steamId ).toInt() );
		updatePlayerStat( "playerHasCrown", globalObj.value( "playerHasCrown" ).toObject().value( steamId ).toInt() );

		// Reassign modified root object to QJsonDocument
		dictOfDicts.insert( "value", dictValue );
		root.insert( "dictionaryOfDictionaries", dictOfDicts );
	}

	// Reassign modified root object to QJsonDocument
	json.setObject( root );
}

void PlayerWidget::SetVisible( const bool visible ) const
{
	ui->stackedWidget->setCurrentIndex( visible ? 1 : 0 );
}

void PlayerWidget::ValueChanged()
{
	emit Edited();
}

void PlayerWidget::SavePlayerInfo()
{
	const QString steamId = ui->playerComboBox->currentData().toString();

	// Copy QJsonDocument to QJsonObject
	QJsonObject root = json_.object();

	// Modify "playerNames"
	QJsonObject playerNamesObj = json_[ "playerNames" ].toObject();
	QJsonObject playerNamesValue = playerNamesObj[ "value" ].toObject();
	playerNamesValue.insert( steamId, ui->playerComboBox->currentText() );
	playerNamesObj.insert( "value", playerNamesValue );
	root.insert( "playerNames", playerNamesObj );

	// Modify dictionaryOfDictionaries
	QJsonObject dictOfDicts = json_[ "dictionaryOfDictionaries" ].toObject();
	QJsonObject dictValue = dictOfDicts[ "value" ].toObject();

	auto updatePlayerStat = [&] ( const QString& key, const int value )
	{
		QJsonObject statObj = dictValue[ key ].toObject();
		statObj.insert( steamId, value );
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
	updatePlayerStat( "playerHasCrown", ui->hasCrownCheckBox->isChecked() ? 1 : 0 );

	// Reassign modified root object to QJsonDocument
	dictOfDicts.insert( "value", dictValue );
	root.insert( "dictionaryOfDictionaries", dictOfDicts );

	// Reassign modified root object to QJsonDocument
	json_.setObject( root );

	emit Edited();
}
