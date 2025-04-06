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

void PlayerWidget::UpdateWidgets( const JsonWrapper& json )
{
	defaultJson = json;
	ui->playerComboBox->clear();

	for ( const QString& steamId : json.GetPlayerIds() )
	{
		const QString playerName = json.GetPlayerName( steamId );
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

	ui->steamIdLabel->setText( QString( "Steam ID: %1" ).arg( steamId ) );

	ui->playerHealthSpinBox->setValue( defaultJson.Get( JsonPath::PlayerUpgrade( steamId, "playerHealth" ) ).toInt() );
	ui->healthSpinBox->setValue( defaultJson.Get( JsonPath::PlayerUpgrade( steamId, "playerUpgradeHealth" ) ).toInt() );
	ui->staminaSpinBox->setValue( defaultJson.Get( JsonPath::PlayerUpgrade( steamId, "playerUpgradeStamina" ) ).toInt() );
	ui->extraJumpSpinBox->setValue( defaultJson.Get( JsonPath::PlayerUpgrade( steamId, "playerUpgradeExtraJump" ) ).toInt() );
	ui->launchSpinBox->setValue( defaultJson.Get( JsonPath::PlayerUpgrade( steamId, "playerUpgradeLaunch" ) ).toInt() );
	ui->mapPlayerCountSpinBox->setValue( defaultJson.Get( JsonPath::PlayerUpgrade( steamId, "playerUpgradeMapPlayerCount" ) ).toInt() );
	ui->speedSpinBox->setValue( defaultJson.Get( JsonPath::PlayerUpgrade( steamId, "playerUpgradeSpeed" ) ).toInt() );
	ui->strengthSpinBox->setValue( defaultJson.Get( JsonPath::PlayerUpgrade( steamId, "playerUpgradeStrength" ) ).toInt() );
	ui->rangeSpinBox->setValue( defaultJson.Get( JsonPath::PlayerUpgrade( steamId, "playerUpgradeRange" ) ).toInt() );
	ui->throwSpinBox->setValue( defaultJson.Get( JsonPath::PlayerUpgrade( steamId, "playerUpgradeThrow" ) ).toInt() );
	ui->hasCrownCheckBox->setCheckState( defaultJson.Get( JsonPath::PlayerUpgrade( steamId, "playerHasCrown" ) ).toInt() == 1 ? Qt::Checked : Qt::Unchecked );
}

void PlayerWidget::SetJsonValue( JsonWrapper& json ) const
{
	for ( int i = 0; i < ui->playerComboBox->count(); ++i )
	{
		const QString steamId = ui->playerComboBox->itemData( i ).toString();

		const JsonPath playerHealth = JsonPath::PlayerUpgrade( steamId, "playerHealth" );
		json.Set( playerHealth, defaultJson.Get( playerHealth ).toInt() );

		const JsonPath playerUpgradeHealth = JsonPath::PlayerUpgrade( steamId, "playerUpgradeHealth" );
		json.Set( playerUpgradeHealth, defaultJson.Get( playerUpgradeHealth ).toInt() );

		const JsonPath playerUpgradeStamina = JsonPath::PlayerUpgrade( steamId, "playerUpgradeStamina" );
		json.Set( playerUpgradeStamina, defaultJson.Get( playerUpgradeStamina ).toInt() );

		const JsonPath playerUpgradeExtraJump = JsonPath::PlayerUpgrade( steamId, "playerUpgradeExtraJump" );
		json.Set( playerUpgradeExtraJump, defaultJson.Get( playerUpgradeExtraJump ).toInt() );

		const JsonPath playerUpgradeLaunch = JsonPath::PlayerUpgrade( steamId, "playerUpgradeLaunch" );
		json.Set( playerUpgradeLaunch, defaultJson.Get( playerUpgradeLaunch ).toInt() );

		const JsonPath playerUpgradeMapPlayerCount = JsonPath::PlayerUpgrade( steamId, "playerUpgradeMapPlayerCount" );
		json.Set( playerUpgradeMapPlayerCount, defaultJson.Get( playerUpgradeMapPlayerCount ).toInt() );

		const JsonPath playerUpgradeSpeed = JsonPath::PlayerUpgrade( steamId, "playerUpgradeSpeed" );
		json.Set( playerUpgradeSpeed, defaultJson.Get( playerUpgradeSpeed ).toInt() );

		const JsonPath playerUpgradeStrength = JsonPath::PlayerUpgrade( steamId, "playerUpgradeStrength" );
		json.Set( playerUpgradeStrength, defaultJson.Get( playerUpgradeStrength ).toInt() );

		const JsonPath playerUpgradeRange = JsonPath::PlayerUpgrade( steamId, "playerUpgradeRange" );
		json.Set( playerUpgradeRange, defaultJson.Get( playerUpgradeRange ).toInt() );

		const JsonPath playerUpgradeThrow = JsonPath::PlayerUpgrade( steamId, "playerUpgradeThrow" );
		json.Set( playerUpgradeThrow, defaultJson.Get( playerUpgradeThrow ).toInt() );

		const JsonPath playerHasCrown = JsonPath::PlayerUpgrade( steamId, "playerHasCrown" );
		json.Set( playerHasCrown, defaultJson.Get( playerHasCrown ).toInt() );
	}
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

	defaultJson.Set( JsonPath::PlayerUpgrade( steamId, "playerHealth" ), ui->playerHealthSpinBox->value() );
	defaultJson.Set( JsonPath::PlayerUpgrade( steamId, "playerUpgradeHealth" ), ui->healthSpinBox->value() );
	defaultJson.Set( JsonPath::PlayerUpgrade( steamId, "playerUpgradeStamina" ), ui->staminaSpinBox->value() );
	defaultJson.Set( JsonPath::PlayerUpgrade( steamId, "playerUpgradeExtraJump" ), ui->extraJumpSpinBox->value() );
	defaultJson.Set( JsonPath::PlayerUpgrade( steamId, "playerUpgradeLaunch" ), ui->launchSpinBox->value() );
	defaultJson.Set( JsonPath::PlayerUpgrade( steamId, "playerUpgradeMapPlayerCount" ), ui->mapPlayerCountSpinBox->value() );
	defaultJson.Set( JsonPath::PlayerUpgrade( steamId, "playerUpgradeSpeed" ), ui->speedSpinBox->value() );
	defaultJson.Set( JsonPath::PlayerUpgrade( steamId, "playerUpgradeStrength" ), ui->strengthSpinBox->value() );
	defaultJson.Set( JsonPath::PlayerUpgrade( steamId, "playerUpgradeRange" ), ui->rangeSpinBox->value() );
	defaultJson.Set( JsonPath::PlayerUpgrade( steamId, "playerUpgradeThrow" ), ui->throwSpinBox->value() );
	defaultJson.Set( JsonPath::PlayerUpgrade( steamId, "playerHasCrown" ), ui->hasCrownCheckBox->checkState() == Qt::Checked ? 1 : 0 );

	emit Edited();
}
