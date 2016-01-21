
Hosting Diagnostics

This module provides some statistics and other data helpful when sizing a
hosting environment.

Installation
------------

Installation is the same as most other Drupal modules - unpack the archive file
to your modules directory, then enable it at
<http://YOURSITE?q=admin/build/modules>.

Usage
-----
There are only three administrative options:
1. Whether logging of page builds is enabled or disabled. 
2. How many logging records to retain.
3. The email address to send reports to.

You may change these settings at
<http://yoursite.com/admin/config/hosting_stats>

Once hosting is enabled, every page view (whether caching is enabled or not)
will be logged to a database table. Be aware that depending on your level of
traffic, this table may become quite large.

Only two data points are logged, to minimize the size of the table and protect
user privacy: 
1. usertype: a boolean value denoting whether the user requesting the page 
build is authenticated to Drupal or not.
2. timestamp: the unix timestamp when the request is made.

Once you've captured what you believe to be a representative sample of your
traffic, you can process the logs into a report, and also capture information
about your site's mysql database and PHP environment. Note that the environment
information is less detailed than what is available via the System Info module
<http://drupal.org/project/systeminfo>.

You may request each portion of the report separately, as some processes may
take longer than others, depending on the size of your database and filesystem.

The Hosting Statistics report is viewable at
<http://yoursite.com/admin/reports/hosting_stats>.

Drush
-----
Hosting Diagnostics also allows most operations to be performed from Drush
<http://drupal.org/project/drush>.

The following commands are available:
hd-enable
hd-disable
hd-calculate
hd-clearlog
hd-stats

Use "drush help <command>" to view help for each Drush command.

Author
------
Chris Yates
chris.yates@acquia.com
chris@christianyates.com
<http://acquia.com>
<http://christianyates.com>

D7 Upgrade
Cameron Tod
cameron.tod@acquia.com
