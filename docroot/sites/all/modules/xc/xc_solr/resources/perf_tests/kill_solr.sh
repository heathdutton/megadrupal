#!/bin/bash
DIR="$( cd "$( dirname "$0" )" && pwd )"
. ${DIR}/env.sh

echo "killing solr" | tee -a $LOG | xargs echo
if [ $TEST_MODE -ne "1" ]
then
  ps -ef | grep 'java.*solr' | gawk '{print $2}' | xargs kill -9 2> /dev/null
  sleep 3
fi
