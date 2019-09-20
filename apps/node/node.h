#ifndef	_NODE_H_
#define	_NODE_H_

//#ifdef __cplusplus
//	extern "C" {
//#endif


int get_node_item(const char * node,int &value);
int set_node_item(const char * node,int value);
int get_node_item(const char * node,double &value);
int set_node_item(const char * node,double value);
int get_node_item(const char * node,std::string &str);
int set_node_item(const char * node,std::string str);


//#ifdef __cplusplus
//	}
//#endif


#endif

