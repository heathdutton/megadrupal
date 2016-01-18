#!/bin/bash

logdir=~/tmp-psr-test
drush=/usr/bin/drush
pwd=$(dirname $0)

mkdir -p $logdir
for site in $( find . -maxdepth 1 -mindepth 1 -type d  | sed 's,./,,' ) ; do
  if [[ "$site" == "all" || "$site" == "default" ]]; then
    continue
  fi
  echo 1>&2 $site
  $drush psr . --site-filter=$site --site-filter-strict --no-group-projects --site-modules --show-used --no-unused --pipe | sort > $logdir/$site.used-psr
  $drush -l $site pml --no-core --status="enabled" --type=module --pipe | sort > $logdir/$site.used-pml
  $drush psr . --site-filter=$site --site-filter-strict --no-group-projects --site-modules --pipe | sort > $logdir/$site.not-used-psr
  $drush -l $site pml --no-core --status="disabled,not installed" --type=module --pipe | sort > $logdir/$site.not-used-pml
done

$pwd/test-evaluate-psr.sh
