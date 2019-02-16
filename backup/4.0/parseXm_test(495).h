#ifndef MONIT_PARSER_XML_TEST_H
#define MONIT_PARSER_XML_TEST_H
#include <string>

void printProcessName();
void printCommandPath(std::string processName);
void printReturnValueDescription(std::string processName, int key);

#endif