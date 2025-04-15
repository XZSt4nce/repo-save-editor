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

	void UpdateWidgets( const JsonWrapper& json );
	void UpdatePlayerInfo() const;
	void SetJsonValue( JsonWrapper& json ) const;

	void SetVisible( bool visible ) const;

signals:
	void Edited();

protected:
	void changeEvent(QEvent* e);

private slots:
	void AddPlayer();
	void RemovePlayer();

	void ValueChanged();
	void UserValueChanged(const QString& property, int value);

private:
	Ui::PlayerWidgetClass* ui;

	JsonWrapper defaultJson;

	inline static const QString health = "playerHealth";
	inline static const QString healthUpgrade = "playerUpgradeHealth";
	inline static const QString staminaUpgrade = "playerUpgradeStamina";
	inline static const QString extraJumpUpgrade = "playerUpgradeExtraJump";
	inline static const QString launchUpgrade = "playerUpgradeLaunch";
	inline static const QString mapPlayerCountUpgrade = "playerUpgradeMapPlayerCount";
	inline static const QString speedUpgrade = "playerUpgradeSpeed";
	inline static const QString strengthUpgrade = "playerUpgradeStrength";
	inline static const QString rangeUpgrade = "playerUpgradeRange";
	inline static const QString throwUpgrade = "playerUpgradeThrow";
	inline static const QString hasCrown = "playerHasCrown";

	const QMap < QString, QWidget* > StatsEdits;

	inline static const QMap < QString, int > PlayerStats = {
		{ health, 100 },
		{ healthUpgrade, 0 },
		{ staminaUpgrade, 0 },
		{ extraJumpUpgrade, 0 },
		{ launchUpgrade, 0 },
		{ mapPlayerCountUpgrade, 0 },
		{ speedUpgrade, 0 },
		{ strengthUpgrade, 0 },
		{ rangeUpgrade, 0 },
		{ throwUpgrade, 0 },
		{ hasCrown, 0 }
	};
};
