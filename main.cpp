#include "stdafx.h"
#include "Windows.h"
#include "reposaveeditor.h"
#include <QtWidgets/QApplication>

int main( int argc, char* argv[ ] )
{
	AllocConsole();
	SetConsoleOutputCP( CP_UTF8 );
	SetConsoleTitle( L"Console" );
	FILE* pConOut;
	bool _ = freopen_s( &pConOut, "CONOUT$", "w", stdout );
	FILE* pConErr;
	_ = freopen_s( &pConErr, "CONOUT$", "w", stderr );

	QApplication a( argc, argv );

	RepoSaveEditor w;
	w.show();

	return QApplication::exec();
}
