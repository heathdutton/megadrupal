
DESCRIPTION
-----------

Similar entries provides a default Views block for displaying
similar published nodes to the currently viewed one, based on
the title and body fields.

This module is for MySQL-based Drupal sites. It uses MySQL's
FULLTEXT indexing for MyISAM tables, and should automatically add
the index when you activate the module for the first time on a
Drupal site version 4.7.x or newer.

NOTE: If your node_revisions table is InnoDB, this module's install
file will convert your table to MyISAM. The FULLTEXT indexing
feature of MyISAM is not available for InnoDB (yet).

INSTALLATION
------------

Copy the similar directory to your modules directory.
(sites/all/modules/similar)

Activate the module in administer >> modules.
Turn on the similar block in administer >> structure >> blocks.

WORKING WITH VIEWS
------------------
To customize the Similar entries block, navigate to administer >>
structure >> views and edit the view named similar_entries.
The similar entries view uses a custom argument. This node ID
argument must be present in the view to be able to perform
FULLTEXT searches on node bodies and CCK fields. Searching and
comparing against CCK fields is optional in the argument. Similar
entries indexes CCK field tables when cron runs. If you set up
a new CCK field on a content type you can force Similar entries
to index it by navigating to administer >> status report and
clicking on 'run cron manually'.


BUG REPORTING
-------------

http://drupal.org/project/issues/similar


CONTRIBUTORS
------------
David Kent Norman http://deekayen.net/

Arnab Nandi http://arnab.org/

Jordan Halterman
