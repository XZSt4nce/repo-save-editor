#pragma once

#include <QWidget>
#include "ui_itemwidgetitem.h"

#include "jsonwrapper.h"

QT_BEGIN_NAMESPACE namespace Ui
{
	class ItemWidgetItemClass;
};

QT_END_NAMESPACE class ItemWidgetItem final : public QWidget
{
	Q_OBJECT public:
	explicit ItemWidgetItem( QWidget* parent = nullptr );
	~ItemWidgetItem() override;

	void changeEvent(QEvent* e);

	void UpdateWidget( const JsonWrapper& json, const QString& itemName_ );
	void SetJsonValues( JsonWrapper& json ) const;

signals:
	void Edited();

private slots:
	void ValueChanged();

private:
	Ui::ItemWidgetItemClass* ui;

	QString itemName;
};
