#!/usr/bin/env sh

# This script will run phpunit-based test classes using Drush's
# test framework.  First, the Drush executable is located, and
# then phpunit is invoked, passing in drush_testcase.inc as
# the bootstrap file.
#
# Any parameters that may be passed to phpunit may also be used
# with runtests.sh.

DRUSH_PATH="`which drush`"
DRUSH_DIRNAME="`dirname -- "$DRUSH_PATH"`"

RUNNER="$DRUSH_DIRNAME/vendor/phpunit/phpunit/phpunit.php"
if [ ! -f "$RUNNER" ] ; then
  RUNNER="`which phpunit`"
  if [ ! -f "$RUNNER" ] ; then
    echo "No phpunit found."
    echo "- To install, run 'composer install --dev' from Drush directory."
    exit 1
  fi
fi
echo "Using phpunit at $RUNNER"

if [ $# = 0 ] ; then
  $RUNNER --bootstrap="$DRUSH_DIRNAME/tests/drush_testcase.inc" .
else
  $RUNNER --bootstrap="$DRUSH_DIRNAME/tests/drush_testcase.inc" "$@"
fi
