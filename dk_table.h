/*
 * =====================================================================================
 *
 *       Filename:  dk_table.h
 *
 *    Description:  (IKP)In Kernel Path table
 *
 *        Version:  1.0
 *        Created:  2016년 02월 11일 15시 27분 51초
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Junhyung Park (dkdkernel@gmail.com)
 *   Organization:  Dankook Univ.
 *
 * =====================================================================================
 */

#include <linux/list.h>


struct ikp_table {
  struct list_head list;
};

struct ikp_node {
  int num;
  int sensor;
  int noti;
  int callback;
  struct list_head list_node;
};



