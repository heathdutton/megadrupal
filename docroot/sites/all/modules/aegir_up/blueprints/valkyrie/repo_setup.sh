#! /bin/bash

# Make aegir user's shell bash, so my .bashrc settings apply
chsh aegir -s /bin/bash

# Update provision repo
cd /var/aegir/.drush/provision
## Replace read-only origin with read-write
git remote set-url origin ergonlogic@git.drupal.org:project/provision.git

# Update hostmaster repo
cd /var/aegir/hostmaster-6.x-1.x/profiles/hostmaster
## Replace read-only origin with read-write
git remote set-url origin ergonlogic@git.drupal.org:project/hostmaster.git
## Remove crud added by Drush
git checkout */*.info
git checkout *.info
chown aegir:aegir . -R

# Update valkyrie repo
cd /var/aegir/hostmaster-6.x-1.x/sites/all/modules/valkyrie
## Replace read-only origin with read-write
git remote set-url origin ergonlogic@git.drupal.org:project/valkyrie.git
## Remove crud added by Drush
git checkout */*.info
git checkout *.info
chown aegir:aegir . -R

# Update devshop_hosting repo
cd /var/aegir/hostmaster-6.x-1.x/sites/all/modules/devshop_hosting
## Add my sandbox repo as a remote
git remote add sandbox ergonlogic@git.drupal.org:sandbox/ergonlogic/1901278.git
git fetch sandbox
## Remove crud added by Drush
git checkout */*.info
git checkout *.info
chown aegir:aegir . -R
## The checkout reverted the patches from our makefile, so let's apply it again
wget http://drupal.org/files/devshop_hosting-allow_task_overrides-1915854-2.patch
git apply devshop_hosting-allow_task_overrides-1915854-2.patch
wget http://drupal.org/files/devshop_hosting-add_diffs-1530542-4_0.patch
git apply devshop_hosting-add_diffs-1530542-4_0.patch
chown aegir:aegir . -R

# Update devshop_provision repo
cd /var/aegir/.drush/devshop_provision
## Add my sandbox repo as a remote
git remote add sandbox ergonlogic@git.drupal.org:sandbox/ergonlogic/1901286.git
git fetch sandbox
## Apply patch(es)
wget http://drupal.org/files/devshop_provision-alternate_git_commit_locations_with_cleanup-1915854-1.patch
git apply devshop_provision-alternate_git_commit_locations_with_cleanup-1915854-1.patch
get http://drupal.org/files/devshop_provision-repo_paths_for_pull_tasks-1915854-3.patch
git apply devshop_provision-repo_paths_for_pull_tasks-1915854-3.patch
chown aegir:aegir . -R
