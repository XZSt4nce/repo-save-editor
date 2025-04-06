#include "stdafx.h"
#include "playereditionwindow.h"

PlayerEditionWindow::PlayerEditionWindow( QWidget* parent, QJsonDocument& json_ ) : InnerWindow( parent ), ui( new Ui::PlayerEditionWindowClass() ), json( json_ )
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
	QJsonObject root = json.object();

	QJsonObject dictOfDicts = root[ "dictionaryOfDictionaries" ].toObject();
	QJsonObject valueObj = dictOfDicts[ "value" ].toObject();

	const QString steamId = ui->removeComboBox->currentData().toString();

	for ( const QString& key : PlayerStats.keys() )
	{
		QJsonObject playerData = valueObj.value( key ).toObject();
		playerData.remove( steamId );
		valueObj.insert( key, playerData );
	}

	dictOfDicts.insert( "value", valueObj );
	root.insert( "dictionaryOfDictionaries", dictOfDicts );

	// playerNames part
	QJsonObject playerNamesObj = root[ "playerNames" ].toObject();
	QJsonObject playerNamesValue = playerNamesObj[ "value" ].toObject();
	playerNamesValue.remove( steamId );
	playerNamesObj.insert( "value", playerNamesValue );
	root.insert( "playerNames", playerNamesObj );

	json.setObject( root );

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

	for ( const QString& steamId : json[ "playerNames" ].toObject().value( "value" ).toObject().keys() )
	{
		const QString playerName = json[ "playerNames" ].toObject().value( "value" ).toObject().value( steamId ).toString();
		ui->removeComboBox->addItem( playerName, steamId );
	}

	connect( ui->utilityButton, &QPushButton::pressed, this, &PlayerEditionWindow::RemovePlayer );
}
