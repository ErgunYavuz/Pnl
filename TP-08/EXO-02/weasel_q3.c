#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/dcache.h>
#include <linux/list_bl.h>
#include <linux/proc_fs.h>

MODULE_DESCRIPTION("weasel");
MODULE_AUTHOR("QM & US");
MODULE_LICENSE("GPL");

static int weasel_init(void)
{
	proc_file_entry = proc_create("proc_file_name", 0, NULL, &proc_file_fops);

	struct dentry *cur;
	struct hlist_bl_node *tmp;
	
	int i, nb_dentry=0, max_size=0;
	
	hlist_bl_lock(dentry_hashtable);
	/*pour lock la liste, qu'il n'y ait pas de modif*/
	
	for (i = 0; i < (1 << d_hash_shift); i++) {
		int size = 0;
		
		/*parcours une ligne de notre dentry_hashtable*/
		hlist_bl_for_each_entry(cur, tmp, dentry_hashtable+i, d_hash){
			size++;
		}
		nb_dentry+=size;
		max_size = (max_size < size) ? size : max_size;
	}
	hlist_bl_unlock(dentry_hashtable);
	
	pr_info("L'adresse de la table dentry_hashtable : %p\n", dentry_hashtable);
	pr_info("Taille dentry_hashtable : %d\n", 1 << d_hash_shift);
	pr_info("Nombre de dentry : %d\n", nb_dentry);
	pr_info("Taille liste la plus longue : %d\n", max_size);
	
	return 0;
}

static void weasel_exit(void)
{
	pr_info("weasel module unloaded\n");
}

module_init(weasel_init);
module_exit(weasel_exit);

