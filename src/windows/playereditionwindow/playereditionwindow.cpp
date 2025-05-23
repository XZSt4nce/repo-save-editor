#include "stdafx.h"
#include "playereditionwindow.h"

#include <QtNetwork/QNetworkReply>

PlayerEditionWindow::PlayerEditionWindow(QWidget* parent, JsonWrapper& json_, ePlayerEditionMode mode, static const QMap < QString, int > playerStats) : InnerWindow(parent), ui(new Ui::PlayerEditionWindowClass()), json(json_), PlayerStats(playerStats)
{
	ui->setupUi(this);
	ui->idLineEdit->setFocus();

	SetEditionMode(mode);

	connect(ui->closeButton, &QPushButton::clicked, this, &PlayerEditionWindow::close);
	connect(ui->cancelButton, &QPushButton::clicked, this, &PlayerEditionWindow::close);
}

PlayerEditionWindow::~PlayerEditionWindow()
{
	delete ui;
}

void PlayerEditionWindow::changeEvent(QEvent* e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void PlayerEditionWindow::keyPressEvent(QKeyEvent* event)
{
	int key = event->key();
	if (key == Qt::Key_Return)
	{
		ui->utilityButton->click();
	}
	else if (key == Qt::Key_Escape)
	{
		ui->cancelButton->click();
	}
	else
	{
		QWidget::keyPressEvent(event);
	}
}

void PlayerEditionWindow::SetEditionMode(const ePlayerEditionMode mode)
{
	switch (mode)
	{
	case ePlayerEditionMode::Add:
		SetupAddMode();
		break;
	case ePlayerEditionMode::Remove:
		SetupRemoveMode();
		break;
	}
}

void PlayerEditionWindow::DisableButtons()
{
	ui->utilityButton->setDisabled(true);
	ui->cancelButton->setDisabled(true);
	ui->closeButton->setDisabled(true);
}

void PlayerEditionWindow::EnableButtons()
{
	ui->utilityButton->setDisabled(false);
	ui->cancelButton->setDisabled(false);
	ui->closeButton->setDisabled(false);
}

void PlayerEditionWindow::AddPlayer()
{
	DisableButtons();

	const QString profileId = ui->idLineEdit->text().trimmed();
	std::string inputType = "Steam URL";
	QUrl profileUrl;

	if (profileId.startsWith("https://steamcommunity.com/id/") || profileId.startsWith("https://steamcommunity.com/profiles/")) {
		profileUrl = profileId;
	}
	else if (profileId.length() == 17 && profileId.startsWith("7656119")) {
		profileUrl = QString( "https://steamcommunity.com/profiles/%1" ).arg( profileId );
		inputType = "SteamID";
	}
	else {
		profileUrl = QString( "https://steamcommunity.com/id/%1" ).arg( profileId );
	}

	if (!profileUrl.isValid() || profileUrl.isEmpty())
	{
		qDebug() << QString("%1 invalide.").arg(QString::fromStdString(inputType));

		EnableButtons();

		return;
	}

	auto* manager = new QNetworkAccessManager(this);

	connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply)
	{
		const QString html = reply->readAll();
		reply->deleteLater();

		QString steamId, personaName;

		// g_rgProfileData method
		const QRegularExpression regex(R"(g_rgProfileData\s*=\s*(\{.*?\});)");
		if (const QRegularExpressionMatch match = regex.match(html); match.hasMatch())
		{
			QJsonParseError err;
			const QString jsonText = match.captured(1);
			if (const QJsonDocument doc = QJsonDocument::fromJson(jsonText.toUtf8(), &err); err.error == QJsonParseError::NoError && doc.isObject())
			{
				steamId = doc.object().value("steamid").toString();
				personaName = doc.object().value("personaname").toString();
			}
		}

		if (!steamId.isEmpty())
		{
			qDebug() << "SteamID found:" << steamId << personaName;

			QMessageBox::information(this, tr("SteamID found"), QString(tr("SteamID found: %1")).arg(steamId));

			json.Set(PropertyPath::PlayerNamePath(steamId), personaName);

			for (const QString& key : PlayerStats.keys())
				json.Set(PropertyPath::PlayerIdUpgrade(key, steamId), PlayerStats[key]);

			emit Edited();
			this->close();
		}
		else
		{
			QMessageBox::critical(this, tr("Error"), tr("Unable to find data in the provided URL/ID."));

			EnableButtons();
		}
	});

	manager->get(QNetworkRequest(profileUrl));
}

void PlayerEditionWindow::RemovePlayer()
{
	DisableButtons();

	const QString steamId = ui->removeComboBox->currentData().toString();

	for (const QString& key : PlayerStats.keys())
		json.Remove(PropertyPath::PlayerIdUpgrade(steamId, key));

	json.Remove(PropertyPath::PlayerNamePath(steamId));
	emit Edited();

	// Close window
	this->close();
}

void PlayerEditionWindow::SetupAddMode()
{
	ui->titleLabel->setText(tr("Add New Player"));
	ui->utilityButton->setText(tr("Add"));
	ui->stackedWidget->setCurrentWidget(ui->addPage);

	disconnect(ui->utilityButton, &QPushButton::clicked, this, &PlayerEditionWindow::AddPlayer);
	connect(ui->utilityButton, &QPushButton::clicked, this, &PlayerEditionWindow::AddPlayer);
}

void PlayerEditionWindow::SetupRemoveMode()
{
	ui->titleLabel->setText(tr("Remove Player"));
	ui->utilityButton->setText(tr("Remove"));
	ui->stackedWidget->setCurrentWidget(ui->removePage);

	for (const QString& steamId : json.GetPlayerIds())
	{
		const QString playerName = json.GetPlayerName(steamId);
		ui->removeComboBox->addItem(QString("%1 | %2").arg(playerName).arg(steamId), steamId);
	}

	disconnect(ui->utilityButton, &QPushButton::clicked, this, &PlayerEditionWindow::RemovePlayer);
	connect(ui->utilityButton, &QPushButton::clicked, this, &PlayerEditionWindow::RemovePlayer);
}
