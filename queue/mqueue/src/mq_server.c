#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 4;
    int value;

    mqd_t mfd;

    mfd = mq_open("/dev/mqueue", O_RDWR | O_CREAT,  0666, &attr);
    if (mfd == -1)
    {
            perror("open error");
            exit(0);
    }

    while(1)
    {
            if((mq_receive(mfd, (char *)&value, attr.mq_msgsize,NULL)) == -1)
            {
                    exit(-1);
            }
            printf("Read Data %d\n", value);
    }
}   
