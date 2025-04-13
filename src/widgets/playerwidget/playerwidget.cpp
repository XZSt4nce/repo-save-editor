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

	ui->playerHealthSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( "playerHealth", steamId ) ).toInt() );
	ui->healthSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( "playerUpgradeHealth", steamId ) ).toInt() );
	ui->staminaSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( "playerUpgradeStamina", steamId ) ).toInt() );
	ui->extraJumpSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( "playerUpgradeExtraJump", steamId ) ).toInt() );
	ui->launchSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( "playerUpgradeLaunch", steamId ) ).toInt() );
	ui->mapPlayerCountSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( "playerUpgradeMapPlayerCount", steamId ) ).toInt() );
	ui->speedSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( "playerUpgradeSpeed", steamId ) ).toInt() );
	ui->strengthSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( "playerUpgradeStrength", steamId ) ).toInt() );
	ui->rangeSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( "playerUpgradeRange", steamId ) ).toInt() );
	ui->throwSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( "playerUpgradeThrow", steamId ) ).toInt() );
	ui->hasCrownCheckBox->setCheckState( defaultJson.Get( PropertyPath::PlayerIdUpgrade( "playerHasCrown", steamId ) ).toInt() == 1 ? Qt::Checked : Qt::Unchecked );
}

void PlayerWidget::SetJsonValue( JsonWrapper& json ) const
{
	json.Set(PropertyPath::PlayerPath(), QJsonValue(QJsonObject()));

	for (const QString& key : PlayerStats.keys())
	{
		json.Set(PropertyPath::PlayerUpgrade(key), QJsonValue(QJsonObject()));
	}

	for ( int i = 0; i < ui->playerComboBox->count(); ++i )
	{
		const QString steamId = ui->playerComboBox->itemData( i ).toString();

		json.Set(PropertyPath::PlayerNamePath(steamId), defaultJson.GetPlayerName(steamId));

		for (const QString& key : PlayerStats.keys())
			json.Set(PropertyPath::PlayerIdUpgrade(key, steamId), defaultJson.Get(PropertyPath::PlayerIdUpgrade(key, steamId)).toInt());
	}
}

void PlayerWidget::SetVisible( const bool visible ) const
{
	ui->stackedWidget->setCurrentIndex( visible ? 1 : 0 );
}

void PlayerWidget::ValueChanged()
{
	UpdateWidgets(defaultJson);
	QMessageBox::information(this, "Remove", defaultJson.GetPlayerNames().join(", "));
	emit Edited();
}

void PlayerWidget::AddPlayer()
{
	if (defaultJson.IsNull())
		return;

	auto* window = new PlayerEditionWindow(this, defaultJson);
	window->show();
	window->SetEditionMode(PlayerEditionWindow::ePlayerEditionMode::Add);
	connect(window, &PlayerEditionWindow::Edited, this, &PlayerWidget::ValueChanged);
}

void PlayerWidget::RemovePlayer()
{
	if (defaultJson.IsNull())
		return;

	auto* window = new PlayerEditionWindow(this, defaultJson);
	window->show();
	window->SetEditionMode(PlayerEditionWindow::ePlayerEditionMode::Remove);
	connect(window, &PlayerEditionWindow::Edited, this, &PlayerWidget::ValueChanged);
}

void PlayerWidget::SavePlayerInfo()
{
	const QString steamId = ui->playerComboBox->currentData().toString();

	defaultJson.Set( PropertyPath::PlayerIdUpgrade( "playerHealth", steamId ), ui->playerHealthSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerIdUpgrade( "playerUpgradeHealth", steamId ), ui->healthSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerIdUpgrade( "playerUpgradeStamina", steamId ), ui->staminaSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerIdUpgrade( "playerUpgradeExtraJump", steamId ), ui->extraJumpSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerIdUpgrade( "playerUpgradeLaunch", steamId ), ui->launchSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerIdUpgrade( "playerUpgradeMapPlayerCount", steamId ), ui->mapPlayerCountSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerIdUpgrade( "playerUpgradeSpeed", steamId ), ui->speedSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerIdUpgrade( "playerUpgradeStrength", steamId ), ui->strengthSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerIdUpgrade( "playerUpgradeRange", steamId ), ui->rangeSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerIdUpgrade( "playerUpgradeThrow", steamId ), ui->throwSpinBox->value() );
	defaultJson.Set( PropertyPath::PlayerIdUpgrade( "playerHasCrown", steamId ), ui->hasCrownCheckBox->checkState() == Qt::Checked ? 1 : 0 );

	emit Edited();
}
