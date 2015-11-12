#!/bin/bash
red='\033[00;31m'
green='\033[00;32m'
blue='\033[00;34m'

report_result() {
  if (( $? > 0 )); then
    echo -e "${red}ERROR"
    exit 1
  else
    echo -e "${green}OK"
  fi;
  tput sgr0
}

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
TMP_DIR="$DIR/tmp"
VALKYRIE_PROJECT_ALIAS_PATH=$DIR
VALKYRIE_DRUSHRC_PATH="$TMP_DIR/.drushrc.php"
VALKYRIE_DRUSHRC_INCLUDE_PATH="$TMP_DIR/.valkyrie.drushrc.php"
export VALKYRIE_PROJECT_ALIAS_PATH
export VALKYRIE_DRUSHRC_PATH
export VALKYRIE_DRUSHRC_INCLUDE_PATH

mkdir -p $TMP_DIR

echo 'Bringing up VM. This may take a few minutes.'
vagrant up > /dev/null

echo -e "\n ${blue}** BEGINNING TESTS **\n"; tput sgr0

echo 'TEST 1: Checking that test alias is available in Drush directly.'
drush sa --alias-path=$VALKYRIE_PROJECT_ALIAS_PATH @valkyrie_test_alias | grep "'valkyrie_test' => 'test1234'" > /dev/null ; report_result

echo 'TEST 2: Checking that test option is available in Drush directly.'
drush --alias-path=$VALKYRIE_PROJECT_ALIAS_PATH @valkyrie_test_alias eval "drush_print_r(drush_get_option('valkyrie_test', FALSE));" | grep "test1234" > /dev/null; report_result

echo 'TEST 3: Checking that test alias is available in Drush via our includes.'
drush sa @valkyrie_test_alias --config=$VALKYRIE_DRUSHRC_PATH | grep "'valkyrie_test' => 'test1234'" > /dev/null ; report_result

echo 'TEST 4: Checking that test option is available in Drush via our includes.'
drush @valkyrie_test_alias --config=$VALKYRIE_DRUSHRC_PATH eval "drush_print_r(drush_get_option('valkyrie_test', FALSE));" | grep "test1234" > /dev/null; report_result

echo 'TEST 5: Checking that test alias is no longer available after suspending VM.'
vagrant suspend > /dev/null
drush sa @valkyrie_test_alias --config=$VALKYRIE_DRUSHRC_PATH 2>&1 | grep "Not found: @valkyrie_test_alias" > /dev/null; report_result

echo 'TEST 6: Checking that test option is available again after resuming VM.'
#vagrant resume > /dev/null
vagrant up > /dev/null
drush @valkyrie_test_alias --config=$VALKYRIE_DRUSHRC_PATH eval "drush_print_r(drush_get_option('valkyrie_test', FALSE));" | grep "test1234" > /dev/null; report_result

echo 'TEST 7: Checking that test alias is no longer available after halting VM.'
vagrant halt > /dev/null
drush sa @valkyrie_test_alias --config=$VALKYRIE_DRUSHRC_PATH 2>&1 | grep "Not found: @valkyrie_test_alias" > /dev/null; report_result

echo 'TEST 8: Checking that test option is available again after bringing the VM back up.'
vagrant up > /dev/null
drush @valkyrie_test_alias --config=$VALKYRIE_DRUSHRC_PATH eval "drush_print_r(drush_get_option('valkyrie_test', FALSE));" | grep "test1234" > /dev/null; report_result

echo 'TEST 9: Checking that test alias is no longer available after destroying VM.'
vagrant destroy -f > /dev/null
drush sa @valkyrie_test_alias --config=$VALKYRIE_DRUSHRC_PATH 2>&1 | grep "Not found: @valkyrie_test_alias" > /dev/null; report_result

echo -e "\n ${green}** ALL TESTS PASSED **\n"; tput sgr0

echo 'Cleaning up temporary files and environmental variables.'
unset VALKYRIE_PROJECT_ALIAS_PATH
unset VALKYRIE_DRUSHRC_PATH
unset VALKYRIE_DRUSHRC_INCLUDE_PATH
rm -r $TMP_DIR
