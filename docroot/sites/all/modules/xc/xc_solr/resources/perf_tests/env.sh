#!/bin/bash
#export SCRIPT_DIR="/home/banderson/scripts/perf_tests"
export SCRIPT_DIR="/home/pkiraly/perf_tests"
#export TEST_OUTPUT_DIR="/home/banderson/test_results"
export TEST_OUTPUT_DIR="/home/pkiraly/perf_tests/test_results"
#export SOLR_DIR="/mnt/apache-solr-3.3.0/example"
#export SOLR_DIR="/xc/dt/xc/apache-solr-3.3.0/example"
export SOLR_DIR="/xc/apache-solr-3.3.0/example"
export SOLR_URL="http://localhost:8984/solr/"
#export DRUPAL_URL="http://localhost/xc/"
export DRUPAL_URL="http://localhost/dtmilestone3/"
#export DRUPAL_DIR="/var/www/xc"
export DRUPAL_DIR="/var/www/dtmilestone3"

export KILL_SOLR_ON_COMPLETION="0"
export TEST_MODE="0"
export BOUNCE_MYSQL="1"

export BIG_FILE="${SCRIPT_DIR}/file_2_clear_cache"
#export MNT_PNT="/xc"
export MNT_PNT="/mnt"
#export MYSQL_ROOT_PASSWORD="root"
#export MYSQL_DATABASE_NAME="xc_drupal"
export MYSQL_ROOT_PASSWORD="extensible"
export MYSQL_DATABASE_NAME="dtmilestone3"

if [ -z $LOG ]
then
  export LOG="/dev/null"
fi

function log2 {
  echo -e "$1" | tee -a $2 | xargs -0 echo -e
}
