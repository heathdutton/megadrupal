#!/bin/bash
#[ "$#" -eq 1 ] || die "perf_test.sh solr|drupal"
if [ "$1" != "solr" ] &&  [ "$1" != "drupal" ]
then
  echo "usage: perf_test.sh solr|drupal"
  exit -1
fi

prev_dir=${PWD}
DIR="$( cd "$( dirname "$0" )" && pwd )"
. ${DIR}/env.sh

cd $SCRIPT_DIR
d=$(date +%Y-%m-%dT%H-%M-%S)
TEST_DIR=$TEST_OUTPUT_DIR/$d
export LOG=$TEST_DIR/script.log

rm -fR $TEST_DIR 2> /dev/null
mkdir -p $TEST_DIR/all_output

echo kill Solr
${SCRIPT_DIR}/kill_solr.sh
sleep 2
echo reset caches
sudo -E ${SCRIPT_DIR}/reset_caches.sh

echo copy keywords
cp ${SCRIPT_DIR}/keywords ${TEST_DIR}/raw_keywords
cat ${SCRIPT_DIR}/keywords | sed 's/://g' | sed 's/\?//g' | sed 's/ /%20/g' | sed "s/'/%39/g" | sed 's/"/%34/g' > ${TEST_DIR}/encoded_keywords

if [ $TEST_MODE -ne "1" ]
then
  ./start_solr.sh
  sleep 40
  echo solr started
fi

sudo /etc/init.d/apache2 restart

echo go testing
let i="0";
for k in $(cat ${TEST_DIR}/encoded_keywords)
do
  let j="0";
  export total="0.0";
  if [ "$1" == "drupal" ]
  then
    queries=$(cat ${SCRIPT_DIR}/drupal_queries | sed '/^#/d')
    base_url="${DRUPAL_URL}"
  elif [ "$1" == "solr" ]
  then
    queries=$(cat ${SCRIPT_DIR}/solr_queries | sed '/^#/d')
    base_url="${SOLR_URL}"
  fi
  for q in $queries
  do
    q2=$(echo -e $q | sed "s/__KEYWORD__/${k}/g")
    echo -e "issuing query for ${k}: ${base_url}${q2}" >> $LOG
    #if [ $TEST_MODE -ne "1" ]
    #then
      CURL_OUT=$(curl -w '\n%{time_total}' -sg "${base_url}${q2}" 2>> $LOG)
      #log2 "CURL_OUT: $CURL_OUT", $LOG
      CURL_TIME=$(echo -e "$CURL_OUT" | tail -n 1)
      #log2 "CURL_TIME: $CURL_TIME", $LOG
      #log2 "total-1: ${total}"
      total=$(echo "${total} + ${CURL_TIME}" | bc)
      #log2 "total-2: ${total}"
      if [ "$1" == "drupal" ]
      then
        echo -e "$CURL_OUT" > ${TEST_DIR}/all_output/q$i-$j-$k
      elif [ "$1" == "solr" ]
      then
        echo -e "$k:" >> ${TEST_DIR}/debug
        echo -e "$CURL_OUT" >> ${TEST_DIR}/debug
        JSON_OUTPUT=$(echo -e "$CURL_OUT" | head -n 1 | python -mjson.tool 2>> $LOG)
        echo -e "$JSON_OUTPUT" > ${TEST_DIR}/all_output/q$i-$j-$k
      fi
      echo -e "q$i-$j-$k $CURL_TIME" >> ${TEST_DIR}/results
    #fi
    let j="$j+1"
  done
  #log2 "total sum: ${total}"
  echo -e "total: ${total}" >> ${TEST_DIR}/results
  let i="$i+1"
done

if [ $KILL_SOLR_ON_COMPLETION -eq "1" ]
then

  ${SCRIPT_DIR}/kill_solr.sh

  if [ $TEST_MODE -ne "1" ]
  then
    cd ${SOLR_DIR}
    mv ./logs/* $TEST_DIR
    mv ./nohup.out $TEST_DIR
  fi

fi

cat ${TEST_DIR}/results | grep '^total' | sed 's/^total: //g' > ${TEST_DIR}/raw_timings
${SCRIPT_DIR}/sd.sh ${TEST_DIR}/raw_timings > ${TEST_DIR}/summary
echo "Top ten:" >> ${TEST_DIR}/summary
sort -rn ${TEST_DIR}/raw_timings | head -n 10  | gawk '{printf("%s", $0 ", ")}' >> ${TEST_DIR}/summary
echo "" >> ${TEST_DIR}/summary
echo "" >> ${TEST_DIR}/summary
echo "Bottom ten:" >> ${TEST_DIR}/summary
sort -n ${TEST_DIR}/raw_timings | head -n 10  | gawk '{printf("%s", $0 ", ")}' >> ${TEST_DIR}/summary
echo "" >> ${TEST_DIR}/summary
echo "" >> ${TEST_DIR}/summary
echo "select * from xc_search_ui" >> ${TEST_DIR}/summary
mysql -t -u root --password=${MYSQL_ROOT_PASSWORD} -D ${MYSQL_DATABASE_NAME} -e "select * from xc_search_ui" >> ${TEST_DIR}/summary
cd $DRUPAL_DIR/sites/all/modules/xc
echo "" >> ${TEST_DIR}/summary
echo "" >> ${TEST_DIR}/summary
git log | head -n 3 >> ${TEST_DIR}/summary

cat ${TEST_DIR}/summary

cd $prev_dir
