CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * How To Use
 * Contributors
 * Credits


INTRODUCTION
------------

Provides soft-deletion/trash bin functionality for Drupal. Content (node/comments/taxonomies/etc) is "soft-deleted"
instead of deleted outright, and can be retained for a configurable span of time before being permanently purged. All
this is opaque to end-users as all soft-deleted content is pervasively hidden from them, but administrators can access
and restore soft-deleted content when necessary.

An index of recently soft-deleted content is also made available as an RSS feed at soft-deleted.rdf (this functionality
requires the RDF module), allowing content deletions to be propagated from one Drupal instance to another that
subscribes to its content (requires FeedAPI on the subscribing side in order to process the content deletions). This
particular functionality is intended for use cases such as multi-instance scenarios based on the publish/subscribe
architectural model.

Module is fully integrated with Rules, Views , Node UI.
The module also records the user whom soft-deleted the content and also deletion time.

This module requires PHP 5.2 or newer, and will integrate with the Views, FeedAPI, and RDF modules if installed.

This project is a fork of killfile module, and is currently being developed by Sina Salek <sina.salek.ws> .
Thanks to Kevin Day, Arto Bendiken, Dan Karran and Miglius Alaburda. and their sponsors OpenBand and MakaluMedia for
developing the original module.

 For more information on this project please refer to:
  <http://drupal.org/project/entity_soft_delete>

INSTALLATION
------------

 Download and Enable Cf module version 2 <https://drupal.org/project/cf>
 Simply add the module to /sites/all/modules/.

 Optional requirements:
   views module
   rules module

How To Use
------------
 Visit admin/config/content/entity_soft_delete and enable soft-delete for entity bundles you wish
 Visit admin/people/permissions and set Entity Soft Delete permissions
 Login as a non admin user Add a new content and try deleting it. Then login again as admin and check the
  content. You'll see that it's still there and you can undelete it if you wish.

 You can renamed delete/undelete form to whatever you want like cancel and uncancel.
 You can achieve this using drupal hooks

CREDITS
-------
 Current Maintainer: Sina Salek <sina.salek.ws>

CREDITS
-------
 Kevin Day <thekevinday@gmail.com>
 Arto Bendiken <http://bendiken.net/>
 MakaluMedia Group <http://www.makalumedia.com/>
 M.C. Dean, Inc. <http://www.mcdean.com/>
 SPAWAR <http://www.spawar.navy.mil/>