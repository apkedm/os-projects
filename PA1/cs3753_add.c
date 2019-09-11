#include <linux/kernel.h>
#include <linux/linkage.h>

asmlinkage long sys_cs3753_add(int a, int b, int * sum)
{
	int c;
	printk("X = %d", x);
	printk("Y = %d", y);
	c = a + b;
	*sum = c;
	printk("Sum = %d", *sum);
	return 0;
}
