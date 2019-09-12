#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/uaccess.h>

asmlinkage long sys_cs3753_add(int a, int b, int * sum)
{
	int c;
	printk(KERN_ALERT "A = %d\n", a);
	printk(KERN_ALERT "B = %d\n", b);
	c = a + b;
	copy_to_user(sum, &c, sizeof(int));
	printk(KERN_ALERT "Sum = %d\n", *sum);
	return 0;
}
