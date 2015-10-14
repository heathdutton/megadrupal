CONTENTS OF THIS FILE
---------------------

  * Overview
  * Features
  * Requirements
  * Known Issues
  * Credits
  * Similar Projects
  * Future/Todo

OVERVIEW
--------

Current Maintainer: typhonius

This module aims to bring originality to drupal comments. More often than not
the majority of comments on a website will be things like 'lol' or perhaps
just some copypasta.

With this module enabled each comment is checked against other comments to
see if what is commented has been said before. Think r9k for comments!


FEATURES
--------

The module has two modes:
Unique Comments per Node - Will ensure that each comment is unique on a per
node basis. No two comments will be the same per individual node.
Unique Comments Sitewide - Will ensure no two comments are the same on the
entire site.

It also had the ability to make certain nodes exempt from any restrictions
imposed on it by Unique Comments and an interface to manage which nodes are
exempt.

Permissions to exempt specified roles from Unique Comments

Ability to add or exempt certain content types from Unique Comments filtering.

Batch updating of nodes to either be affected by or exempt from Unique
Comments. This can be done on the admin/content page via hook_node_operations


REQUIREMENTS
------------

Currently there are no requirements to this module and it should work on a
standard Drupal 7 install.


KNOWN ISSUES
------------

Currently there are no known issues.


CREDITS
-------

Many thanks to all in the issue queue who helped improve this module and
make it ready for promotion to a full project.


SIMILAR PROJECTS
----------------

Unique Field


FUTURE
------
The module will eventually get a Drupal 6 version as well as Drupal 8 version.
Autocomplete on the form to exempt a node to show matching nodes
Views integration


CONFIGURATION
-------------

No special installation process with Unique Comments. Just enable the
module and then head over to /admin/config/content/unique_comments
to configure your settings. At that page the administrator or user with 
correct permissions will be able to set which mode they want Unique Comments
to operate in (site-wide, per node or not at all) as well as enter in any
nodes that shouldn't be subject to the module on 
/admin/config/content/unique_comments/add and view and delete these exemptions
on /admin/config/content/unique_comments/nodes. Users with the right
privileges can also exempt nodes from Unique Comments upon creation or update
on the node edit page.
Batch updating of nodes can be done from the /admin/content page with options
selected from the dropdown that take effect on ticked nodes.
