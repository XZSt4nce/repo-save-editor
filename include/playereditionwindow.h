#pragma once

#include "innerwindow.h"
#include "ui_playereditionwindow.h"

#include "jsonwrapper.h"

QT_BEGIN_NAMESPACE namespace Ui
{
	class PlayerEditionWindowClass;
};

QT_END_NAMESPACE class PlayerEditionWindow final : public InnerWindow
{
	Q_OBJECT public:
	enum class ePlayerEditionMode
	{
		Add,
		Remove,
	};

	explicit PlayerEditionWindow( QWidget* parent, JsonWrapper& json_ );
	~PlayerEditionWindow() override;

	void changeEvent(QEvent* e);

	void SetEditionMode( ePlayerEditionMode mode );

signals:
	void Edited();

private slots:
	void AddPlayer();
	void RemovePlayer();

private:
	Ui::PlayerEditionWindowClass* ui;

	JsonWrapper& json;

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

	void SetupAddMode();
	void SetupRemoveMode();
};
