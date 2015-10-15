#!/bin/bash

##
# @file
# Shorthand wrapper to perform arbitrary drush multisite commands.
#
# @param string
#   The command to pass to each Drupal site.
# @params string
#   (Optional) one or more specific site ID(s) to perform the command on.
#   If omitted, the command will run on all known sites.
#
# @todo
#   Include actual sites from sites.settings.inc.

command="$1"
shift

mywd=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

declare -a SITES
SITES=()

if [ "$1" != "" ]; then
	while ! [ "$1" = "" ]; do
		SITES+=("$1")
		shift
	done
else
	for site in $(php "${mywd}/drush_bulk.php"); do
		SITES+=($site)
	done
fi

for site in ${SITES[@]}; do
	echo "Now processing \"${command}\" for site \"$site\"..."
	drush "@${site}" $command
done
