#include "stdafx.h"
#include "playerwidget.h"

#include "playerwidgetitem.h"

PlayerWidget::PlayerWidget( QWidget* parent ) : QWidget( parent ), ui( new Ui::PlayerWidgetClass() )
{
	ui->setupUi( this );
	connect(ui->playerComboBox, &QComboBox::currentIndexChanged, this, &PlayerWidget::UpdatePlayerInfo);
}

PlayerWidget::~PlayerWidget()
{
	delete ui;
}

void PlayerWidget::UpdateWidgets( const QJsonDocument& json )
{
	json_ = json;

	QComboBox* playerComboBox = ui->playerComboBox;

	while (ui->playerComboBox->count() > 0)
	{
		ui->playerComboBox->removeItem(0);
	}

	for ( const QString& steamId : json[ "playerNames" ].toObject().value( "value" ).toObject().keys() )
	{
		const QString playerName = json["playerNames"].toObject().value("value").toObject().value(steamId).toString();
		playerComboBox->addItem(QString("%1|%2").arg(playerName).arg(steamId), steamId);
	}

	UpdatePlayerInfo(0);

	SetVisible( true );
}

void PlayerWidget::UpdatePlayerInfo(const uint8_t playerIndex) const
{
	while (ui->playerInfoLayout->count() > 0)
	{
		QWidget* widget = ui->playerInfoLayout->itemAt(0)->widget();
		ui->playerInfoLayout->removeWidget(widget);
		delete widget;
	}

	QString steamId = json_["playerNames"].toObject().value("value").toObject().keys()[playerIndex];

	PlayerWidgetItem* playerWidget = new PlayerWidgetItem();
	playerWidget->UpdateWidgets(json_, steamId);
	connect(playerWidget, &PlayerWidgetItem::Edited, this, &PlayerWidget::ValueChanged);

	ui->playerInfoLayout->addWidget(playerWidget);
}

void PlayerWidget::SetJsonValue( QJsonDocument& json ) const
{
	for (int i = 0; i < ui->playerInfoLayout->count(); ++i)
	{
		const auto playerWidget = qobject_cast < PlayerWidgetItem* >( ui->playerInfoLayout->itemAt(i)->widget() );
		playerWidget->SetJsonValues( json );
	}
}

void PlayerWidget::SetVisible( const bool visible ) const
{
	ui->stackedWidget->setCurrentIndex( visible ? 1 : 0 );
}

void PlayerWidget::ValueChanged()
{
	emit Edited();
}
