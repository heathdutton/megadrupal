;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; URL Alias Permissions module
;;
;; Original author: jtphelan (http://drupal.org/user/54285)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CONTENTS OF THIS FILE
=====================
* OVERVIEW
* USAGE
* INSTALLATION


OVERVIEW
========

The URL Alias Permissions module allows site administrators to set permissions 
to create and edit url path settings by content type.

Have you ever wanted to allow your users to create or edit the alias for pages
and webforms but you want pathauto to create blog paths and you don't want your
users to change that path? Or perhaps you have slides content types and you 
don't needs paths for those at all, so you want to hide that from your users to 
make things simpler. This module allows you to do that.


USAGE
=====

Once installed permissions for each content type will also be available on the 
standard permissions page at Administer -> People -> Permissions.

  * Create and edit CONTENT TYPE URL alias




INSTALLATION
============

1) Copy all contents of this package to your modules directory preserving
   subdirectory structure.

2) Go to Administer -> Modules to install module. If the (Drupal core) Path
   module is not enabled, do so.

3) Permissions for each content type will also be available on the standard 
   permissions page at Administer -> People -> Permissions.
