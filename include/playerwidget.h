#pragma once

#include <QWidget>
#include "ui_playerwidget.h"

#include "jsonwrapper.h"

QT_BEGIN_NAMESPACE namespace Ui
{
	class PlayerWidgetClass;
};

QT_END_NAMESPACE class PlayerWidget final : public QWidget
{
	Q_OBJECT public:
	explicit PlayerWidget( QWidget* parent = nullptr );
	~PlayerWidget() override;

	void changeEvent(QEvent* e);

	void UpdateWidgets( const JsonWrapper& json );
	void UpdatePlayerInfo() const;
	void SetJsonValue( JsonWrapper& json ) const;

	void SetVisible( bool visible ) const;

signals:
	void Edited();

private slots:
	void AddPlayer();
	void RemovePlayer();

	void ValueChanged();
	void SavePlayerInfo();

private:
	Ui::PlayerWidgetClass* ui;

	JsonWrapper defaultJson;

	inline static const QMap < QString, int > PlayerStats = {
		{ "playerHealth", 100 },
		{ "playerUpgradeHealth", 0 },
		{ "playerUpgradeStamina", 0 },
		{ "playerUpgradeExtraJump", 0 },
		{ "playerUpgradeLaunch", 0 },
		{ "playerUpgradeMapPlayerCount", 0 },
		{ "playerUpgradeSpeed", 0 },
		{ "playerUpgradeStrength", 0 },
		{ "playerUpgradeRange", 0 },
		{ "playerUpgradeThrow", 0 },
		{ "playerHasCrown", 0 }
	};
};
