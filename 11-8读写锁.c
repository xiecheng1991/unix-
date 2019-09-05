    1. 读模式下加锁
/*
    程序清单 11-8 使用读写锁
    读写锁 三种状态:
    1. 读模式下加锁
    2. 写模式下加锁
        在该模式下的锁被解锁之前,所有试图对这个锁加锁的线程都会被阻塞
    3. 不加锁
    读写锁也被称为共享-独占锁
    读共享:可以有多个线程加读锁 多个线程加读锁时会得到访问权限
    写独占:只能有一个线程加写锁 其他线程加写锁会被阻塞
    和互斥量一样使用之前必须初始化,在释放他们底层的内存前必须销毁

 */
#include"../my_apue.h"
#include<pthread.h>

struct job {
    struct job* j_next;
    struct job* j_prev;
    pthread_t   j_id;
};

struct queue {
    struct job      *q_head;
    struct job      *q_tail;
    pthread_rwlock_t q_lock;    
};

int
queue_init( struct queue *qp ) {
    int err;
    qp->q_head = NULL;
    qp->q_tail = NULL;
    err = pthread_rwlock_init( &qp->q_lock, NULL );
    if ( err != 0 ) {
        return err;
    }

    return 0;

}

void
job_insert( struct queue* qp, struct job* jp ) {
    pthread_rwlock_wrlock( &qp->q_lock );
    jp->j_next = qp->q_head;
    jp->j_prev = NULL;
    if ( qp->q_head != NULL ) {
        qp->q_head->j_prev = jp;
    } else {
        qp->q_tail = jp;
    }

    qp->q_head = jp;

    pthread_rwlock_unlock( &qp->q_lock );
}

void
job_append( struct queue *qp, struct job *jp ) {
    pthread_rwlock_wrlock( &qp->q_lock );
    jp->j_next = NULL;
    jp->j_prev = qp->q_tail;
    if ( qp->q_tail != NULL ) {
        qp->q_tail->j_next = jp;
    } else {
        qp->q_head = jp;
    }
    qp->q_tail = jp;
    pthread_rwlock_unlock( &qp->q_lock );
}


void
job_remove( struct queue* qp, struct job* jp ) {
    pthread_rwlock_wrlock( &qp->q_lock );
    if ( jp == qp->q_head ) {
        qp->q_head = jp->j_next;
        if ( qp->q_tail == jp ) {
            qp->q_tail = NULL;
        } else if ( jp == qp->q_tail ) {
            qp->q_tail = jp->j_prev;
            if ( qp->q_head == jp ) {
                qp->q_head = NULL;
            }
        } else {
            jp->j_prev->j_next = jp->j_next;
            jp->j_next->j_prev = jp->j_prev;
        }
    }
    pthread_rwlock_unlock( &qp->q_lock );
}

struct job*
job_find( struct queue* qp, pthread_t id ) {
    struct job* jp;
    if ( pthread_rwlock_rdlock( &qp->q_lock ) != 0 ) {
        return NULL;
    }

    for ( jp = qp->q_head; jp != NULL; jp = jp->j_next ) {
        if ( pthread_equal( jp->j_id, id ) ) {
            break;
        }
    }
    pthread_rwlock_unlock( &qp->q_lock );
    return jp;
}
