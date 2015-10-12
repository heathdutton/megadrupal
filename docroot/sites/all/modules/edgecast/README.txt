Module: EdgeCast
Author: Mike Carter <http://drupal.org/user/13164>


Description
===========
The Edgecast module connects your Drupal site to the CDN so that when you edit content the CDN knows to update it's cache quicker than the standard expiry time you've defined (usually hours or days).
You'll get support for Nodes, User profiles, comments and more thanks to the Expire module.


Requirements
============
* Edgecast account
* Expire Module (https://drupal.org/project/expire)


Installation
============
* Copy the 'edgecast' module directory in to your Drupal sites/all/modules directory as usual.

* Configure your Edgecast account details at /admin/config/development/edgecast/api
 - Customer ID - This can be found at the top right of every page at https://my.edgecast.com
 - Edgecast Token - This can be found under the 'Web Service REST API Token' area in 'My Settings' <https://my.edgecast.com/settings/>
 - Default Path - The fully qualified domain name of your Edge Cname


Usage
=====
* Upon updating individual nodes purge requests will be automatically sent to Edgecast.
* Manual purging can be performed at /admin/config/development/edgecast, wildcards can be used for example * to purge cache for the entire site.
