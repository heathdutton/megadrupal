#!/bin/bash

logdir=~/tmp-psr-test

mkdir -p $logdir
for site in $( find . -maxdepth 1 -mindepth 1 -type d  | sed 's,./,,' ) ; do
  if [[ "$site" == "all" || "$site" == "default" ]]; then
    continue
  fi
  diff -wup $logdir/$site.used-psr $logdir/$site.used-pml
  diff -wup $logdir/$site.not-used-psr $logdir/$site.not-used-pml
done
