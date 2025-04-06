#include "stdafx.h"
#include "utility.h"
#include "reposaveeditor.h"
#include "innerwindow.h"

InnerWindow::InnerWindow( QWidget* parent ) : QDialog( parent ), animation( new QPropertyAnimation( this, "pos" ) )
{
	this->setWindowFlags( this->windowFlags() | Qt::FramelessWindowHint );
	this->setAttribute( Qt::WA_DeleteOnClose );

	if ( Global::Application() )
	{
		Global::Application()->setEnabled( false );
		this->setEnabled( true );

		connect( Global::Application(), &RepoSaveEditor::WindowMovedOrResized, this, &InnerWindow::Recenter );
	}

	this->setFocus();
}

void InnerWindow::Recenter() const
{
	if ( Global::Application() && this->animation )
	{
		const QPoint targetPosition = Global::Application()->frameGeometry().center() - this->rect().center();
		this->animation->setDuration( 300 ); // ms
		this->animation->setStartValue( this->pos() );
		this->animation->setEndValue( targetPosition );
		this->animation->start();
	}
}

void InnerWindow::showEvent( QShowEvent* event )
{
	QDialog::showEvent( event );

	const QPoint initialPosition = Global::Application()->frameGeometry().center() - this->rect().center();
	this->move( initialPosition );
}

void InnerWindow::keyPressEvent( QKeyEvent* event )
{
	if ( event->key() == Qt::Key_Escape )
		this->close();
}

void InnerWindow::closeEvent( QCloseEvent* event )
{
	emit this->WindowClosed();
	Global::Application()->setEnabled( true );
	Global::Application()->setFocus();
	QDialog::closeEvent( event );
}
