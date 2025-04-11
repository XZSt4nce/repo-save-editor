#include "stdafx.h"
#include "playereditionwindow.h"

#include <QtNetwork/QNetworkReply>

PlayerEditionWindow::PlayerEditionWindow(QWidget* parent, JsonWrapper& json_) : InnerWindow(parent), ui(new Ui::PlayerEditionWindowClass()), json(json_)
{
	ui->setupUi(this);

	connect(ui->closeButton, &QPushButton::pressed, this, &PlayerEditionWindow::close);
	connect(ui->cancelButton, &QPushButton::pressed, this, &PlayerEditionWindow::close);
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

void PlayerEditionWindow::AddPlayer()
{
	const QString profileId = ui->idLineEdit->text().trimmed();
	std::string inputType = "Steam URL";
	QUrl profileUrl;

	if (profileId.startsWith("https://steamcommunity.com/id/")) {
		profileUrl = profileId;
	}
	else if (profileId.length() == 17 && profileId.startsWith("7656119")) {
		profileUrl = QString::fromStdString(std::format("{}{}", "https://steamcommunity.com/profiles/", profileId.toStdString()));
		inputType = "SteamID";
	}
	else {
		profileUrl = QString::fromStdString(std::format("{}{}", "https://steamcommunity.com/id/", profileId.toStdString()));
	}

	if (!profileUrl.isValid() || profileUrl.isEmpty())
	{
		qDebug() << std::format("%s invalide.", inputType);
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
				qDebug() << tr("SteamID found:") << steamId << personaName;

				QMessageBox::information(this, tr("SteamID found"), QString(tr("SteamID found: %1")).arg(steamId));

				json.Set(PropertyPath::PlayerNamePath(steamId), personaName);

				for (const QString& key : PlayerStats.keys())
					json.Set(PropertyPath::PlayerUpgrade(steamId, key), PlayerStats[key]);

				emit Edited();
				this->close();
			}
			else
			{
				QMessageBox::critical(this, tr("Error"), tr("Unable to find data in the provided URL/ID."));
			}
		});

	manager->get(QNetworkRequest(profileUrl));
}

void PlayerEditionWindow::RemovePlayer()
{
	const QString steamId = ui->removeComboBox->currentData().toString();

	for (const QString& key : PlayerStats.keys())
		json.Remove(PropertyPath::PlayerUpgrade(steamId, key));

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

	connect(ui->utilityButton, &QPushButton::pressed, this, &PlayerEditionWindow::AddPlayer);
}

void PlayerEditionWindow::SetupRemoveMode()
{
	ui->titleLabel->setText(tr("Remove Player"));
	ui->utilityButton->setText(tr("Remove"));
	ui->stackedWidget->setCurrentWidget(ui->removePage);

	for (const QString& steamId : json.GetPlayerIds())
	{
		const QString playerName = json.GetPlayerName(steamId);
		ui->removeComboBox->addItem(playerName, steamId);
	}

	connect(ui->utilityButton, &QPushButton::pressed, this, &PlayerEditionWindow::RemovePlayer);
}
