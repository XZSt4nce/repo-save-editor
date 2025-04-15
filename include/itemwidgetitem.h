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
	explicit ItemWidgetItem( const QString& itemName = "", const std::function< QString()>& itemLabel = [] { return ""; }, QWidget* parent = nullptr );
	~ItemWidgetItem() override;

	void UpdateWidget( const JsonWrapper& json);
	void SetJsonValues( JsonWrapper& json ) const;

signals:
	void Edited();

private slots:
	void ValueChanged();

protected:
	void changeEvent(QEvent* e);

private:
	Ui::ItemWidgetItemClass* ui;

	const QString itemName;
	const std::function< QString()>& itemLabel;
};
