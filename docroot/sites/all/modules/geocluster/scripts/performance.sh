#!/bin/bash
# declare an array called array and define 3 vales

debug() {
  echo $1
  echo $1 >> /tmp/drupal_debug.txt
}

test() {
  local arg=$1
  local n=$2
  local path=$3
  local items=$4
  local extra=$5

  file=$items"_items_"$5"_"$path"_"$n"_iterations"
  debug $file
  csv=$file".csv"
  log=$file".log"
  ab $arg -n $n -e $csv "https://geocluster-dasjo.dev1.epiqo.com/"$path >> $log
}

testPaths() {
  local arg=$1
  local n=$2
  local items=$3
  for path in "${paths[@]}"
  do
    if [ $items -gt 10000 -a "$path" == "php-json" ]
  	then
  	  debug "skip php for more than 10000 items"
  		continue
  	fi
    if [ $items -gt 10000 -a "$path" == "default-json" ]
  	then
  	  debug "skip default for more than 10000 items"
  		continue
  	fi

    drush cc all
    debug ""

    debug "Single run fresh"
    test "$arg" 1 $path $items "a_fresh"
    sleep 1

    debug "Single run again"
    test "$arg" 1 $path $items "b_again"
    sleep 1

    debug "Multiple run"
    test "$arg" $n $path $items "c_multi"
  done
}

cleanup() {
  debug "cleaning up"
  drush sql-drop -y
  drush sql-cli < ~/web/geocluster_blank.sql
  drush sapi-c
  rm *.csv *.log
}

base="https://geocluster-dasjo.dev1.epiqo.com/"
paths=( \
  'mysql-json' \
  'solr-json' \
  'php-json' \
  'default-json' \
)

cleanup

batch=$3
max_items=$4
items=0
while [ $items -lt $max_items ]; do
   let add=batch/2-items
   let items+=add
   echo Plus $add items to $items
   drush genc $add --types=article
   testPaths "$1" $2 $items

   let add=batch-items
   let items+=add
   echo Plus $add items to $items
   drush genc $add --types=article
   testPaths "$1" $2 $items

   let batch*=10
done

# print results
tail -n +1 -- *.log | grep "==>\|Path\|across all concurrent requests"
