#!/bin/bash
# MegaDrupal
#
# Builds a drupal repository that contains all projects.
#
# Requirements:
#   Must be a module or theme.
#   Must be published.
#   Must have at least one download.
#   Must have a project type assigned.
#   Must be compatible with drush make.


BASEDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
repo="git@github.com:heathdutton/megadrupal.git"
repoDir="$BASEDIR/docroot"
tmpDir="$BASEDIR/tmp"

datestamp() {
  date +%Y-%m-%d
}

make(){

  version="$1"
  echo "Making MegaDrupal $1"

  echo "Building make file"
  php build-make-files.php $version

  echo "Running Drush Make process"
  rm -rf "$tmpDir"
  drush make "drupal-org-$version.make" "$tmpDir" --no-recursion --force-complete --concurrency 1 --prepare-install -y

  # echo "Cloning if needed"
  # rm -rf "$repoDir" >/dev/null 2>&1
  # git clone $repo "$repoDir" >/dev/null 2>&1

  # branch="$version.x"
  # echo "Checking out branch: ${branch}"
  # cd "$repoDir"

  # Use the following if this is a first time push
  # git branch $branch >/dev/null 2>&1
  # git checkout $branch
  # git pull

  echo "Cleaning out repo files to be replaced with those from make process"
  rm -rf "$repoDir"

  echo "Moving make files into repo"
  mv "$tmpDir" "$repoDir"

  echo "Copying make file into repo"
  cp "$BASEDIR/drupal-org-$version.make" "$repoDir/drupal-org-$version.make"

  # echo "Committing all changes."
  # git add .
  # git commit --all --message="$(datestamp)"

  # echo "Cleaning up Git."
  # git remote prune origin
  # git gc

  # echo "Pushing changes."
  # git push origin master:master --force
}

# make 6
make 7
# make 8
