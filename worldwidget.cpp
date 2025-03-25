#include "stdafx.h"
#include "worldwidget.h"
#include "reposaveeditor.h"

WorldWidget::WorldWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::WorldWidgetClass() )
{
	ui->setupUi( this );

	connect( ui->timePlayedDoubleSpinBox, QOverload < double >::of( &QDoubleSpinBox::valueChanged ), this, &WorldWidget::UpdateTimePlayedLabel );

	connect( ui->levelSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &WorldWidget::ValueChanged );
	connect( ui->currencySpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &WorldWidget::ValueChanged );
	connect( ui->liveSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &WorldWidget::ValueChanged );
	connect( ui->chargeSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &WorldWidget::ValueChanged );
	connect( ui->totalHaulSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &WorldWidget::ValueChanged );
	connect( ui->teamLineEdit, &QLineEdit::textChanged, this, &WorldWidget::ValueChanged );
	connect( ui->dateEdit, &QDateEdit::dateChanged, this, &WorldWidget::ValueChanged );
	connect( ui->timePlayedDoubleSpinBox, QOverload < double >::of( &QDoubleSpinBox::valueChanged ), this, &WorldWidget::ValueChanged );
}

WorldWidget::~WorldWidget()
{
	delete ui;
}

void WorldWidget::UpdateWidgets( const QJsonDocument& json ) const
{
	const QJsonObject& runObj = json[ "dictionaryOfDictionaries" ].toObject().value( "value" ).toObject().value( "runStats" ).toObject();

	// Block signals to prevent signals from being emitted when setting values
	QSignalBlocker blockers[ ] = {
		QSignalBlocker( ui->levelSpinBox ),
		QSignalBlocker( ui->currencySpinBox ),
		QSignalBlocker( ui->liveSpinBox ),
		QSignalBlocker( ui->chargeSpinBox ),
		QSignalBlocker( ui->totalHaulSpinBox ),
		QSignalBlocker( ui->teamLineEdit ),
		QSignalBlocker( ui->dateEdit ),
		QSignalBlocker( ui->timePlayedDoubleSpinBox )
	};

	SetVisible( true );

	ui->levelSpinBox->setValue( runObj.value( "level" ).toInt() );
	ui->currencySpinBox->setValue( runObj.value( "currency" ).toInt() );
	ui->liveSpinBox->setValue( runObj.value( "lives" ).toInt() );
	ui->chargeSpinBox->setValue( runObj.value( "chargingStationCharge" ).toInt() );
	ui->totalHaulSpinBox->setValue( runObj.value( "totalHaul" ).toInt() );
	ui->teamLineEdit->setText( json[ "teamName" ].toObject().value( "value" ).toString() );
	ui->dateEdit->setDate( QDate::fromString( json[ "dateAndTime" ].toObject().value( "value" ).toString(), "yyyy-MM-dd" ) );
	ui->timePlayedDoubleSpinBox->setValue( json[ "timePlayed" ].toObject().value( "value" ).toDouble() );

	UpdateTimePlayedLabel();
}

void WorldWidget::SetJsonValue( QJsonDocument& json ) const
{
	// Copy QJsonDocument to QJsonObject
	QJsonObject root = json.object();

	// Modify "runStats"
	QJsonObject runObj;
	runObj.insert( "level", ui->levelSpinBox->value() );
	runObj.insert( "currency", ui->currencySpinBox->value() );
	runObj.insert( "lives", ui->liveSpinBox->value() );
	runObj.insert( "chargingStationCharge", ui->chargeSpinBox->value() );
	runObj.insert( "totalHaul", ui->totalHaulSpinBox->value() );

	QJsonObject dictOfDicts = root[ "dictionaryOfDictionaries" ].toObject();
	QJsonObject dictOfDictsValue = dictOfDicts[ "value" ].toObject();
	dictOfDictsValue.insert( "runStats", runObj );
	dictOfDicts.insert( "value", dictOfDictsValue );
	root.insert( "dictionaryOfDictionaries", dictOfDicts );

	// Modify "teamName"
	QJsonObject teamNameObj = root[ "teamName" ].toObject();
	teamNameObj.insert( "value", ui->teamLineEdit->text() );
	root.insert( "teamName", teamNameObj );

	// Modify "dateAndTime"
	QJsonObject dateAndTimeObj = root[ "dateAndTime" ].toObject();
	dateAndTimeObj.insert( "value", ui->dateEdit->date().toString( "yyyy-MM-dd" ) );
	root.insert( "dateAndTime", dateAndTimeObj );

	// Modify "timePlayed"
	QJsonObject timePlayedObj = root[ "timePlayed" ].toObject();
	timePlayedObj.insert( "value", ui->timePlayedDoubleSpinBox->value() );
	root.insert( "timePlayed", timePlayedObj );

	// Replace QJsonObject in QJsonDocument
	json.setObject( root );
}


void WorldWidget::SetVisible( const bool visible ) const
{
	ui->stackedWidget->setCurrentIndex( visible ? 1 : 0 );
}

void WorldWidget::UpdateTimePlayedLabel() const
{
	const double totalSeconds = ui->timePlayedDoubleSpinBox->value();

	if ( totalSeconds == 0.0 )
	{
		ui->timePlayedLabel->setText( "Time Played" );
		return;
	}

	const int hours = static_cast < int >( totalSeconds / 3600 );
	const int minutes = static_cast < int >( ( totalSeconds - hours * 3600 ) / 60 );
	const int secs = static_cast < int >( totalSeconds - hours * 3600 - minutes * 60 );

	ui->timePlayedLabel->setText( QString( "Time Played (%1h %2m %3s)" ).arg( hours ).arg( minutes ).arg( secs ) );
}

void WorldWidget::ValueChanged()
{
	emit Edited();
}
