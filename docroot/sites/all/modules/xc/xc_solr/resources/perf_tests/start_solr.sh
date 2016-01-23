DIR="$( cd "$( dirname "$0" )" && pwd )"
. ${DIR}/env.sh
prev_dir=$PWD

cd ${SOLR_DIR}
#rm ./logs/*
rm nohup.out 2> /dev/null
pwd
if [ $TEST_MODE -ne "1" ]
then
  echo starting solr
  nohup ./multicore.sh &
fi

cd $prev_dir
