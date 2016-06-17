#include <linux/unistd.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>

asmlinkage int sys_add(int a, int b, int *res) {
	int err=0, ret=0;
	err = access_ok(VERIFY_WRITE, res, sizeof(int));
	if(err == -EFAULT) return err;

	ret = a+b;
	put_user(ret, res);

	printk(KERN_INFO "%d+%d = %d from Linux Kernel!!\n", a, b, *res);

	return err;
}
