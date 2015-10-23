Features Control Panel (version 1.x)
--------------------------------------
The Features Control Panel module enables you to: 
- Send users directly to an end-user control panel upon login
- Add controls (links and help text) to the control panel
- Export controls with the Features module 
- Reorganize and rename controls provided by other modules
- Customize control permissions that come from other modules

Installation 
---------------
Place the entire fcontrolpanel folder into your modules directory.
Go to Administer -> Structure -> Modules and enable the 
Features Control Panel module. 

Admin module integration
-------------------------
This module is intended to be used with the Admin module. 
With Admin installed, Features Control Panel will automatically
create Admin blocks to be displayed on the Admin Toolbar for all the 
feature sets (fieldgroups) on the Control Panel. This provides a consistent, 
familiar interface for authenticated users throughout the website. 
http://drupal.org/project/admin

Use
----
Go to Administration -> Structure -> Features Control Panel. 

Here you will see a list of controls on your website. (Controls can be entered
manually, or provided by feature modules.)

Click Add to add a new control. You will be prompted for the following 
information:
- name, machine readable name
- title, title text will be used for the link on your control panel
- feature set (package), organize controls into field groups
    The idea is to organize controls into feature sets that make sense 
    to end users (e.g. blog, press releases, events, etc.).
- description
- path, e.g. node/add/article
- query, query string to be appended to the URL path
- required permission, designate user permissions required to view this control
- weight

Features
-----------
To export controls go to Administer -> Structure -> Features.


Maintainer
-------------
Bryan Hirsch, bryan AT bryanhirsch DOT com
