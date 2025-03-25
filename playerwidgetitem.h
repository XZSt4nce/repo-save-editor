#pragma once

#include <QWidget>
#include "ui_playerwidgetitem.h"

QT_BEGIN_NAMESPACE namespace Ui
{
	class PlayerWidgetItemClass;
};

QT_END_NAMESPACE class PlayerWidgetItem final : public QWidget
{
	Q_OBJECT public:
	explicit PlayerWidgetItem( QWidget* parent = nullptr );
	~PlayerWidgetItem() override;

	void UpdateWidgets( const QJsonDocument& json, const QString& steamId );
	void SetJsonValues( QJsonDocument& json ) const;

signals:
	void Edited();

private slots:
	void ValueChanged();

private:
	Ui::PlayerWidgetItemClass* ui;

	QString steamId_;
};
