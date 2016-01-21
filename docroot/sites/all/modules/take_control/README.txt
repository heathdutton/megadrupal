
-- SUMMARY --

The Drupal Take Control module allows you to take complete control of your Drupal file-system.

As of version 2.0, the module has moved on to a core/add on modules architecture,
where the core simply provides some common configuration, and methods, whereas the bulk of the functionality comes
from various add-on modules.

Currently, there are 2 add-on modules:
1) File Browser: A complete File Browser for Drupal. Allows you to manage your Drupal file-system directly
  through Drupal without needing FTP or your Hosting Control Panel access.
  The immediate vision is to make it as functional as the popular CPanel Hosting Panel's built-in file manager.
  
2) Quick Permissions: Many Drupal administrators are troubled by the: Drupal_Security_Do_Not_Remove_See_SA_2006_006
directive in Drupal file-system root's .htaccess file that effectively prevents write access to the file-system folder outside Drupal.
This module allows you to take control of the file-system folder (usually /sites/default/files)
by specifying custom permissions on it from Drupal.
This enables manipulation of your Drupal files folder outside the Drupal using FTP or your Hosting Control Panel.

Additionally, you can specify custom permissions / delete any directory/file owned by Drupal using this module.
This was the original version 1.0 of the take_control module, that has been moved to the Quick Permissions add-on module.

Please notice that the File Browser add-on module has been specifically created for managing the Drupal files folder
from Drupal itself, without needing to change permissions on it.

However if you still need to do so, please restore back the permissions of the Drupal files folder after your work is over,
or it could be a security concern.


Effectively, the take_control module together with its add-ons allows you to take complete control of your Drupal file-system folder,
from within as well as from outside Drupal.

Please take care to manipulate only those directories/files through these modules that are owned by Drupal (i.e. created by Drupal itself),
or to which Drupal's Web Server account has write permissions.
Otherwise, the actions might not take effect (depending upon permissions) and might log a entry in your Site Log.
The File Browser module shows the UserId of the account under which Drupal runs, and Owner UserId's of all filse/folders.
  
Once the File Browser module stabilizes, I have plans of adding more file-system related features. If you find
something lacking, please file a feature request. However, please dont file requests for the File Browser add-on
saying CPanel File Manager also supports this or that. With time, I have plans to add all CPanel File Manager
features that can be securely supported.

I think I should mention this that I am very clear regarding the target users of this module: Drupal administrators.
Although there can be arguments, that File Browser can be enhanced to provide restricted file system access to
appropriate users based on permissions, this scenario is NOT targeted in the near future. Currently, the add-on
modules are implemented on an all-or-none basis, meaning either their functionality is totally available to a role,
or not at all available.

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/take_control


-- REQUIREMENTS --

1) ExtJs Javascript library
  File Browser add-on module is currently based on ExtJs 3.1.0 (although it should work on immediately preceding
  or succeeding ExtJs releases).
  Download the copy of ExtJs from:
  http://www.extjs.com/products/extjs/download.php
  
  Extract it to: sites/all/libraries/extjs/version
  
  e.g. the current (as on Feb 16, 2010) version of ExtJs should be extracted to:
  sites/all/libraries/extjs/3.1.0
  
  Please preserve the internal file organization of ExtJs files. However, you can safely delete docs and examples
  from the package.
  
  Please note that this module would work with ExtJs 3.x but not with ExtJs 4.x. ExtJs 3.x can be downloaded from any of these pages:
  http://www.sencha.com/products/extjs3/download/
  http://www.sencha.com/learn/Ext_Version_Archives
  
2) PHP Zip Extension
  File Browser add-on module allows extraction and creation of .zip archive files, and is dependent on this
  extension. Please verify that Zip extension is available and enabled, in your portal's phpinfo() output.

  You can view phpinfo() for your portal at:
  http://domain.com/admin/reports/status/php


-- INSTALLATION --

  There is a video for installation instructions also available for this module at my website:
  http://www.rahulsingla.com/projects/drupal-take-control-module/installation

* Please read and follow the instructions below carefully.

1) Download and extract ExtJs Javascript library to /sites/all/libraries/extjs folder as described above.

2) Extract the module zip to your /sites/all/modules folder (or to your particular site folder).

3) Enable the module(s) as usual from your Drupal admin Modules section at:
  domain.com/admin/build/modules
  
  You should see 3 modules:
  i)    Take Control
  ii)   Quick Permissions
  iii)  File Browser

  Take Control should be installed, and enabled to make any other add-on module to work. However, atleast one
  add-on module should also be enabled to access the functionality.

4) Visit the module's admin section at: /admin/settings/take_control, and change settings appropriately.

5) Optional:
  The File Browser add-on module can be easily made to use icons of your choice. It comes with a default set.
  Some pre-defined icon-sets for File Browser together with instructions to use them are available at:
  http://www.rahulsingla.com/projects/drupal-take-control-module/icon-sets


-- CONFIGURATION --

* Configure administration options in Administer >> Site Configuration >> Take Control >>


* Configure user permissions in Administer >> User management >> Permissions >>
  take_control module:

  - access take control
  - administer take control
  
  file browser module:
  - access file browser
  - administer file browser
  
  quick permissions module:
  - access quick permissions

  You should allow only trusted user roles to access these modules.
  They would normally constitute the support staff for your portal.
  
* After installation, please ensure that the "Take Control File Browser" block is enabled by visiting your site's Block administration page at:
  domain.com/admin/structure/block


-- USING THE MODULE --

* You can access the main and add-on modules at:
  domain.com/admin/config/content/take_control


-- FAQ --

Q: The Quick Permissions module does not allow me to set permission less than 700 on any file/directory. Why?

A: To prevent the owner of the file from losing access to the file.


Q: Why does File Browser shows Owner UserIds for files and not Usernames?

A: PHP's POSIX extension is required to be installed and enabled on the server for accessing information about
  files, e.g. their owner Usernames. And many third-party Hosting providers do not provide this extension.
  I might in a future version check if POSIX is available, and show Usernames if it is, and UserIds if it's not.


Q: How does the File Browser calculates Current User Id?

A: File Browser displays the Owner of Drupal file-system folder as the Current User Id. This has the built-in
  assumption that this folder was created from within Drupal itself. Therefore, this information might be incorrect
  if you created the folder from outside Drupal (through your Hosting Panel or FTP).
  A future version of the module might use POSIX extension to display this information currently on installations
  which have POSIX available.


Q: Why are multiple blocks for File Browser available on Block administration page?

A: Starting with 7.x-2.3 version, the module switched to using blocks for rendering the File Browser instead of a page.
  The advantage being it allows site administrators to precisely control the positioning and visibility of the block on their site.
  By default, only a single block is available which can be accessed at:
  domain.com/admin/config/content/take_control
  You can however increase the number of blocks available on the module's administration page and then add the blocks on various pages
  at different positions and/or visibility.
  Please note that multiple blocks are just available as a convenience, all blocks function exactly the same way and permissions cannot
  be enforced on a per-block basis.
  Further you should only drop one block belonging to this module on a Drupal page, multiple instances of this module's blocks on a page are not supported
  (although they might work fine but the scenario has not been tested nor is officially supported).
  

-- CONTACT --

Current maintainers:
* Rahul Singla (r_honey) - http://drupal.org/user/473356

This module was originally created for my personal website: http://www.rahulsingla.com

The module's permanent address on my website is:
http://www.rahulsingla.com/projects/drupal-take-control-module

For paid customizations of the project, contact me through my personal website above.

This module is currently sponsored by Imbibe Technologies Pvt Ltd.:
http://imbibe.in

