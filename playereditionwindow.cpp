#include "stdafx.h"
#include "playereditionwindow.h"

PlayerEditionWindow::PlayerEditionWindow( QWidget* parent, JsonWrapper& json_ ) : InnerWindow( parent ), ui( new Ui::PlayerEditionWindowClass() ), json( json_ )
{
	ui->setupUi( this );

	connect( ui->closeButton, &QPushButton::pressed, this, &PlayerEditionWindow::close );
	connect( ui->cancelButton, &QPushButton::pressed, this, &PlayerEditionWindow::close );
}

PlayerEditionWindow::~PlayerEditionWindow()
{
	delete ui;
}

void PlayerEditionWindow::SetEditionMode( const ePlayerEditionMode mode )
{
	switch ( mode )
	{
	case ePlayerEditionMode::Add:
		SetupAddMode();
		break;
	case ePlayerEditionMode::Remove:
		SetupRemoveMode();
		break;
	}
}

void PlayerEditionWindow::AddPlayer()
{
	// TODO: Add player to json

	emit Edited();

	// Close window
	this->close();
}

void PlayerEditionWindow::RemovePlayer()
{
	const QString steamId = ui->removeComboBox->currentData().toString();

	for ( const QString& key : PlayerStats.keys() )
	{
		json.Remove( JsonPath::PlayerUpgrade( steamId, key ) );
	}

	json.Remove( JsonPath::PlayerNamePath( steamId ) );

	qDebug() << "Removed player:" << steamId << "At path:" << JsonPath::PlayerNamePath( steamId ) << json.GetPlayerNames();

	emit Edited();

	// Close window
	this->close();
}

void PlayerEditionWindow::SetupAddMode()
{
	ui->titleLabel->setText( "Add New Player" );
	ui->utilityButton->setText( "Add" );
	ui->stackedWidget->setCurrentWidget( ui->addPage );

	connect( ui->utilityButton, &QPushButton::pressed, this, &PlayerEditionWindow::AddPlayer );
}

void PlayerEditionWindow::SetupRemoveMode()
{
	ui->titleLabel->setText( "Remove Player" );
	ui->utilityButton->setText( "Remove" );
	ui->stackedWidget->setCurrentWidget( ui->removePage );

	for ( const QString& steamId : json.GetPlayerIds() )
	{
		const QString playerName = json.GetPlayerName( steamId );
		ui->removeComboBox->addItem( playerName, steamId );
	}

	connect( ui->utilityButton, &QPushButton::pressed, this, &PlayerEditionWindow::RemovePlayer );
}
