#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/hugetlb.h>
#include <linux/mman.h>
#include <linux/mmzone.h>
#include <linux/proc_fs.h>
#include <linux/quicklist.h>
#include <linux/seq_file.h>
#include <linux/swap.h>
#include <linux/vmstat.h>
#include <linux/atomic.h>
#include <linux/vmalloc.h>
#include <asm/page.h>
#include <asm/pgtable.h>


struct sysinfo i;
unsigned long committed;
unsigned long allowed;
struct vmalloc_info vmi;
long cached;
unsigned long pages[NR_LRU_LISTS];
int lru;

static int OS2_show(struct seq_file *m, void *v){
seq_printf(m, "Proceso:\n");
seq_printf(m, "\n");

#define K(x) ((x) << (PAGE_SHIFT - 10))
si_meminfo(&i);
for (lru = LRU_BASE; lru < NR_LRU_LISTS; lru++)
pages[lru] = global_page_state(NR_LRU_BASE + lru);

seq_printf(m,"MemTotal: %8lu kB\n",K(i.totalram));
seq_printf(m,"MemFree: %8lu kB\n",K(i.freeram));
seq_printf(m,"Buffers: %8lu kB\n",K(i.bufferram));
seq_printf(m,"Active: %8lu kB\n",K(pages[LRU_ACTIVE_ANON] + pages[LRU_ACTIVE_FILE]));
seq_printf(m,"Inactive: %8lu kB\n",K(pages[LRU_INACTIVE_ANON] + pages[LRU_INACTIVE_FILE]));
seq_printf(m,"Active(anon): %8lu kB\n",K(pages[LRU_ACTIVE_ANON]));
seq_printf(m,"Inactive(anon): %8lu kB\n",K(pages[LRU_INACTIVE_ANON]));
seq_printf(m,"Active(file): %8lu kB\n",K(pages[LRU_ACTIVE_FILE]));
seq_printf(m,"Inactive(file): %8lu kB\n",K(pages[LRU_INACTIVE_FILE]));
seq_printf(m,"Unevictable: %8lu kB\n",K(pages[LRU_UNEVICTABLE]));
seq_printf(m,"Mlocked: %8lu kB\n",K(global_page_state(NR_MLOCK)));
#ifdef CONFIG_HIGHMEM
seq_printf(m,"HighTotal: %8lu kB\n",K(i.totalhigh));
seq_printf(m,"HighFree: %8lu kB\n",K(i.freehigh));
seq_printf(m,"LowTotal: %8lu kB\n",K(i.totalram-i.totalhigh));
seq_printf(m,"LowFree: %8lu kB\n",K(i.freeram-i.freehigh));
#endif
#ifndef CONFIG_MMU
seq_printf(m,"MmapCopy: %8lu kB\n",K((unsigned long) atomic_long_read(&mmap_pages_allocated)));
#endif
seq_printf(m,"SwapTotal: %8lu kB\n",K(i.totalswap));
seq_printf(m,"SwapFree: %8lu kB\n",K(i.freeswap));
#undef K
return 0;
}

/*
void __attribute__((weak)) arch_report_meminfo(struct seq_file *m)
{
}

static int meminfo_proc_show(struct seq_file *m, void *v)
{
	struct sysinfo i;
	long available;

	si_meminfo(&i);
	si_swapinfo(&i);

	available = si_mem_available();
	
	seq_printf(m,"Carne:  9430901\n");
	seq_printf(m,"Nombre: Celso Salvador Soto Taracena\n");
	seq_printf(m,"Sistema operativo: CentOS 7\n");

	seq_printf(m, "MemTotal:	%lu", i.totalram);
	seq_printf(m,"\n");	
	seq_printf(m, "MemFree:		%lu", i.freeram);
	seq_printf(m,"\n");
	seq_printf(m, "MemAvailable:	%ld", available);
	seq_printf(m,"\n");
	seq_printf(m, "HighTotal:	%lu", i.totalhigh);
	seq_printf(m,"\n");
	seq_printf(m, "HighFree:	%lu", i.freehigh);
	seq_printf(m,"\n");
	seq_printf(m, "LowTotal:	%lu", i.totalram - i.totalhigh);
	seq_printf(m,"\n");
	seq_printf(m, "LowFree:		%lu", i.freeram - i.freehigh);
	seq_printf(m,"\n");

	arch_report_meminfo(m);

	return 0;
}*/

static void __exit final(void) //Salida de modulo
{   
	printk(KERN_INFO "Limpiando.\n");
}

static int meminfo_proc_open(struct inode *inode, struct file *file)
{
	//return single_open(file, meminfo_proc_show, NULL);
	return single_open(file, OS2_show, NULL);
}

static const struct file_operations meminfo_proc_fops = {
	.open		= meminfo_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init inicio(void)
{
	proc_create("memo_9430901", 0, NULL, &meminfo_proc_fops);
	return 0;
}

module_init(inicio);
module_exit(final);

MODULE_AUTHOR("Celso Soto");
MODULE_DESCRIPTION("9430901");
MODULE_LICENSE("GPL");
