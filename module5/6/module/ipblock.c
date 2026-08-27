#include <linux/inet.h>
#include <linux/init.h>
#include <linux/ip.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>

#define PROC_DIR_NAME "ipblock"
#define PROC_LIST_NAME "blacklist"
#define MAX_ENTRIES 64
#define PROC_INPUT_MAX 32

static __be32 blacklist[MAX_ENTRIES];
static int blacklist_len;
static DEFINE_SPINLOCK(blacklist_lock);

static struct proc_dir_entry *proc_dir;
static struct nf_hook_ops ipblock_ops;

static bool blacklist_contains(__be32 addr) {
  int i;

  for (i = 0; i < blacklist_len; i++)
    if (blacklist[i] == addr)
      return true;
  return false;
}

static unsigned int nf_drop_hook(void *priv, struct sk_buff *skb,
                                 const struct nf_hook_state *state) {
  struct iphdr *iph = ip_hdr(skb);
  bool drop;

  spin_lock_bh(&blacklist_lock);
  drop = blacklist_contains(iph->daddr);
  spin_unlock_bh(&blacklist_lock);

  if (drop) {
    pr_info("ipblock: dropped packet to %pI4\n", &iph->daddr);
    return NF_DROP;
  }
  return NF_ACCEPT;
}

static int bl_seq_show(struct seq_file *m, void *v) {
  int i;

  spin_lock_bh(&blacklist_lock);
  for (i = 0; i < blacklist_len; i++)
    seq_printf(m, "%pI4\n", &blacklist[i]);
  spin_unlock_bh(&blacklist_lock);
  return 0;
}

static int bl_proc_open(struct inode *inode, struct file *file) {
  return single_open(file, bl_seq_show, NULL);
}

static ssize_t bl_proc_write(struct file *file, const char __user *buf,
                             size_t len, loff_t *offset) {
  char input[PROC_INPUT_MAX];
  char op;
  __be32 addr;

  if (len >= PROC_INPUT_MAX)
    return -EINVAL;
  if (copy_from_user(input, buf, len))
    return -EFAULT;

  input[len] = '\0';
  if (input[len - 1] == '\n')
    input[len - 1] = '\0';

  op = input[0];
  if (op != '+' && op != '-')
    return -EINVAL;

  addr = in_aton(input + 1);
  if (!addr)
    return -EINVAL;

  spin_lock_bh(&blacklist_lock);

  if (op == '+') {
    int i;
    bool exists = false;

    for (i = 0; i < blacklist_len; i++)
      if (blacklist[i] == addr) {
        exists = true;
        break;
      }
    if (!exists && blacklist_len < MAX_ENTRIES)
      blacklist[blacklist_len++] = addr;
  } else {
    int i;

    for (i = 0; i < blacklist_len; i++) {
      if (blacklist[i] == addr) {
        blacklist[i] = blacklist[--blacklist_len];
        break;
      }
    }
  }

  spin_unlock_bh(&blacklist_lock);
  return len;
}

static const struct proc_ops bl_proc_ops = {
    .proc_open = bl_proc_open,
    .proc_read = seq_read,
    .proc_write = bl_proc_write,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init ipblock_init(void) {
  proc_dir = proc_mkdir(PROC_DIR_NAME, NULL);
  if (!proc_dir)
    return -ENOMEM;

  if (!proc_create(PROC_LIST_NAME, 0660, proc_dir, &bl_proc_ops)) {
    remove_proc_entry(PROC_DIR_NAME, NULL);
    return -ENOMEM;
  }

  ipblock_ops.hook = nf_drop_hook;
  ipblock_ops.pf = NFPROTO_IPV4;
  ipblock_ops.hooknum = NF_INET_LOCAL_OUT;
  ipblock_ops.priority = NF_IP_PRI_FIRST;

  if (nf_register_net_hook(&init_net, &ipblock_ops) < 0) {
    remove_proc_entry(PROC_LIST_NAME, proc_dir);
    remove_proc_entry(PROC_DIR_NAME, NULL);
    return -EINVAL;
  }

  pr_info("ipblock: loaded, manage via /proc/%s/%s\n", PROC_DIR_NAME,
          PROC_LIST_NAME);
  return 0;
}

static void __exit ipblock_exit(void) {
  nf_unregister_net_hook(&init_net, &ipblock_ops);
  remove_proc_entry(PROC_LIST_NAME, proc_dir);
  remove_proc_entry(PROC_DIR_NAME, NULL);
  pr_info("ipblock: unloaded\n");
}

module_init(ipblock_init);
module_exit(ipblock_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dmitriy Sikachenko");
MODULE_DESCRIPTION("Outgoing IP packet filter with /proc blacklist management");
