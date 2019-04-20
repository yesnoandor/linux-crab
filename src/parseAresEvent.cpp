 


#include	"common.h"
#include 	"parseAresEvent.h"

using namespace std;

void parseAresEvent::splitString(const string& s, vector<string>& v, const string& c)
{
	DbgFuncEntry();
	
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
         
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));

    DbgFuncExit();    
}


void parseAresEvent::parseLine(CJsonObject& json,const string& line)
{
	DbgFuncEntry();
		
	cout << line << endl;

	json.AddEmptySubObject("event");

    vector<string> item;
    splitString(line, item," ");

    cout << "item size = " << item.size() << endl;
    
    for(vector<string>::size_type i = 0; i != item.size(); ++i)
    {
        DbgPrintf("item[%d] = %s\r\n",i,item[i].c_str());

		vector<string> v;
		splitString(item[i],v,"=");

		cout << v.size() << " " << endl;
		cout << v[0] << endl;
		cout << v[1] << endl;
		
		json["event"].Add(v[0],v[1]);
    }
    cout << "-----------------------" << endl;

    oJsonStr.push_back(json.ToString());
	
	cout << "------------------" << endl;
	cout << json.ToString() << endl;
	cout << "oJsonStr size = " << oJsonStr.size() << endl;
	cout << "------------------" << endl;

	DbgFuncExit();
}


vector<string> parseAresEvent::getNewAresEvent()
{
	return getAresEvent(m_id);
}

vector<string> parseAresEvent::getAresEvent()
{
	return oJsonStr;
}


vector<string> parseAresEvent::getAresEvent(int id)
{
	vector<string> subJsonStr;
	CJsonObject json;
	string key = "id";

	DbgFuncEntry();
	DbgPrintf("id = %d\r\n",id);
	
//	oJson
	vector<CJsonObject>::iterator it;
	
	for (it = oJson.begin();it<oJson.end();++it)  
	{
		json = *it;
		string idStr;

		cout << "json str = " << json.ToString() << endl;
		
		json["event"].Get(key,idStr);
		
		cout << "id = " << idStr << endl;

		int i = static_cast<int>(strtol(idStr.c_str(),NULL,10));
		
		cout << "i = " << i << endl;

		if(id < i)
		{
			subJsonStr.push_back(json["event"].ToString());
			m_id = i;
		}
	}

	DbgPrintf("m_id = %d\r\n",m_id);
	DbgFuncExit();
	
	return subJsonStr;
}


void parseAresEvent::onCallback()
{
	DbgFuncEntry();

	DbgGood("onCallback!!!\r\n");

	DbgFuncExit();
}

void parseAresEvent::start(const char *file)
{
	ifstream in(file);
	string line;
	CJsonObject json;

	DbgFuncEntry();

	
	if(in)
	{
		while (getline (in, line))
		{
			size_t n = line.find_last_not_of("\r\n\t" );
			if( n != string::npos )
			{
    			line.erase( n + 1 , line.size() - n );
			}

			json.Clear();
			parseLine(json,line);
			oJson.push_back(json);
        }  
    }  
    else
    {  
        cout <<"no such file" << endl;  
    }

	in.close();
	
    DbgFuncExit();
}

parseAresEvent::parseAresEvent(const char *file)
{
	m_id = 0;
	start(file);
}


parseAresEvent::parseAresEvent(string file)
{
	DbgFuncEntry();

	m_id = 0;
	start(file.c_str());

#if 0
	ifstream in(file);
	string line;
	CJsonObject json;
	
	DbgFuncEntry();

	m_id = 0;
	if(in)
	{
		while (getline (in, line))
		{
			size_t n = line.find_last_not_of("\r\n\t" );
			if( n != string::npos )
			{
				line.erase( n + 1 , line.size() - n );
			}

			json.Clear();
			parseLine(json,line);
			oJson.push_back(json);
		}  
	}  
	else
	{  
		cout <<"no such file" << endl;	
	}

	//vector<CJsonObject>::iterator it;
	
	//for (it = oJson.begin();it<oJson.end();++it)  
	//{
	//	json = *it;
	//	cout << "json str = " << json.ToString() << endl;
	//}

	
#endif

	DbgFuncExit(); 
}


parseAresEvent::~parseAresEvent()
{
}

parseAresEvent::parseAresEvent()
{
}


#if 0
neb::CJsonObject oJson("{\"refresh_interval\":60,"
                        "\"dynamic_loading\":["
                            "{"
                                "\"so_path\":\"plugins/User.so\", \"load\":false, \"version\":1,"
                                "\"cmd\":["
                                     "{\"cmd\":2001, \"class\":\"neb::CmdUserLogin\"},"
                                     "{\"cmd\":2003, \"class\":\"neb::CmdUserLogout\"}"
                                "],"
                                "\"module\":["
                                     "{\"path\":\"im/user/login\", \"class\":\"neb::ModuleLogin\"},"
                                     "{\"path\":\"im/user/logout\", \"class\":\"neb::ModuleLogout\"}"
                                "]"
                             "},"
                             "{"
                             "\"so_path\":\"plugins/ChatMsg.so\", \"load\":false, \"version\":1,"
                                 "\"cmd\":["
                                      "{\"cmd\":2001, \"class\":\"neb::CmdChat\"}"
                                 "],"
                             "\"module\":[]"
                             "}"
                        "]"
                    "}");
#endif
