

#ifndef _PROCESS_H_
#define _PROCESS_H_


#ifdef __cplusplus
extern "C" {
#endif

class Process
{
private:
    std::string company;
    long shares;
    double share_val;
    double total_val;
    void set_tot() { total_val = shares * share_val; }

public:
    Process();
    Stock10(const std::string & co, long n = 0, double pr = 0.0);
    ~Process();      
    
    void create_process(std::string path);
    void update_process();
private:
	int get_process_id(std::string process_name);
};

#ifdef __cplusplus
}
#endif


#endif
