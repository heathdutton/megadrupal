#!/bin/bash
#
# This script should not be run directly.
# Instead you need to source it from your .bashrc,
# by adding this line:
#   . ~/.drush/drush-vagrant/lib/scripts/dvroot.sh
#
# or, if drush-vagrant installed elsewhere on your system:
#   . /<path to drush-vagrant>/lib/scripts/dvroot.sh
#

function dvroot() {
  alias=$1
  # We specify '-n' here to avoid getting stuck at a prompt, should a group
  # alias be passed.
  path=`drush -n ${alias} vagrant-root`
  # Non-existant aliases return null, so ensure we have a path
  if [ "$path" ]
  then
    cd $path
  fi
}
