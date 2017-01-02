#include <linux/module.h>
#include <linux/timer.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

#define DRIVER_AUTHOR "Simon Barotte <simon.barotte@gmail.com>"
#define DRIVER_DESC   "Light up LED if new mail is waiting"
/* Define output GPIO 24 */
#define RPI_GPIO_OUT 24


static ssize_t ex_read(struct file *filp, char *u_buffer, size_t max_lg, loff_t *offset)
{
	int lg;
	char k_msg[128];

	pr_debug("Try to read email_check");

	snprintf(k_msg, 128, "Value of GPIO : %d", gpio_get_value(RPI_GPIO_OUT));

	lg = strlen(k_msg) - (*offset);
	if (lg <= 0)
		return 0;

	if (lg > max_lg)
		lg = max_lg;

	if (copy_to_user(u_buffer, &k_msg[*offset], lg) != 0)
		return -EFAULT;

	*offset += lg;

	return lg;
}

static ssize_t ex_write(struct file *filp, const char *u_buffer, size_t lg, loff_t *offset)
{
	int value;
	int ret;
	char * k_msg;
	k_msg = kmalloc(lg, GFP_KERNEL);

	pr_debug("Try to write email_check");

	if (k_msg < 0)
		return -ENOMEM;

	if (copy_from_user(k_msg, u_buffer, lg) != 0){
		kfree(k_msg);
		return -EFAULT;
	}else{
		/* Convert char ton int for gpio_set_value */
	    ret = sscanf(k_msg, "%d", &value);
	    if (ret != 1) {
	    	kfree(k_msg);
	        pr_err("Error in reading value from user");
	        return -EINVAL;
	    }

	    /*  safety check */
        if (value != 0 && value != 1){
        	kfree(k_msg);
			return -EINVAL;
		}

		gpio_set_value(RPI_GPIO_OUT, value);
	}
	
	return lg;
}

static struct file_operations ex_fops = {
	.owner = THIS_MODULE,
	.read = ex_read,
	.write = ex_write,
};

static struct miscdevice ex_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = THIS_MODULE->name,
	.fops = &ex_fops,
};

static int __init email_check_init(void)
{
	int err;

	pr_debug("Init email_check");

	if ((err = gpio_request(RPI_GPIO_OUT, THIS_MODULE->name)) != 0){
    	return err;
	}

	if ((err = gpio_direction_output(RPI_GPIO_OUT, 0)) != 0){
		gpio_free(RPI_GPIO_OUT);
    	return err;
	}

	err = misc_register(&ex_misc);

	if (err){
		pr_err("Enable to register misc device : email_check");
		return err;
	}

	return 0;
}

static void __exit email_check_exit(void)
{
	pr_debug("Exit email_check");
	misc_deregister(&ex_misc);
  	gpio_free(RPI_GPIO_OUT);
}

module_init(email_check_init);
module_exit(email_check_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
