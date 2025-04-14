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

	const QMap< QString, QString > Items = {
		{ "Item Cart Medium", "C.A.R.T." },
		{ "Item Cart Small", tr("Pocket C.A.R.T.") },
		{ "Item Drone Battery", tr("Recharge Drone") },
		{ "Item Drone Feather", tr("Feather Drone") },
		{ "Item Drone Indestructible", tr("Indestructible Drone") },
		{ "Item Drone Zero Gravity", tr("Zero Gravity Drone") },
		{ "Item Drone Torque", tr("Roll Drone") },
		{ "Item Extraction Tracker", tr("Extraction Tracker") },
		{ "Item Grenade Duct Taped", tr("Duct Taped Grenades") },
		{ "Item Grenade Explosive", tr("Grenade") },
		{ "Item Grenade Human", tr("Human Grenade") },
		{ "Item Grenade Shockwave", tr("Shockwave Grenade") },
		{ "Item Grenade Stun", tr("Stun Grenade") },
		{ "Item Gun Handgun", tr("Gun") },
		{ "Item Gun Shotgun", tr("Shotgun") },
		{ "Item Gun Tranq", tr("Tranq Gun") },
		{ "Item Health Pack Small", tr("Small Health Pack") },
		{ "Item Health Pack Medium", tr("Medium Health Pack") },
		{ "Item Health Pack Large", tr("Large Health Pack") },
		{ "Item Melee Baseball Bat", tr("Baseball Bat") },
		{ "Item Melee Frying Pan", tr("Frying Pan") },
		{ "Item Melee Inflatable Hammer", tr("Inflatable Hammer") },
		{ "Item Melee Sledge Hammer", tr("Sledge Hammer") },
		{ "Item Melee Sword", tr("Sword") },
		{ "Item Mine Explosive", tr("Explosive Mine") },
		{ "Item Mine Shockwave", tr("Shockwave Mine") },
		{ "Item Mine Stun", tr("Stun Mine") },
		{ "Item Orb Zero Gravity", tr("Zero Gravity Orb") },
		{ "Item Power Crystal", tr("Energy Crystal") },
		{ "Item Rubber Duck", tr("Rubber Duck") },
		{ "Item Upgrade Map Player Count", tr("Map Player Count Upgrade") },
		{ "Item Upgrade PlayerEnergy", tr("Stamina Upgrade") },
		{ "Item Upgrade PlayerExtra Jump", tr("Extra Jump Upgrade") },
		{ "Item Upgrade PlayerGrab Range", tr("Range Upgrade") },
		{ "Item Upgrade PlayerGrab Strength", tr("Strength Upgrade") },
		{ "Item Upgrade PlayerHealth", tr("Health Upgrade") },
		{ "Item Upgrade PlayerSprint Speed", tr("Sprint Upgrade") },
		{ "Item Upgrade PlayerTumble Launch", tr("Tumble Launch Upgrade") },
		{ "Item Valuable Tracker", tr("Valuable Tracker") }
	};

	for ( const QString& itemName : Items.keys())
	{
		const auto itemWidget = new ItemWidgetItem(itemName, Items[itemName]);
		itemWidget->UpdateWidget( json );
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
