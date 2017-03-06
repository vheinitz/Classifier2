#include "gui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	/*LoggChannel *ch = new FileLoggChannel( 
				QString("LOG/helios_%2.log")
					.arg( dataRootPath )
					.arg( QDateTime::currentDateTime().toString("yyyy-MM-dd_hh_mm_ss") )
					);

		PDLogger::instance().initLogging( a.arguments() );
		PDLogger::instance().addLogChannel( ch );
*/
    GUI w;
    w.show();
    
    return a.exec();
}
