#ifndef _PARSE_ARES_EVENT_H
#define _PARSE_ARES_EVENT_H

#include "common.h"

using namespace std;

class parseAresEvent
{
private:
	vector<string> oJsonStr;
	vector<CJsonObject> oJson;
	map<string, string> oEvent;
	int m_id;

private:
	void parseLine(CJsonObject& json,const string& line);
	void splitString(const string& s, vector<string>& v, const string& c);
    
public:
    parseAresEvent();
    parseAresEvent(const char *file);
    parseAresEvent(string file);

    ~parseAresEvent();

	vector<string> getAresEvent();
	vector<string> getAresEvent(int id);
	vector<string> getNewAresEvent();

	void start(const char *file);
	void onCallback();
};

#endif // _PARSE_ARES_EVENT_H

