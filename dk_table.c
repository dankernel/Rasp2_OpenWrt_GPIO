/*
 * =====================================================================================
 *
 *       Filename:  dk_table.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016년 02월 11일 16시 11분 03초
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <linux/slab.h>
#include "dk_table.h"

struct ikp_table ikp_table;

int init_ikp_talbe(void)
{
  INIT_LIST_HEAD(&ikp_table.list);
  return 0;
}

struct ipk_node *ikp_node_alloc(int sensor, int noti, int *f)
{
  struct ipk_node *tmp = NULL;

  tmp = kmalloc(sizeof(struct ikp_node*), GFP_KERNEL);
  printk("[IKP] init.. \n");
  return tmp;
}

struct ikp_node *ikp_node_add(int sensor, int noti, int *f)
{  
  struct ikp_node *tmp = NULL;

  if (sensor < 0 || noti < 0 || f == NULL)
    return NULL;

  tmp = (struct ipk_node*) ikp_node_alloc(sensor, noti, f);
  list_add_tail(&tmp->list_node, &ikp_table.list);
  printk("[IKP] Add %d %d %x node\n", sensor, noti, f);

  return tmp;
}

int lookup_noti(int noti)
{
  struct list_head *entry;
  int tmp_noti = 0;

  list_for_each(entry, &ikp_table.list) {
        tmp_noti = list_entry(entry, struct ikp_node, list_node);

        if (tmp_noti == noti)
          return tmp_noti;
  }

  return -1;
}



