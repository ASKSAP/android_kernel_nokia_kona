#ifdef CONFIG_RECORD_CRASH_INFO
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/io.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/kallsyms.h>

#define CRASH_RECORD_REGION_BASE_ADDR 0x95717000
#define CRASH_RECORD_REGION_SIZE 0x1000

#define CRASH_RECORD_MAGIC 0x4352494E
#define MAX_CRASH_INFO_DES_SIZE 256
#define CRASH_INFO_UNKNOWN "UNKNOWN"
#define CRASH_INFO_UNKNOWN_LEN 7

static bool b_crash_record_inited = false;

struct crash_info {
    unsigned int magic;
    char main_reason[MAX_CRASH_INFO_DES_SIZE];
    char sub_reason[MAX_CRASH_INFO_DES_SIZE];
};

struct crash_info __iomem *crash_info_region_base;

void set_crash_info_main_reason(const char *str)
{
    int len = 0;
    if(!b_crash_record_inited)
    {
        pr_err("not inited skip set main reason\n");
        return;
    }
    len = strlen(str);
    if(len > (MAX_CRASH_INFO_DES_SIZE - 1))
    {
        pr_err("skip set for too long\n");
        return;
    }
    memcpy_toio(crash_info_region_base->main_reason, str, len + 1);
}

void set_crash_info_sub_reason(const char *str)
{
    int len = 0;
    if(!b_crash_record_inited)
    {
        pr_err("not inited skip set sub reason\n");
        return;
    }
    len = strlen(str);
    if(len > (MAX_CRASH_INFO_DES_SIZE - 1))
    {
        pr_err("skip set for too long\n");
        return;
    }
    memcpy_toio(crash_info_region_base->sub_reason, str, len + 1);
}

void set_crash_info_sub_reason_with_addr(unsigned long addr)
{
    int len = 0;
    char sym[KSYM_SYMBOL_LEN];
    unsigned long offset, size;

    if(!b_crash_record_inited)
    {
        pr_err("not inited skip set sub reason\n");
        return;
    }

    kallsyms_lookup(addr, &size, &offset, NULL, sym);
    len = strlen(sym);
    sprintf(sym + len, "+%#lx/%#lx", offset, size);
    memcpy_toio(crash_info_region_base->sub_reason, sym, KSYM_SYMBOL_LEN);
}

static int __init crash_record_init(void)
{
    pr_info("init\n");
    crash_info_region_base = ioremap(CRASH_RECORD_REGION_BASE_ADDR, CRASH_RECORD_REGION_SIZE);
    if (!crash_info_region_base)
    {
        pr_err("error to ioremap crash_record base\n");
        return -1;
    }

    crash_info_region_base->magic = CRASH_RECORD_MAGIC;
    memcpy_toio(crash_info_region_base->main_reason, CRASH_INFO_UNKNOWN, CRASH_INFO_UNKNOWN_LEN);
    memcpy_toio(crash_info_region_base->sub_reason, CRASH_INFO_UNKNOWN, CRASH_INFO_UNKNOWN_LEN);

    b_crash_record_inited = true;

    return 0;
}

device_initcall(crash_record_init);
#endif
