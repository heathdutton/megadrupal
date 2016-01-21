
Introduction
============

Project status is an extension for drush which takes a list of drupal
platforms as arguments and returns a list of modules which are not in use by
any site within these platforms. It is also optionally able to list modules
which are use, by platform and site.

By default it only reports on modules within sites/all/ or sites/default/, but
it can optionally report on modules within site directories as well. Results
are grouped by project (for example, the modules views and views_ui are shown
together as views) but it is possible to list each module separately. At
present no distinction is made between different versions of the same module,
and core modules and modules provided by install profiles are ignored. Options
are provided to filter the report by site and module name.

The default output is similar to running the command

  drush pm-list --status="disabled,not installed" --type=module --no-core

for a single site to determine which modules are no longer in use and can be
removed from a site, except that all sites within all listed platforms will be
checked. This is useful for site administrators running many unrelated sites
using Drupal's multisite support, who wish to know which modules can be removed
from the sites/all/modules/ directory.

This command supports Drupal 5, 6, and 7.  The command will return an error if
multiple platforms are given which are not all the same major version of
Drupal.

Each command can be followed with the option --pipe for machine-readable
output.

How it works
============

This module uses Drupal's system table to determine which modules are installed
in a given site. With the option --rebuild the module will call the Drupal
function module_rebuild_cache() to rebuild this table for each site, which is
necessary if modules have been moved. (Running the drush command pm-list or
loading the page admin/build/modules will also rebuild this table.)

The option --rebuild actually calls "drush project-status-rebuild --pipe" for
each site. Without this option, Drush will call "drush sqlc" to dump the system
table for each site, which gives the same output.

Note: the option --rebuild is always used for Drupal 5, as detailed module
information is not cached in the system table.

Examples
========

List modules not enabled on any site in this platform:

  drush project-status /var/aegir/platforms/pressflow-6.20-prod

Same as above, but rebuild the system table for each site (to be used after
manually moving modules around):

  drush project-status /var/aegir/platforms/pressflow-6.20-prod --rebuild

List all modules, enabled or not, on mysite.com in the given platform:

  drush project-status /var/aegir/platforms/pressflow-6.20-prod --show-used --site-filter=mysite.com

List all modules enabled on these platforms, listed by platform and site:
(Note: --no-unused implies --show-used)

  drush project-status platforms/pressflow-6.20-prod platforms/pressflow-6.20-stage --no-unused --site-modules

Print a machine-readable list of modules not in use in the development,
staging, and production platforms:

  drush project-status /path/to/dev/platform /path/to/stage/platform /path/to/prod/platform --pipe

Determine which sites in a series of platforms are using the Views project
(includes the modules Views and Views UI):

  drush project-status /path/to/platform /path/to/other/platform  --filter=views --filter-strict --show-used --no-unused --site-modules

Determine which sites in a series of platforms are using any module whose names
includes the string "views" (here, Views and Views UI will be listed separately):

  drush project-status /path/to/platform /path/to/other/platform  --filter=views --show-used --no-unused --site-modules --no-group-projects

Known issues
============

 * Sites using database prefixes are not supported for now.
 * This script needs to execute drush, meaning that drush must be available in
   your current $PATH (see https://en.wikipedia.org/wiki/PATH_(variable) for
   more on this).
 * Unlike drush pm-list, this script will list modules with the same name as a
   site theme. This happens because module-theme naming conflicts are not
   supported in Drupal; see http://drupal.org/node/143020 for how to recover
   from this situation.

Author
======

Matt Corks (mvc)
