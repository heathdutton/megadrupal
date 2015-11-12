Subversion Module READ ME

CONTENTS OF THIS FILE
---------------------

  * Introduction
  * Installation
  * Configuration
  * Usage
  * Known issues.


INTRODUCTION
------------
This module allows privileged users to commit changes in code to an SVN 
repository. It relies heavily on the existing SVN settings so a working copy is
required for this module to work.

This module is only meant to be installed on development environments where
users with little to no knowledge of the SVN CVS need to be able to commit
changes.


INSTALLATION
------------
Simple one click installation: At admin/build/modules enable the 
subversion module.


CONFIGURATION
-------------
1. Enable the permission "administer subversion" at admin/user/permissions.


USAGE
-----

Commiting to SVN
==================
1. Go to /admin/svn
2. If there are changes waiting to be committed, a text area and a button 
   labeled Commit Changes will show up on the page. Otherwise you will see a 
   message stating that there are no changes pending.
3. Write a commit message that explains what you changed and hit the Commit 
   Changes button.

Tagging a release
==================
1. Go to /admin/svn/tag
2. From the drop-down select one of the available tags.
3. Click the Tag Release button.

KNOWN ISSUES
------------
No known issues at the moment.
