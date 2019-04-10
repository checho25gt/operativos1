#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
    

static int meminfo_proc_show(struct seq_file *m, void *v)
{
	int cont = 0;
        struct task_struct *task;
	seq_printf(m,"Carne:  9430901\n");
	seq_printf(m,"Nombre: Celso Salvador Soto Taracena\n");
	seq_printf(m,"Sistema operativo: CentOS 7\n");
        for_each_process(task)
        {
		seq_printf(m,"{\"PID\":%d,\"NOMBRE\":\"%s\",\"ESTADO\":", task->pid, task->comm);
		if(task->state == 0)
		{
			seq_printf(m,"\"R\"}");
		}
		if(task->state == 1)
		{
			seq_printf(m,"\"I\"}");
		}
		if(task->state == 2)
		{
			seq_printf(m,"\"U\"}");
		}
		if(task->state == 4)
		{
			seq_printf(m,"\"Z\"}");
		}
		if(task->state == 8)
		{
			seq_printf(m,"\"S\"}");
		}
		seq_printf(m,"\n");
		cont = cont +1;
        }
        return 0;
}

static void __exit final(void) //Salida de modulo
{   
	printk(KERN_INFO "Sistemas Operativos 1.\n");
}

static int meminfo_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, meminfo_proc_show, NULL);
}

static const struct file_operations meminfo_proc_fops = {
	.open       = meminfo_proc_open,
        .read       = seq_read,
        .llseek     = seq_lseek,
        .release    = single_release,
};

static int __init inicio(void) //Escribe archivo en /proc
{
	printk(KERN_INFO "Nombre: Celso Soto.\n");
        proc_create("cpu_9430901", 0, NULL, &meminfo_proc_fops);
        return 0;
}

module_init(inicio);
module_exit(final);

MODULE_AUTHOR("Celso Soto");
MODULE_DESCRIPTION("9430901");
MODULE_LICENSE("GPL");
