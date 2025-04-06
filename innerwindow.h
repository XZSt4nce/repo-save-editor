#pragma once

#include <QDialog>

class InnerWindow : public QDialog
{
	Q_OBJECT public:
	explicit InnerWindow( QWidget* parent = nullptr );

public slots:
	void Recenter() const;
	void showEvent( QShowEvent* event ) override;

signals:
	void WindowClosed();

protected:
	void keyPressEvent( QKeyEvent* event ) override;
	void closeEvent( QCloseEvent* event ) override;

private:
	QPropertyAnimation* animation;
};
