#ifdef CONFIG_RECORD_CRASH_INFO
#ifndef CRASH_RECORD_H
#define CRASH_RECORD_H

void set_crash_info_main_reason(const char *str);
void set_crash_info_sub_reason(const char *str);
void set_crash_info_sub_reason_with_addr(unsigned long addr);

#endif
#endif
