#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 4;
    int value = 0;

    mqd_t mfd;

    mfd = mq_open("/dev/mqueue", O_WRONLY, 0666, &attr);
    if(mfd == -1)
    {
        perror("open error");
        exit(0);
    }

    while(1)
    {
        if((mq_send(mfd, (char *)&value, attr.mq_msgsize, 1)) == -1)
        {
            perror("send error");
            exit(-1);
        }
        sleep(1);
        value ++;
    }
}
