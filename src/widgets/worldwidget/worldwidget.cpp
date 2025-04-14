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

void WorldWidget::changeEvent(QEvent* e)
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

void WorldWidget::UpdateWidgets( const JsonWrapper& json ) const
{
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

	SetVisible( !json.IsEmpty() );

	ui->levelSpinBox->setValue( json.Get( PropertyPath::RunStatsPath( "level" ) ).toInt() );
	ui->currencySpinBox->setValue( json.Get( PropertyPath::RunStatsPath( "currency" ) ).toInt() );
	ui->liveSpinBox->setValue( json.Get( PropertyPath::RunStatsPath( "lives" ) ).toInt() );
	ui->chargeSpinBox->setValue( json.Get( PropertyPath::RunStatsPath( "chargingStationCharge" ) ).toInt() );
	ui->totalHaulSpinBox->setValue( json.Get( PropertyPath::RunStatsPath( "totalHaul" ) ).toInt() );
	ui->teamLineEdit->setText( json.Get( PropertyPath::TeamNamePath() ).toString() );
	ui->dateEdit->setDate( QDate::fromString( json.Get( PropertyPath::DateAndTimePath() ).toString(), "yyyy-MM-dd" ) );
	ui->timePlayedDoubleSpinBox->setValue( json.Get( PropertyPath::TimePlayedPath() ).toDouble() );

	UpdateTimePlayedLabel();
}

void WorldWidget::SetJsonValue( JsonWrapper& json ) const
{
	json.Set( PropertyPath::RunStatsPath( "level" ), ui->levelSpinBox->value() );
	json.Set( PropertyPath::RunStatsPath( "currency" ), ui->currencySpinBox->value() );
	json.Set( PropertyPath::RunStatsPath( "lives" ), ui->liveSpinBox->value() );
	json.Set( PropertyPath::RunStatsPath( "chargingStationCharge" ), ui->chargeSpinBox->value() );
	json.Set( PropertyPath::RunStatsPath( "totalHaul" ), ui->totalHaulSpinBox->value() );
	json.Set( PropertyPath::TeamNamePath(), ui->teamLineEdit->text() );
	json.Set( PropertyPath::DateAndTimePath(), ui->dateEdit->date().toString( "yyyy-MM-dd" ) );
	json.Set( PropertyPath::TimePlayedPath(), ui->timePlayedDoubleSpinBox->value() );
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
		ui->timePlayedLabel->setText( tr("Time Played") );
		return;
	}

	const int hours = static_cast < int >( totalSeconds / 3600 );
	const int minutes = static_cast < int >( ( totalSeconds - hours * 3600 ) / 60 );
	const int secs = static_cast < int >( totalSeconds - hours * 3600 - minutes * 60 );

	ui->timePlayedLabel->setText( QString( tr("Time Played (%1h %2m %3s)") ).arg( hours ).arg( minutes ).arg( secs ) );
}

void WorldWidget::ValueChanged()
{
	emit Edited();
}
