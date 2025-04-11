#include "stdafx.h"
#include "playerwidget.h"

#include "playereditionwindow.h"

PlayerWidget::PlayerWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::PlayerWidgetClass() )
{
	ui->setupUi( this );

	connect( ui->playerComboBox, &QComboBox::currentIndexChanged, this, &PlayerWidget::UpdatePlayerInfo );

	connect( ui->addButton, &QPushButton::clicked, this, &PlayerWidget::AddPlayer );
	connect( ui->removeButton, &QPushButton::clicked, this, &PlayerWidget::RemovePlayer );

	connect( ui->resetButton, &QPushButton::clicked, this, &PlayerWidget::UpdatePlayerInfo );
	connect( ui->saveButton, &QPushButton::clicked, this, &PlayerWidget::SavePlayerInfo );
}

PlayerWidget::~PlayerWidget()
{
	delete ui;
}

void PlayerWidget::changeEvent(QEvent* e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
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

	ui->playerHealthSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerUpgrade( steamId, "playerHealth" ) ).toInt() );
	ui->healthSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeHealth" ) ).toInt() );
	ui->staminaSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeStamina" ) ).toInt() );
	ui->extraJumpSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeExtraJump" ) ).toInt() );
	ui->launchSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeLaunch" ) ).toInt() );
	ui->mapPlayerCountSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeMapPlayerCount" ) ).toInt() );
	ui->speedSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeSpeed" ) ).toInt() );
	ui->strengthSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeStrength" ) ).toInt() );
	ui->rangeSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeRange" ) ).toInt() );
	ui->throwSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeThrow" ) ).toInt() );
	ui->hasCrownCheckBox->setCheckState( defaultJson.Get( PropertyPath::PlayerUpgrade( steamId, "playerHasCrown" ) ).toInt() == 1 ? Qt::Checked : Qt::Unchecked );
}

void PlayerWidget::SetJsonValue( JsonWrapper& json ) const
{
	for ( int i = 0; i < ui->playerComboBox->count(); ++i )
	{
		const QString steamId = ui->playerComboBox->itemData( i ).toString();

		const PropertyPath playerHealth = PropertyPath::PlayerUpgrade( steamId, "playerHealth" );
		json.Set( playerHealth, defaultJson.Get( playerHealth ).toInt() );

		const PropertyPath playerUpgradeHealth = PropertyPath::PlayerUpgrade( steamId, "playerUpgradeHealth" );
		json.Set( playerUpgradeHealth, defaultJson.Get( playerUpgradeHealth ).toInt() );

		const PropertyPath playerUpgradeStamina = PropertyPath::PlayerUpgrade( steamId, "playerUpgradeStamina" );
		json.Set( playerUpgradeStamina, defaultJson.Get( playerUpgradeStamina ).toInt() );

		const PropertyPath playerUpgradeExtraJump = PropertyPath::PlayerUpgrade( steamId, "playerUpgradeExtraJump" );
		json.Set( playerUpgradeExtraJump, defaultJson.Get( playerUpgradeExtraJump ).toInt() );

		const PropertyPath playerUpgradeLaunch = PropertyPath::PlayerUpgrade( steamId, "playerUpgradeLaunch" );
		json.Set( playerUpgradeLaunch, defaultJson.Get( playerUpgradeLaunch ).toInt() );

		const PropertyPath playerUpgradeMapPlayerCount = PropertyPath::PlayerUpgrade( steamId, "playerUpgradeMapPlayerCount" );
		json.Set( playerUpgradeMapPlayerCount, defaultJson.Get( playerUpgradeMapPlayerCount ).toInt() );

		const PropertyPath playerUpgradeSpeed = PropertyPath::PlayerUpgrade( steamId, "playerUpgradeSpeed" );
		json.Set( playerUpgradeSpeed, defaultJson.Get( playerUpgradeSpeed ).toInt() );

		const PropertyPath playerUpgradeStrength = PropertyPath::PlayerUpgrade( steamId, "playerUpgradeStrength" );
		json.Set( playerUpgradeStrength, defaultJson.Get( playerUpgradeStrength ).toInt() );

		const PropertyPath playerUpgradeRange = PropertyPath::PlayerUpgrade( steamId, "playerUpgradeRange" );
		json.Set( playerUpgradeRange, defaultJson.Get( playerUpgradeRange ).toInt() );

		const PropertyPath playerUpgradeThrow = PropertyPath::PlayerUpgrade( steamId, "playerUpgradeThrow" );
		json.Set( playerUpgradeThrow, defaultJson.Get( playerUpgradeThrow ).toInt() );

		const PropertyPath playerHasCrown = PropertyPath::PlayerUpgrade( steamId, "playerHasCrown" );
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

void PlayerWidget::AddPlayer()
{
	if (defaultJson.IsNull())
		return;

	auto* window = new PlayerEditionWindow(this, defaultJson);
	window->show();
	window->SetEditionMode(PlayerEditionWindow::ePlayerEditionMode::Add);
	connect(window, &PlayerEditionWindow::Edited, this, [this] { UpdateWidgets(defaultJson); });
}

void PlayerWidget::RemovePlayer()
{
	if (defaultJson.IsNull())
		return;

	auto* window = new PlayerEditionWindow(this, defaultJson);
	window->show();
	window->SetEditionMode(PlayerEditionWindow::ePlayerEditionMode::Remove);
	connect(window, &PlayerEditionWindow::Edited, this, [this] { UpdateWidgets(defaultJson); });
}

void PlayerWidget::SavePlayerInfo()
{
	const QString steamId = ui->playerComboBox->currentData().toString();

	defaultJson.Set( PropertyPath::PlayerUpgrade( steamId, "playerHealth" ), ui->playerHealthSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeHealth" ), ui->healthSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeStamina" ), ui->staminaSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeExtraJump" ), ui->extraJumpSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeLaunch" ), ui->launchSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeMapPlayerCount" ), ui->mapPlayerCountSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeSpeed" ), ui->speedSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeStrength" ), ui->strengthSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeRange" ), ui->rangeSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerUpgrade( steamId, "playerUpgradeThrow" ), ui->throwSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerUpgrade( steamId, "playerHasCrown" ), ui->hasCrownCheckBox->checkState() == Qt::Checked ? 1 : 0 );

	emit Edited();
}
