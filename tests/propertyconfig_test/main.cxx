#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/fileinfo.h>
#include <log4cplus/loggingmacros.h>

#include <windows.h>
#include <string>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;


string GetCurrentDir()
{
	string str = "";

	char chPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, chPath, MAX_PATH);
	char* p = strrchr(chPath, '\\');
	if (p)
	{
		*(p + 1) = 0;
	}

	str = chPath;

	return str;
}

log4cplus::tstring
getPropertiesFileArgument (int argc, char * argv[])
{
    if (argc >= 2)
    {
        char const * arg = argv[1];
        log4cplus::tstring file = LOG4CPLUS_C_STR_TO_TSTRING (arg);
        log4cplus::helpers::FileInfo fi;
        if (getFileInfo (&fi, file) == 0)
            return file;
    }

	tostringstream buffer;
	buffer << GetCurrentDir().c_str();
	buffer << LOG4CPLUS_TEXT("log4cplus.properties");

	return buffer.str();
}

int
main(int argc, char * argv[])
{
	//tstring str1 = LOG4CPLUS_TEXT(" str1 ");

	//tostringstream buffer;
	//buffer << str1;
	//buffer << LOG4CPLUS_TEXT(".");
	//buffer << 1004;
	//buffer << LOG4CPLUS_TEXT(" str3 ");
	//buffer << LOG4CPLUS_TEXT(" .bak1 ");
	//buffer << str1;
	//buffer << 1001;
	//buffer << 1002;
	//buffer << str1;
	//buffer << 1003;

	//tcout << buffer.str() << endl;

    tcout << LOG4CPLUS_TEXT("Entering main()...") << endl;
    log4cplus::initialize ();
    LogLog::getLogLog()->setInternalDebugging(true);
    Logger root = Logger::getRoot();
    try {
        PropertyConfigurator::doConfigure(
            getPropertiesFileArgument (argc, argv));
        Logger fileCat = Logger::getInstance(LOG4CPLUS_TEXT("filelogger"));

        LOG4CPLUS_WARN(root, LOG4CPLUS_TEXT("Testing...."));
        LOG4CPLUS_WARN(root, LOG4CPLUS_TEXT("Writing messages to log...."));
        //for (int i=0; i<10; ++i)
        //LOG4CPLUS_WARN(fileCat, LOG4CPLUS_TEXT("This is a WARNING...")
        //        << i);

		for (int i = 0; i < 12000; ++i)
		{
			tostringstream buffer;
			buffer << LOG4CPLUS_TEXT("[") << i << LOG4CPLUS_TEXT("] Writing messages to log....");

			LOG4CPLUS_DEBUG(root, buffer.str());
		}

        // Test that DOS EOLs in property files get removed.
#define TEST_TEXT LOG4CPLUS_TEXT ("this is a test with DOS EOL-->")
        tistringstream propsStream (
            LOG4CPLUS_TEXT ("text=") TEST_TEXT LOG4CPLUS_TEXT ("\r\n"));
        Properties props (propsStream);
        LOG4CPLUS_ASSERT (root,
            props.getProperty (LOG4CPLUS_TEXT ("text")) == TEST_TEXT);
    }
    catch(...) {
        tcout << LOG4CPLUS_TEXT("Exception...") << endl;
        LOG4CPLUS_FATAL(root, LOG4CPLUS_TEXT("Exception occured..."));
    }

    tcout << LOG4CPLUS_TEXT("Exiting main()...") << endl;
    return 0;
}
