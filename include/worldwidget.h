#pragma once

#include <QWidget>
#include "ui_worldwidget.h"

#include "jsonwrapper.h"

QT_BEGIN_NAMESPACE namespace Ui
{
	class WorldWidgetClass;
};

QT_END_NAMESPACE class WorldWidget final : public QWidget
{
	Q_OBJECT public:
	explicit WorldWidget( QWidget* parent = nullptr );
	~WorldWidget() override;

	void changeEvent(QEvent* e);

	void UpdateWidgets( const JsonWrapper& json ) const;
	void SetJsonValue( JsonWrapper& json ) const;

	void SetVisible( bool visible ) const;

signals:
	void Edited();

private slots:
	void ValueChanged();
	void UpdateTimePlayedLabel() const;

private:
	Ui::WorldWidgetClass* ui;
};
