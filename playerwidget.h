#pragma once

#include <QWidget>
#include "ui_playerwidget.h"

QT_BEGIN_NAMESPACE namespace Ui
{
	class PlayerWidgetClass;
};

QT_END_NAMESPACE class PlayerWidget final : public QWidget
{
	Q_OBJECT public:
	explicit PlayerWidget( QWidget* parent = nullptr );
	~PlayerWidget() override;

	void UpdateWidgets( const QJsonDocument& json );
	void UpdatePlayerInfo() const;
	void SetJsonValue( QJsonDocument& json ) const;

	void SetVisible( bool visible ) const;

signals:
	void Edited();

private slots:
	void ValueChanged();
	void SavePlayerInfo();

private:
	Ui::PlayerWidgetClass* ui;

	QJsonDocument json_;
};
