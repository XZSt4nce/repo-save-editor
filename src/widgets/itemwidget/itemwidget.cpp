#include "stdafx.h"
#include "itemwidget.h"

#include "itemwidgetitem.h"

ItemWidget::ItemWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::ItemWidgetClass() )
{
	ui->setupUi( this );
}

ItemWidget::~ItemWidget()
{
	delete ui;
}

void ItemWidget::changeEvent(QEvent* e)
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

void ItemWidget::UpdateWidgets( const JsonWrapper& json ) const
{
	while ( ui->itemLayout->count() > 0 )
	{
		QWidget* widget = ui->itemLayout->itemAt( 0 )->widget();
		ui->itemLayout->removeWidget( widget );
		delete widget;
	}

	for ( const QString& itemName : json.Get( PropertyPath::ItemsPurchasedPath() ).toObject().keys() )
	{
		const auto itemWidget = new ItemWidgetItem();
		itemWidget->UpdateWidget( json, itemName );
		connect( itemWidget, &ItemWidgetItem::Edited, this, &ItemWidget::ValueChanged );
		ui->itemLayout->insertWidget( ui->itemLayout->count(), itemWidget );
	}

	SetVisible( true );
}

void ItemWidget::SetJsonValue( JsonWrapper& json ) const
{
	for ( int i = 0; i < ui->itemLayout->count(); ++i )
	{
		const auto itemWidget = qobject_cast < ItemWidgetItem* >( ui->itemLayout->itemAt( i )->widget() );
		itemWidget->SetJsonValues( json );
	}
}

void ItemWidget::SetVisible( const bool visible ) const
{
	ui->stackedWidget->setCurrentIndex( visible ? 1 : 0 );
}

void ItemWidget::ValueChanged()
{
	emit Edited();
}
