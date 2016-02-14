/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016년 01월 07일 11시 27분 02초
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  JunHyung Park
 *   Organization:  Dankook Univ.
 *
 * =====================================================================================
 */

#include <linux/module.h>   /*  Needed by all modules */
#include <linux/kernel.h>   /*  Needed for KERN_INFO */

#include <linux/init.h>
#include <linux/completion.h>
#include <linux/kthread.h>

#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/time.h>

#include <linux/jiffies.h>

/* IRQ */
#include <linux/interrupt.h>

/* WorkQueue */
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/slab.h>

/* .. */
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>

#include "dk_table.c"
#include "dk_queue.c"

#define GPIO_PIN4 4
#define GPIO_PIN17 17

#define GPIO_INPUT 0x1
#define GPIO_OUTPUT 0x2

MODULE_LICENSE("GPL");

void interupt_work_func(struct work_struct *data);
DECLARE_DELAYED_WORK(interupt_work, interupt_work_func);
static struct workqueue_struct *dk_workqueue;
int irq_num = 0;

/*
 * Init GPIO
 * @GPIO_NUM : GPIO number
 * @GPIO_IO : input or output
 * return : x
 */
void GPIO_direction_init(const int GPIO_NUM, const int GPIO_IO)
{/*{{{*/
  if (GPIO_NUM < 0 || 64 < GPIO_NUM) {
    printk("[Fail] GPIO number");
    goto fail;
  }

  if (GPIO_IO < 0 || 3 < GPIO_IO) {
    printk("[Fail] GPIO_IO");
    goto fail;
  }

  /* Init */
  if (GPIO_IO == GPIO_INPUT) {
    /* InPut */
    if (gpio_direction_input(GPIO_NUM)) {
      printk("[Fail] Set input\n");
      goto fail;
    } else
      printk("[O  K] Set input\n");

  } else if (GPIO_IO == GPIO_OUTPUT) {
    /* OutPut */
    if ((gpio_direction_output(GPIO_NUM, 0)) < 0) {
      printk("[Fail] Set output\n");
      goto fail;
    } else
      printk("[O  K] Set output\n");
  }

  printk("[%s : OK] num : %d, io : %d <<<<< \n", __func__, GPIO_NUM, GPIO_IO);
  return;

fail:
  printk("[%s : OK] num : %d, io : %d <<<<< \n", __func__, GPIO_NUM, GPIO_IO);

}/*}}}*/

/*
 * GPIO control
 * @GPIO_NUM : GPIO number
 * @GPIO_IO : input or output
 * @val : value
 * return : x
 */
void GPIO_control(const int GPIO_NUM, const int GPIO_IO, int *val)
{/*{{{*/

  /* Init */
  if (GPIO_IO == GPIO_INPUT) {
    *val = gpio_get_value(GPIO_NUM);
  } else if (GPIO_IO == GPIO_OUTPUT) {
    gpio_set_value(GPIO_NUM, *val);
  }
  return;

}/*}}}*/

void interupt_work_func(struct work_struct *data)                                        
{/*{{{*/
  int val = 0;

  GPIO_control(4, GPIO_OUTPUT, &val);

}/*}}}*/

/* 
 * Innerrupt
 */
void interrupt_function(void)
{/*{{{*/
  int val = 0;
  int one = 1;

  printk("interupt!!! %d\n", val);

  GPIO_direction_init(4, GPIO_OUTPUT);
  GPIO_control(4, GPIO_OUTPUT, &one);

  /* queue_work(dk_workqueue, &interupt_work); */
  queue_delayed_work(dk_workqueue, &interupt_work, 50);

  /* End.. */
  gpio_free(4);
  return;
}/*}}}*/

/*  
 * Init
 */
int init_module(void)
{/*{{{*/
  int ret = 0;

  printk(KERN_INFO "[kmod] Start\n");

  // Init workqueue
  dk_workqueue = create_workqueue("dk_wq");
  printk("create_workqueue\n");

  // Set IRQ GPIO17
  irq_num = gpio_to_irq(GPIO_PIN17);
  printk("[sys_dk_world] GPIO4 IRQ : %d\n", irq_num);

  // Request IRQ
  ret = request_irq(irq_num, (irq_handler_t)interrupt_function, IRQF_NOBALANCING , "IRQ_dk", NULL);

  printk("[kmod] request_irq : %d\n", ret);
  return 0;
}/*}}}*/

/*
 * Cleanup
 */
void cleanup_module(void)
{/*{{{*/
  printk(KERN_INFO "[kmod] Clean start \n");

  /* ReInit */
  gpio_request(GPIO_PIN4, "");
  gpio_request(GPIO_PIN17, "");
  printk(KERN_INFO "[kmod] Re-Init \n");

  /* Free */
  gpio_free(GPIO_PIN4);
  gpio_free(GPIO_PIN17);
  free_irq(irq_num, NULL);

  /* cancel_delayed_work(&interupt_work); */
  flush_workqueue(dk_workqueue);
  destroy_workqueue(dk_workqueue);

  printk(KERN_INFO "[kmod] End\n");
}/*}}}*/

