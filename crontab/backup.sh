#!/bin/bash

LOG_FILE="rsync_`date +'%y%m%d'`.log"

rsync -avz --progress --delete /home/k150030/working k150030@192.168.0.1:/home/k150030/backup > /home/k150030/log/backup/$LOG_FILE

