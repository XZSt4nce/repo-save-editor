#include "stdafx.h"
#include "playerwidget.h"

#include "playerwidgetitem.h"

PlayerWidget::PlayerWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::PlayerWidgetClass() )
{
	ui->setupUi( this );
}

PlayerWidget::~PlayerWidget()
{
	delete ui;
}

void PlayerWidget::UpdateWidgets( const QJsonDocument& json ) const
{
	while ( ui->playerLayout->count() > 0 )
	{
		QWidget* widget = ui->playerLayout->itemAt( 0 )->widget();
		ui->playerLayout->removeWidget( widget );
		delete widget;
	}

	for ( const QString& steamId : json[ "playerNames" ].toObject().value( "value" ).toObject().keys() )
	{
		const auto playerWidget = new PlayerWidgetItem();
		playerWidget->UpdateWidgets( json, steamId );
		connect( playerWidget, &PlayerWidgetItem::Edited, this, &PlayerWidget::ValueChanged );
		ui->playerLayout->insertWidget( ui->playerLayout->count(), playerWidget );
	}

	SetVisible( true );
}

void PlayerWidget::SetJsonValue( QJsonDocument& json ) const
{
	for ( int i = 0; i < ui->playerLayout->count(); ++i )
	{
		const auto playerWidget = qobject_cast < PlayerWidgetItem* >( ui->playerLayout->itemAt( i )->widget() );
		playerWidget->SetJsonValues( json );
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
