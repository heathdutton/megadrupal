#!/bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

# Install Drupal
$DIR/install_drupal.sh

# Start up a server, wait for it to load up.
pushd $DIR/../../build/drupal
  drush runserver 127.0.0.1:8181 --browser=0 > /dev/null 2>&1 &
  sleep 4
popd
