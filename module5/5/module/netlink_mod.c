#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <net/net_namespace.h>
#include <net/sock.h>

#define NETLINK_UNIT 31
#define REPLY_MSG "hello from kernel"

static struct sock *nl_sock;

static void nl_recv(struct sk_buff *skb) {
  struct nlmsghdr *req_nlh = nlmsg_hdr(skb);
  struct sk_buff *reply;
  struct nlmsghdr *rep_nlh;
  size_t reply_len = strlen(REPLY_MSG);

  pr_info("netlink: received from pid %u: %s\n", req_nlh->nlmsg_pid,
          (char *)nlmsg_data(req_nlh));

  reply = nlmsg_new(reply_len, GFP_KERNEL);
  if (!reply) {
    pr_err("netlink: nlmsg_new failed\n");
    return;
  }

  rep_nlh = nlmsg_put(reply, 0, req_nlh->nlmsg_seq, NLMSG_DONE, reply_len, 0);
  NETLINK_CB(reply).dst_group = 0;
  memcpy(nlmsg_data(rep_nlh), REPLY_MSG, reply_len);

  if (nlmsg_unicast(nl_sock, reply, req_nlh->nlmsg_pid) < 0)
    pr_err("netlink: unicast send failed\n");
}

static struct netlink_kernel_cfg nl_cfg = {
    .input = nl_recv,
};

static int __init nl_init(void) {
  nl_sock = netlink_kernel_create(&init_net, NETLINK_UNIT, &nl_cfg);
  if (!nl_sock) {
    pr_err("netlink: failed to create socket\n");
    return -ENOMEM;
  }
  pr_info("netlink: socket created, proto=%d\n", NETLINK_UNIT);
  return 0;
}

static void __exit nl_exit(void) {
  netlink_kernel_release(nl_sock);
  pr_info("netlink: socket released\n");
}

module_init(nl_init);
module_exit(nl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dmitriy Sikachenko");
MODULE_DESCRIPTION("Bidirectional kernel-userspace communication via netlink");
