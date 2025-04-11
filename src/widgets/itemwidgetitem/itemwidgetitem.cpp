#include "stdafx.h"
#include "itemwidgetitem.h"

ItemWidgetItem::ItemWidgetItem( QWidget* parent ) : QWidget( parent ), ui( new Ui::ItemWidgetItemClass() )
{
	ui->setupUi( this );

	connect( ui->valueSpinBox, QOverload < int >::of( &QSpinBox::valueChanged ), this, &ItemWidgetItem::ValueChanged );
}

ItemWidgetItem::~ItemWidgetItem()
{
	delete ui;
}

void ItemWidgetItem::changeEvent(QEvent* e)
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

void ItemWidgetItem::UpdateWidget( const JsonWrapper& json, const QString& itemName_ )
{
	itemName = itemName_;

	// Block signals to prevent signals from being emitted when setting values
	QSignalBlocker blockers[ ] = { QSignalBlocker( ui->valueSpinBox ) };

	ui->itemNameLabel->setText( QString( itemName ).replace( "Item ", "" ) );
	ui->valueSpinBox->setValue( json.Get( PropertyPath::ItemsPurchasedCountPath( itemName ) ).toInt() );
}

void ItemWidgetItem::SetJsonValues( JsonWrapper& json ) const
{
	json.Set( PropertyPath::ItemsPurchasedCountPath( itemName ), ui->valueSpinBox->value() );
}

void ItemWidgetItem::ValueChanged()
{
	emit Edited();
}
