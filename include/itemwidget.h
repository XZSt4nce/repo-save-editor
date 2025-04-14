#pragma once

#include <QWidget>
#include "ui_itemwidget.h"

#include "jsonwrapper.h"

QT_BEGIN_NAMESPACE namespace Ui
{
	class ItemWidgetClass;
};

QT_END_NAMESPACE class ItemWidget final : public QWidget
{
	Q_OBJECT public:
	explicit ItemWidget( QWidget* parent = nullptr );
	~ItemWidget() override;

	void UpdateWidgets( const JsonWrapper& json ) const;
	void SetJsonValue( JsonWrapper& json ) const;
	
	void SetVisible( bool visible ) const;
	
signals:
	void Edited();
	
private slots:
	void ValueChanged();
	
protected:
	void changeEvent(QEvent* e);
	
private:
	Ui::ItemWidgetClass* ui;

	const QMap< QString, std::function< QString()> > Items = {
		{ "Item Cart Medium", [] { return "C.A.R.T."; } },
		{ "Item Cart Small", [] { return tr("Pocket C.A.R.T."); } },
		{ "Item Drone Battery", [] { return tr("Recharge Drone"); } },
		{ "Item Drone Feather", [] { return tr("Feather Drone"); } },
		{ "Item Drone Indestructible", [] { return tr("Indestructible Drone"); } },
		{ "Item Drone Zero Gravity", [] { return tr("Zero Gravity Drone"); } },
		{ "Item Drone Torque", [] { return tr("Roll Drone"); } },
		{ "Item Extraction Tracker", [] { return tr("Extraction Tracker"); } },
		{ "Item Grenade Duct Taped", [] { return tr("Duct Taped Grenades"); } },
		{ "Item Grenade Explosive", [] { return tr("Grenade"); } },
		{ "Item Grenade Human", [] { return tr("Human Grenade"); } },
		{ "Item Grenade Shockwave", [] { return tr("Shockwave Grenade"); } },
		{ "Item Grenade Stun", [] { return tr("Stun Grenade"); } },
		{ "Item Gun Handgun", [] { return tr("Gun"); } },
		{ "Item Gun Shotgun", [] { return tr("Shotgun"); } },
		{ "Item Gun Tranq", [] { return tr("Tranq Gun"); } },
		{ "Item Health Pack Small", [] { return tr("Small Health Pack"); } },
		{ "Item Health Pack Medium", [] { return tr("Medium Health Pack"); } },
		{ "Item Health Pack Large", [] { return tr("Large Health Pack"); } },
		{ "Item Melee Baseball Bat", [] { return tr("Baseball Bat"); } },
		{ "Item Melee Frying Pan", [] { return tr("Frying Pan"); } },
		{ "Item Melee Inflatable Hammer", [] { return tr("Inflatable Hammer"); } },
		{ "Item Melee Sledge Hammer", [] { return tr("Sledge Hammer"); } },
		{ "Item Melee Sword", [] { return tr("Sword"); } },
		{ "Item Mine Explosive", [] { return tr("Explosive Mine"); } },
		{ "Item Mine Shockwave", [] { return tr("Shockwave Mine"); } },
		{ "Item Mine Stun", [] { return tr("Stun Mine"); } },
		{ "Item Orb Zero Gravity", [] { return tr("Zero Gravity Orb"); } },
		{ "Item Power Crystal", [] { return tr("Energy Crystal"); } },
		{ "Item Rubber Duck", [] { return tr("Rubber Duck"); } },
		{ "Item Upgrade Map Player Count", [] { return tr("Map Player Count Upgrade"); } },
		{ "Item Upgrade PlayerEnergy", [] { return tr("Stamina Upgrade"); } },
		{ "Item Upgrade PlayerExtra Jump", [] { return tr("Extra Jump Upgrade"); } },
		{ "Item Upgrade PlayerGrab Range", [] { return tr("Range Upgrade"); } },
		{ "Item Upgrade PlayerGrab Strength", [] { return tr("Strength Upgrade"); } },
		{ "Item Upgrade PlayerHealth", [] { return tr("Health Upgrade"); } },
		{ "Item Upgrade PlayerSprint Speed", [] { return tr("Sprint Upgrade"); } },
		{ "Item Upgrade PlayerTumble Launch", [] { return tr("Tumble Launch Upgrade"); } },
		{ "Item Valuable Tracker", [] { return tr("Valuable Tracker"); } }
	};
};
