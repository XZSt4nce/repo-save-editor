#include "stdafx.h"
#include "playerwidget.h"

#include "playereditionwindow.h"

PlayerWidget::PlayerWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::PlayerWidgetClass() ), StatsEdits([this](){
	ui->setupUi(this);
	QMap<QString, QWidget*> tempMap;
	tempMap.insert(health, ui->playerHealthSpinBox);
	tempMap.insert(healthUpgrade, ui->healthSpinBox);
	tempMap.insert(staminaUpgrade, ui->staminaSpinBox);
	tempMap.insert(extraJumpUpgrade, ui->extraJumpSpinBox);
	tempMap.insert(launchUpgrade, ui->launchSpinBox);
	tempMap.insert(mapPlayerCountUpgrade, ui->mapPlayerCountSpinBox);
	tempMap.insert(speedUpgrade, ui->speedSpinBox);
	tempMap.insert(strengthUpgrade, ui->strengthSpinBox);
	tempMap.insert(rangeUpgrade, ui->rangeSpinBox);
	tempMap.insert(throwUpgrade, ui->throwSpinBox);
	tempMap.insert(hasCrown, ui->hasCrownCheckBox);
	return tempMap;
}())
{
	connect( ui->playerComboBox, &QComboBox::currentIndexChanged, this, &PlayerWidget::UpdatePlayerInfo );

	connect( ui->addButton, &QPushButton::clicked, this, &PlayerWidget::AddPlayer );
	connect( ui->removeButton, &QPushButton::clicked, this, &PlayerWidget::RemovePlayer );

	for (const QString& key : StatsEdits.keys())
	{
		QWidget* widget = StatsEdits[key];
		if (QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget)) {
			connect( spinBox, &QSpinBox::valueChanged, this, [this, key]( int value ) { UserValueChanged( key, value ); } );
		}
		else if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(widget)) {
			connect(checkBox, &QCheckBox::checkStateChanged, this, [this, key]( Qt::CheckState checked ) { UserValueChanged( key, static_cast< int >( checked ) ); } );
		}
	}
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

void PlayerWidget::SetEditsDisabled( const bool isDisabled )
{
	if (editsDisabled != isDisabled)
	{
		for (QWidget* edit : StatsEdits.values())
		{
			edit->setDisabled(isDisabled);
		}
		editsDisabled = isDisabled;
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
	else
	{
		SetEditsDisabled(true);
	}

	SetVisible( !json.IsEmpty() );
}

void PlayerWidget::UpdatePlayerInfo()
{
	const QString steamId = ui->playerComboBox->currentData().toString();
	const bool isSteamIdEmpty = steamId.isEmpty();
	QString label = QString( "Steam ID: %1" ).arg( steamId );
	std::function<int(QString)> getEditValue = [this, steamId](QString key) { return defaultJson.Get( PropertyPath::PlayerIdUpgrade( key, steamId ) ).toInt(); };

	SetEditsDisabled(isSteamIdEmpty);

	if (isSteamIdEmpty)
	{
		ui->steamIdLabel->setText( "#STEAM_ID" );

		getEditValue = [](QString) { return 0; };
	}

	ui->steamIdLabel->setText(label);

	for (const QString& key : StatsEdits.keys())
	{
		QWidget* widget = StatsEdits[key];
		int value = getEditValue( key );

		if (QSpinBox* spinBox = qobject_cast<QSpinBox*>(widget)) {
			spinBox->setValue( value );
		}
		else if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(widget)) {
			checkBox->setCheckState( Qt::CheckState( value ) );
		}
	}
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
				const QString& playerName = defaultJson.GetPlayerName(steamId);
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
