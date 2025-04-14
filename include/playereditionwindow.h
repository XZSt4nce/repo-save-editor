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

	explicit PlayerEditionWindow( QWidget* parent, JsonWrapper& json_, ePlayerEditionMode mode, static const QMap < QString, int > playerStats );
	~PlayerEditionWindow() override;

signals:
	void Edited();

private slots:
	void AddPlayer();
	void RemovePlayer();

	void DisableButtons();
	void EnableButtons();

	void SetEditionMode( ePlayerEditionMode mode );

protected:
	void changeEvent(QEvent* e) override;
	void keyPressEvent(QKeyEvent* event) override;

private:
	Ui::PlayerEditionWindowClass* ui;

	JsonWrapper& json;

	const QMap < QString, int > PlayerStats;

	void SetupAddMode();
	void SetupRemoveMode();
};
