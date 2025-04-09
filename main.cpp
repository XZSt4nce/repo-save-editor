#include "stdafx.h"
#include "Windows.h"
#include "reposaveeditor.h"
#include <QLocale>
#include <QtWidgets/QApplication>

int main( int argc, char* argv[ ] )
{
#if defined( _DEBUG )
	AllocConsole();
	SetConsoleOutputCP( CP_UTF8 );
	SetConsoleTitle( L"Console" );
	FILE* pConOut;
	bool _ = freopen_s( &pConOut, "CONOUT$", "w", stdout );
	FILE* pConErr;
	_ = freopen_s( &pConErr, "CONOUT$", "w", stderr );
#endif

	QApplication a( argc, argv );

	QTranslator translator;
	QString locale = QLocale::system().name();

	if (translator.load(QString("Translation_%1.qm").arg(locale), QString("src/i11n/%1").arg(locale)))
	{
		a.installTranslator(&translator);
	}

	RepoSaveEditor w;
	Global::SetApplication( &w );
	w.show();


	return QApplication::exec();
}
