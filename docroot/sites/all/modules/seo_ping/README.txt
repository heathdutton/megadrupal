INTRODUCTION
------------
The SEO Ping module allows the user to use ping services like pingomatic.
It adds a configurable action so the user can choose what kind of nodes
and which parameters to send at the service. It uses XMLRPC client to
archive that.

 * For a full description of the module, visit the project page:
   https://drupal.org/project/seo_ping

 * To submit bug reports and feature suggestions, or to track changes:
   https://drupal.org/project/issues/seo_ping
   
REQUIREMENTS
------------
This module requires the following modules:
 * Trigger (core)
 * Token (https://www.drupal.org/project/token)
   
INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------
This module adds an action type. The user is able to add this action
and configured via Actions administration page (admin/config/system/actions/manage),
then "Create an advanced action" and select "Ping Services", after that
you must select:
  a) On which node types will be activated
  b) Ping service URL, the remote RPC server of the service
  c) Ping Port, the port of RPC Server (usually 80)
  d) The method, most times is weblogUpdates.ping, but some services allow extended
     values such as sitemaps etc so they use other methods, please consult the desired
     service for further instructions.
  e) Parameters, are the values of node to be sent, this text area allows you to use tokens
     in order to select values such as node title, url, author and other, also these values
     have to be separeted by line and be in format label=value. Enter only the allowed
     by your ping service values.For example pingomatic uses url and title, so you should write:
     title=[node:title]
     url=[node:url] 

After that the action is available to be attached to a trigger via trigger administration page
(admin/structure/trigger/node)
 
TROUBLESHOOTING
---------------

FAQ
---


MAINTAINERS
-----------
Current maintainers:
 * Georgios Tsotsos (tsotsos) - https://drupal.org/u/tsotsos
