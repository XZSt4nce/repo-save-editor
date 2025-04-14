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
};
