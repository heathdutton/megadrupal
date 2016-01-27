#!/bin/bash

LOG_FILE=drupal_test.cahce.distinct.log
RUN_DIR=`pwd`
SOLR_DIR=/home/kiru/download/xc-1.0rc-test/apache-solr-3.3.0/example/

cd $SOLR_DIR
rm $LOG_FILE

# an alternative way to change vm.drop_cache variable
#/sbin/sysctl vm.drop_caches=1

for step in {1..10}
do
  cd $SOLR_DIR
  echo Starting Solr \#$step
  sh solr.sh start 2>>$LOG_FILE &
  # wait till
  echo Waiting a minute to warm Solr
  sleep 40
  for term in dummy charles bowen david randall john benjamin peter
  do
    wget -O /dev/null -nv http://127.0.0.1/test1/xc/search/$term
  done

  for term in manifestation Juvenile History Image Sound
  do
    wget -O /dev/null -nv http://127.0.0.1/test1/xc/search/$term
  done

  for term in Text Electronic books Early works Periodicals Biography Congresses Scores
  do
    wget -O /dev/null -nv http://127.0.0.1/test1/xc/search/$term
  done

  for term in English German French Latin Italian Spanish Russian Japanese Dutch
  do
    wget -O /dev/null -nv http://127.0.0.1/test1/xc/search/$term
  done

  for term in Proceedings Annual report Selected poems Bulletin Bibliography
  do
    wget -O /dev/null -nv http://127.0.0.1/test1/xc/search/$term
  done

  for term in United States General Accounting Office Geological Bach Johann Sebastian Mozart Wolfgang Amadeus
  do
    wget -O /dev/null -nv http://127.0.0.1/test1/xc/search/$term
  done

  for term in Beethoven Ludwig van Congress Senate Committee Energy android Natural Resources House Rules
  do
    wget -O /dev/null -nv http://127.0.0.1/test1/xc/search/$term
  done

  for term in Foreign Relations Brahms Johannes Politics government criticism Law Bible interpretation
   do
    wget -O /dev/null -nv http://127.0.0.1/test1/xc/search/$term
  done

  # rochester MovingImage Survey on legislation

  sh solr.sh stop
  sleep 20
  cd $RUN_DIR
done
