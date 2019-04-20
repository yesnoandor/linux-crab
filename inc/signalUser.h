#ifndef _SIGNAL_USER_H_
#define _SIGNAL_USER_H_

typedef void (* signal_func_t)(void); 

void register_signal_user(signal_func_t func);
void register_signal_alarm(signal_func_t func);

#endif
