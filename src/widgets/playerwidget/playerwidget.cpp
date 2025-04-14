#include "stdafx.h"
#include "playerwidget.h"

#include "playereditionwindow.h"

PlayerWidget::PlayerWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::PlayerWidgetClass() )
{
	ui->setupUi( this );

	connect( ui->playerComboBox, &QComboBox::currentIndexChanged, this, &PlayerWidget::UpdatePlayerInfo );

	connect( ui->addButton, &QPushButton::clicked, this, &PlayerWidget::AddPlayer );
	connect( ui->removeButton, &QPushButton::clicked, this, &PlayerWidget::RemovePlayer );

	connect( ui->playerHealthSpinBox, &QSpinBox::valueChanged, this, &PlayerWidget::HealthChanged );
	connect( ui->healthSpinBox, &QSpinBox::valueChanged, this, &PlayerWidget::HealthUpgradeChanged );
	connect( ui->staminaSpinBox, &QSpinBox::valueChanged, this, &PlayerWidget::StaminaUpgradeChanged );
	connect( ui->extraJumpSpinBox, &QSpinBox::valueChanged, this, &PlayerWidget::ExtraJumpUpgradeChanged );
	connect( ui->launchSpinBox, &QSpinBox::valueChanged, this, &PlayerWidget::LaunchUpgradeChanged );
	connect( ui->mapPlayerCountSpinBox, &QSpinBox::valueChanged, this, &PlayerWidget::MapPlayerCountUpgradeChanged );
	connect( ui->speedSpinBox, &QSpinBox::valueChanged, this, &PlayerWidget::SpeedUpgradeChanged );
	connect( ui->strengthSpinBox, &QSpinBox::valueChanged, this, &PlayerWidget::StrengthUpgradeChanged );
	connect( ui->rangeSpinBox, &QSpinBox::valueChanged, this, &PlayerWidget::RangeUpgradeChanged );
	connect( ui->throwSpinBox, &QSpinBox::valueChanged, this, &PlayerWidget::ThrowUpgradeChanged );
	connect( ui->hasCrownCheckBox, &QCheckBox::checkStateChanged, this, &PlayerWidget::HasCrownChanged );
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

	ui->playerHealthSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( health, steamId ) ).toInt() );
	ui->healthSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( healthUpgrade, steamId ) ).toInt() );
	ui->staminaSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( staminaUpgrade, steamId ) ).toInt() );
	ui->extraJumpSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( extraJumpUpgrade, steamId ) ).toInt() );
	ui->launchSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( launchUpgrade, steamId ) ).toInt() );
	ui->mapPlayerCountSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( mapPlayerCountUpgrade, steamId ) ).toInt() );
	ui->speedSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( speedUpgrade, steamId ) ).toInt() );
	ui->strengthSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( strengthUpgrade, steamId ) ).toInt() );
	ui->rangeSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( rangeUpgrade, steamId ) ).toInt() );
	ui->throwSpinBox->setValue( defaultJson.Get( PropertyPath::PlayerIdUpgrade( throwUpgrade, steamId ) ).toInt() );
	ui->hasCrownCheckBox->setCheckState( Qt::CheckState( defaultJson.Get( PropertyPath::PlayerIdUpgrade( hasCrown, steamId ) ).toInt() ) );
}

void PlayerWidget::SetJsonValue( JsonWrapper& json ) const
{
	json.Set(PropertyPath::PlayerPath(), QJsonValue(QJsonObject()));

	for (const QString& key : PlayerStats.keys())
	{
		json.Set(PropertyPath::PlayerUpgrade(key), QJsonValue(QJsonObject()));
	}

	for ( int i = 0; i < defaultJson.GetPlayerIds().count(); ++i )
	{
		const QString steamId = defaultJson.GetPlayerIds()[i];

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
	emit Edited();
}

void PlayerWidget::UserValueChanged(const QString& property, const int value)
{
	const QString steamId = ui->playerComboBox->currentData().toString();
	defaultJson.Set(PropertyPath::PlayerIdUpgrade(property, steamId), value);
	ValueChanged();
}

void PlayerWidget::HealthChanged(int value)
{
	UserValueChanged(health, value);
}

void PlayerWidget::HealthUpgradeChanged(int value)
{
	UserValueChanged(healthUpgrade, value);
}

void PlayerWidget::StaminaUpgradeChanged(int value)
{
	UserValueChanged(staminaUpgrade, value);
}

void PlayerWidget::ExtraJumpUpgradeChanged(int value)
{
	UserValueChanged(extraJumpUpgrade, value);
}

void PlayerWidget::LaunchUpgradeChanged(int value)
{
	UserValueChanged(launchUpgrade, value);
}

void PlayerWidget::MapPlayerCountUpgradeChanged(int value)
{
	UserValueChanged(mapPlayerCountUpgrade, value);
}

void PlayerWidget::SpeedUpgradeChanged(int value)
{
	UserValueChanged(speedUpgrade, value);
}

void PlayerWidget::StrengthUpgradeChanged(int value)
{
	UserValueChanged(strengthUpgrade, value);
}

void PlayerWidget::RangeUpgradeChanged(int value)
{
	UserValueChanged(rangeUpgrade, value);
}

void PlayerWidget::ThrowUpgradeChanged(int value)
{
	UserValueChanged(throwUpgrade, value);
}

void PlayerWidget::HasCrownChanged(Qt::CheckState checked)
{
	UserValueChanged(hasCrown, static_cast< int >( checked ));
}

void PlayerWidget::AddPlayer()
{
	if (defaultJson.IsNull())
		return;

	auto* window = new PlayerEditionWindow(this, defaultJson, PlayerEditionWindow::ePlayerEditionMode::Add, PlayerStats);
	window->show();
	connect(window, &PlayerEditionWindow::Edited, this, [this]()
		{
			ui->playerComboBox->clear();

			for (const QString& steamId : defaultJson.GetPlayerIds())
			{
				const QString playerName = defaultJson.GetPlayerName(steamId);
				ui->playerComboBox->addItem(playerName, steamId);
			}

			ValueChanged();
		});
}

void PlayerWidget::RemovePlayer()
{
	if (defaultJson.IsNull())
		return;

	auto* window = new PlayerEditionWindow(this, defaultJson, PlayerEditionWindow::ePlayerEditionMode::Remove, PlayerStats);
	window->show();
	connect( window, &PlayerEditionWindow::Edited, this, [this]()
		{
			ui->playerComboBox->clear();

			for (const QString& steamId : defaultJson.GetPlayerIds())
			{
				const QString playerName = defaultJson.GetPlayerName(steamId);
				ui->playerComboBox->addItem(playerName, steamId);
			}

			ValueChanged();
		});
}
