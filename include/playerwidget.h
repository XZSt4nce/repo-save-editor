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
};
