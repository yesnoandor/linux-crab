#include	"common.h"

#include	"log4cplus/logger.h"
#include	"log4cplus/fileappender.h"
#include	"log4cplus/layout.h"
#include	"log4cplus/ndc.h"
#include	"log4cplus/helpers/loglog.h"
#include	"log4cplus/loggingmacros.h"
#include	"log4cplus/consoleappender.h"


using namespace log4cplus;
using namespace log4cplus::helpers;
using namespace std;


void log4cplus_test(void)
{
	/* step 1: Instantiate an appender object */
	SharedAppenderPtr append(new ConsoleAppender());
	append->setName("append for test");
	/* step 2: Instantiate a layout object */
	string pattern = "%d{%m/%d/%y %H:%M:%S}  - %m [%l]%n";
	auto_ptr<Layout> layout(new PatternLayout(pattern));
	/* step 3: Attach the layout object to the appender */
	append->setLayout(layout);
	/* step 4: Instantiate a logger object */
	Logger logger = Logger::getInstance("test");
	/* step 5: Attach the appender object to the logger  */
	logger.addAppender(append);
	/* step 6: Set a priority for the logger  */
	logger.setLogLevel(ALL_LOG_LEVEL);
	/* log activity */
	LOG4CPLUS_DEBUG(logger, "This is the FIRST log message");
	sleep(1);
	LOG4CPLUS_WARN(logger, "This is the SECOND log message");
}
