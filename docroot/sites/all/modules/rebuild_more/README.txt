SUMMARY: This module only does something if one or more modules are providing node access control.  In such a case this module:

* Adds an elapsed time and estimate time to complete to the node access permissions rebuild process.
* Adds the ability to target nodes by type, nid, nid range for selective permissions rebuilding.
* Allows you to select when the node access table is purged (beginning or during).  Very helpful when you have many records and you don't want to block access to your users for the duration of the rebuild.



REQUIREMENTS:
* One or more enabled modules calling hook_node_access_grants


INSTALLATION:
* Download and unzip this module into your modules directory.
* Goto Administer > Site Building > Modules and enable this module.


CONFIGURATION:
* When visiting admin/reports/status/rebuild you will see additional options for rebuilding your permissions.


USAGE:
* Visit admin/reports/status/rebuild and choose your options.  The default options, mimick core's handling of the process.


--------------------------------------------------------
CONTACT:
In the Loft Studios
Aaron Klump - Web Developer
PO Box 29294 Bellingham, WA 98228-1294
aim: theloft101
skype: intheloftstudios

http://www.InTheLoftStudios.com
