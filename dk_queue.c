/*
 * =====================================================================================
 *
 *       Filename:  dk_queue.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016년 02월 12일 16시 22분 57초
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#define IKP_QUEUE_SIZE 1024

struct queue_node {
  int sensor;
};

struct queue_node ikp_queue[IKP_QUEUE_SIZE];
int wirte_index;
int read_index;

int ikp_queue_wirte(int sensor)
{
  if (wirte_index == read_index)
    return -1;
  
  ikp_queue[wirte_index++].sensor = sensor;
  if (IKP_QUEUE_SIZE < wirte_index)
    wirte_index = 0;

  return 0;
}

int ikp_queue_read(void)
{
  int ret = -2;

  if (wirte_index == read_index)
    return -1;
  
  ret = ikp_queue[read_index++].sensor;

  if (IKP_QUEUE_SIZE < read_index)
    read_index = 0;

  return ret;
}
