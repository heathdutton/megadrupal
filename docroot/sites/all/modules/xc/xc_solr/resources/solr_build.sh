#!/bin/sh
#set -x
#
# ----------------------------------------------------------------------------
# Downloads and prepares all components needed for a custom Apache Solr which
# works together with eXtensible Catalog Drupal Toolkit
# ----------------------------------------------------------------------------
# This script is for the usage of maintainers of xc module.
# It produces a tag.gz file, which contains:
# - all Java library used by XC
# - starting/stopping scripts
# - custom Solr configuration files (schema.xml, solrconfig.xml)
# - logging.properties file for saving log messages
# - singe and multicore setup
# - full WordNet index
#
# The resulting file is
#  apache-solr-[Solr version]-xc-[XC Drupal Toolkit version].tar.gz
#  such as
#  apache-solr-3.6.1-xc-6.x-1.3.tar.gz
#
# Requirements:
# - wget or curl
# - Java compiler (JDK)
# - Java running environment (JRE)
# ----------------------------------------------------------------------------

#
# configuration section
#
DRUPAL_NAME=xc-6.x-1.3
SOLR_VERSION=3.6.1
SOLR_DIR=apache-solr-$SOLR_VERSION

#
# functions
#
download() {
  if [ "$HAS_WGET" = "1" ]; then
    wget -q -nc $1
  elif [ "$HAS_CURL" = "1" ]; then
    curl -O $1
  fi
}

download_requirement_check() {
  HAS_WGET=`command -v wget >/dev/null && echo "1" || echo "0"`
  HAS_CURL=`command -v curl >/dev/null && echo "1" || echo "0"`

  if [ "$HAS_WGET" = "0" -a "$HAS_CURL" = "0" ]; then
    echo "Please install either wget or curl"
    echo ""
    echo "Or download these files manually:"
    echo "http://archive.apache.org/dist/lucene/solr/$SOLR_VERSION/apache-solr-$SOLR_VERSION.tgz"
    exit;
  fi
}

requirement_check() {
  HAS_JAVA=`command -v java >/dev/null && echo "1" || echo "0"`

  if [ "$HAS_JAVA" = "0" ]; then
    echo "To run Solr Java is a must have. Please install it."
    exit;
  fi
}

download_all() {
  download_requirement_check
  if [ ! -e apache-solr-$SOLR_VERSION.tgz ]; then
    echo "Downloading Apache Solr"
    download http://archive.apache.org/dist/lucene/solr/$SOLR_VERSION/apache-solr-$SOLR_VERSION.tgz
  fi
  download http://drupalcode.org/project/xc.git/blob_plain/refs/heads/6.x-1.x:/xc_solr/resources/schema-for-$SOLR_VERSION.xml
  download http://drupalcode.org/project/xc.git/blob_plain/refs/heads/6.x-1.x:/xc_solr/resources/solrconfig-for-$SOLR_VERSION.xml
  download http://drupalcode.org/project/xc.git/blob_plain/refs/heads/6.x-1.x:/xc_solr/resources/multicore-solr-for-$SOLR_VERSION.xml
  download http://drupalcode.org/project/xc.git/blob_plain/refs/heads/6.x-1.x:/xc_solr/resources/solr.sh
  download http://drupalcode.org/project/xc.git/blob_plain/refs/heads/6.x-1.x:/xc_solr/resources/solr.start
  download http://drupalcode.org/project/xc.git/blob_plain/refs/heads/6.x-1.x:/xc_solr/resources/solr.stop
  download http://drupalcode.org/project/xc.git/blob_plain/refs/heads/6.x-1.x:/xc_solr/resources/multicore.sh
  download http://drupalcode.org/project/xc.git/blob_plain/refs/heads/6.x-1.x:/xc_solr/resources/multicore-start.sh
  download http://drupalcode.org/project/xc.git/blob_plain/refs/heads/6.x-1.x:/xc_solr/resources/multicore-stop.sh
  download http://drupalcode.org/project/xc.git/blob_plain/refs/heads/6.x-1.x:/xc_solr/resources/logging.properties
  download http://drupalcode.org/project/xc.git/blob_plain/refs/heads/6.x-1.x:/xc_wordnet/resources/WordnetSyns2Solr.java
  if [ ! -e WNprolog-3.0.tar.gz ]; then
    echo "Downloading WordNet"
    download http://wordnetcode.princeton.edu/3.0/WNprolog-3.0.tar.gz
  fi
}

#
# the script
#
rm -rf apache-solr-$SOLR_VERSION
rm -rf apache-solr-$SOLR_VERSION-$DRUPAL_NAME
rm -rf example
rm solr.s*
rm multicore*
rm logging.properties
rm WordnetSyns2Solr.java
rm WordnetSyns2Solr.class

requirement_check
download_all

ls -la

CWD=`pwd`
echo "Prepare Drupal"

#
# The Solr part
#
echo "The Solr part"
solr_instance=apache-solr-$SOLR_VERSION/example

echo '** extracting Solr...'
tar -zxf apache-solr-$SOLR_VERSION.tgz

conf_dir=$solr_instance/solr/conf

# save original config files
cp solr.* $solr_instance
cp multicore*.sh $solr_instance
cp logging.properties $solr_instance
if [ ! -e $conf_dir/solrconfig-orig.xml ]; then
  mv $conf_dir/solrconfig.xml $conf_dir/solrconfig-orig.xml
  mv $conf_dir/schema.xml $conf_dir/schema-orig.xml
fi

# copy Solr configuration file
if [ -e solrconfig-for-$SOLR_VERSION.xml ]; then
  cp solrconfig-for-$SOLR_VERSION.xml $conf_dir/solrconfig.xml
else
  cp solrconfig.xml $conf_dir
fi

# copy Solr schema file
if [ -e schema-for-$SOLR_VERSION.xml ]; then
  cp schema-for-$SOLR_VERSION.xml $conf_dir/schema.xml
else
  cp schema.xml $conf_dir
fi

cd $solr_instance
chmod +x solr.s*
chmod +x multicore*.sh
cd $CWD

# copy jar files
SOLR_DIR=apache-solr-$SOLR_VERSION
SOLR_LIB=$solr_instance/solr/lib
mkdir $SOLR_LIB
cp $SOLR_DIR/dist/apache-solr-analysis-extras-$SOLR_VERSION.jar $SOLR_LIB
cp $SOLR_DIR/contrib/analysis-extras/lib/icu4j-4.8.1.1.jar $SOLR_LIB
cp $SOLR_DIR/contrib/analysis-extras/lucene-libs/lucene-icu-$SOLR_VERSION.jar $SOLR_LIB

echo "*** prepare multicore setup"
cp $solr_instance/multicore/solr.xml $solr_instance/multicore/solr-orig.xml
cp multicore-solr-for-$SOLR_VERSION.xml $solr_instance/multicore/solr.xml

# rm -rf $solr_instance/multicore/core0
# rm -rf $solr_instance/multicore/core1
rm -rf $solr_instance/multicore/exampledocs
mkdir -p $solr_instance/multicore/xc/conf
cp $solr_instance/solr/conf/schema.xml $solr_instance/multicore/xc/conf
cp $solr_instance/solr/conf/solrconfig.xml $solr_instance/multicore/xc/conf
cp $solr_instance/solr/conf/synonyms.txt $solr_instance/multicore/xc/conf
cp $solr_instance/solr/conf/stopwords.txt $solr_instance/multicore/xc/conf
cp $solr_instance/solr/conf/protwords.txt $solr_instance/multicore/xc/conf
cp $solr_instance/solr/conf/elevate.xml $solr_instance/multicore/xc/conf
cp $solr_instance/solr/conf/currency.xml $solr_instance/multicore/xc/conf
cp -r $solr_instance/solr/conf/lang $solr_instance/multicore/xc/conf
cd $solr_instance/multicore/
cp -r xc wordnet
cd $CWD

echo "Creating $SOLR_DIR-$DRUPAL_NAME"
mv $SOLR_DIR/example .
cp -r example $SOLR_DIR-$DRUPAL_NAME
sleep 5

echo "Creating wordnet import file"
tar -zxf WNprolog-3.0.tar.gz
javac WordnetSyns2Solr.java
java WordnetSyns2Solr prolog wordnet.xml

cd $SOLR_DIR-$DRUPAL_NAME
./multicore-start.sh
echo "*** waiting for Solr..."
sleep 20

cd $CWD

url='http://localhost:8983/solr/wordnet/update'
echo "*** clear index..."
curl $url --data-binary '<delete><query>*:*</query></delete>' -H "Content-type: application/xml"
echo "*** index wordnet..."
curl $url --data-binary @wordnet.xml -H "Content-type: application/xml"
echo "*** commit..."
curl $url --data-binary '<commit/>' -H "Content-type: application/xml"
echo "*** optimize..."
curl $url --data-binary '<optimize/>' -H "Content-type: application/xml"

cd $SOLR_DIR-$DRUPAL_NAME
./multicore-stop.sh
sleep 5
cd $CWD

rm -f $SOLR_DIR-$DRUPAL_NAME/logs/*
rm -rf $SOLR_DIR-$DRUPAL_NAME/work

tar -cvzf $SOLR_DIR-$DRUPAL_NAME.tar.gz $SOLR_DIR-$DRUPAL_NAME
# tar -cvzf $SOLR_DIR-$DRUPAL_NAME.tar.gz apache-solr-3.6.1-xc-6.x-1.3

