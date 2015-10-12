// $Id$

CONTENTS OF THIS FILE
---------------------

  * Introduction
  * Installation
  * To-Do List

INTRODUCTION
------------

Current Maintainer: Marie Wendel <caligan@gmail.com>

Node Adoption allows you to automatically reassign nodes created by a deleted
user to another user of your choice. Additionally, a form is provided to
transfer ownership of all nodes from one user to another at any time. Node 
Adoption was originally maintained by Mark Dickson (ideaoforder) and sponsored 
by The Chicago Technology Cooperative. In Drupal 6.x, Node Adoption was 
maintained by Morbus Iff <morbus@disobey.com>.  As of Drupal 7.x, Node 
Adoption is maintained by Marie Wendel <caligan@gmail.com>.


INSTALLATION
------------

1. Copy the files to your sites/SITENAME/modules directory
   or, alternatively, to your sites/all/modules directory.

2. Enable the Node Adoption module at admin/build/modules.

3. You may define a default user to adopt all orphaned nodes at
   admin/settings/node_adoption or specifically change ownership
   from one user to another at admin/content/node_adoption.  (A
   Transfer Content tab is provided on the admin/content page.)

TO-DO LIST
----------

 * Allow the admin setting to be set to NULL, disabling all-0 changes.
 
 * Allow selecting Anonymous in admin/content/node_adoption.

 * Allow on-the-fly selection at user deletion instead of only a
   single configured user.

 * Alter structure to grab an array of the user's owned nodes instead of
   automatically changing everything owned by 0. (It must change ownership
   at some point before the user_delete hook is invoked; when?

 * Setting a nonexistant username produces a false (blank) positive rather
   than an appropriate error. (e.g., typing in 'Anonymous')

 * List transferred nodes subsequent to 'Node ownership has been
   transferred' message.

