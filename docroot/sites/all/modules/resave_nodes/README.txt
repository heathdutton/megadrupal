CONTENTS OF THIS FILE
---------------------
* Introduction
* Requirements
* Installation
* Configuration
* Permissions
* Maintainers


Introduction
------------
The Resave Nodes module allows for the automatic resaving of nodes.  The
resaving of nodes can be handled automatically through Drupal's cron
functionality or it can be managed with a Unix-style cron configuration. The
nodes that are resaved can be either the ones that have been created or changed
since the last time the module ran (via one of the two cron options) or it can
be all nodes regardless of created/change date.  It can also be restricted to
nodes of selected content types.

Why do this?  Consider the issue discussed here:

https://www.drupal.org/node/2001446

In certain situations, importing nodes with geolocation information doesn't
always cause the geolookup to occur.  Resaving the node by hand triggers
that geolookup.  This by itself is fine if there aren't that many nodes and
the importing occurs manually.  But when hundreds (or more) nodes are
imported automatically on a recurring schedule, manually resaving
individual nodes or using the Views Bulk Operations module to manually bulk
save nodes is not a realistic option.  Enter this module.

Upon installation, configure the desired content types to monitor and
whether or not to do only those nodes that have been recently
created/changed.  Then, select the preferred cron option (via Drupal or using
the Job Scheduler module).  The next time cron is run, nodes of that content
type will be resaved, forcing the computing of that special field.


Requirements
------------
The Job Scheduler module is required.
https://www.drupal.org/project/job_scheduler

NOTE: this is a new requirement if you are upgrading from a prior version of
Resave Nodes.

Installation
------------
Install as you would normally install a contributed drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information.


Configuration
-------------
There are three main configuration options accessible under
'Configuration > System > Resave Nodes' or by going to
/admin/config/system/resave_nodes:

* Content type(s) to limit which nodes are resaved.
* Whether to go with the default "resave only created/changed nodes" or to
  resave all nodes regardless of created/changed time.
* Schedule the resaving of nodes via Drupal's cron utility or with the
  Unix-style crontab available from the Job Scheduler.


Permissions
-----------
If someone other than user 1 needs to configure which content types to
monitor, set the "Administer the periodic resaving of nodes via cron"
permission for that user's role.


MAINTAINERS
-----------
Current maintainers:
* Adam Fuller (dasfuller) - https://drupal.org/user/2731951
