#include	"common.h"

#include	"parseConfig.h"

#include 	"parseAresEvent.h"


void parseAresEvent_test()
{
	string eventJson;
	
	parseAresEvent parse("mlog20190216.txt");
    vector<string> event = parse.getAresEvent();

	//so.registerCallback(std::bind(&parseAresEvent::onCallback, parse));
    for (auto it = event.begin(); it != event.end(); ++it)
    {
    	eventJson = *it;
       	cout << "item : " << eventJson << endl;
       	//so.send((unsigned char *)eventJson.c_str(),eventJson.length() + 1);
    }

    pause();
}

