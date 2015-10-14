#!/bin/bash
DIR="$( cd "$( dirname "$0" )" && pwd )"
. ${DIR}/env.sh

echo $LOG
if [ -z $TEST_MODE ]
then
  TEST_MODE=0
fi
if [ -z $LOG ]
then
  LOG=/dev/null
fi

echo "in reset_caches.sh"
echo "umount $MNT_PNT" >> $LOG

if [ $TEST_MODE -ne "1" ]
then
  if [ $BOUNCE_MYSQL -eq "1" ]
  then
    sudo service mysql stop
  fi
  umount $MNT_PNT
  mount $MNT_PNT
  sync && echo 1 > /proc/sys/vm/drop_caches
  # another method that Peter found
  # sync
  # /sbin/sysctl vm.drop_caches=1
  cat $BIG_FILE > ${SCRIPT_DIR}/delete_me
  rm ${SCRIPT_DIR}/delete_me

  if [ $BOUNCE_MYSQL -eq "1" ]
  then
    sudo service mysql start
  fi
fi


