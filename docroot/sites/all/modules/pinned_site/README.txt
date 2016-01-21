------------------------------------------------------------------------------
                            PINNED SITE MODULE
------------------------------------------------------------------------------
Users of Windows Internet Explorer can pin any website to the Windows taskbar 
just as they pin applications. When a website is pinned, it can be opened 
quickly and conveniently. A Pinned Site is more than just a shortcut, however.
Using the Pinned Site features, you can improve a user's ability to navigate, 
control, and interact with your site.

The Pinned Site module helps to integrate a site with Windows Desktop using 
Internet Explorer features fast, easy and without any knowledges in JavaScript 
and HTML.

Client requirements:
* Internet Explorer 9 or higher.
* Windows 7 or higher.

Note: Some features will only work on Windows 8 (Live Tiles),
      while others require Windows 7 or Windows 8 desktop mode (Static Tasks).

Detailed information about Pinned Sites technology you can read at 
"Introduction to Pinned Sites" [1].

This module gives to user several useful features:
* Declaring a static list of tasks for fast navigation to common destinations 
  within a site.
* Creating a dynamic list of recent site posts.
* Changing the application name, application tooltip, application start URL 
  and application window size.
* Making the browser look and feel like your site by changing the color of the 
  Back and Forward buttons.
* Creating a custom Windows 8 Live Tile for the startscreen.

Pinning a site to the taskbar is very simple: 
* Tear off a tab, and drag it to the taskbar OR
* Drag the favicon from the address bar in Internet Explorer to the taskbar.

The module uses JavaScript Pinned Site API and special HTML meta tags to make 
the site Pinned. Detailed information about Pinned Site API is located at 
"Pinned Sites Developer Documentation" [2].

Note: The module's JavaScript and meta tags only take effect in Internet 
      Explorer on Windows. Other browsers will ignore them and provide no 
      additional functionality.

Available interface translations:
* Russian (Русский)

This version of the module only works with Drupal 7.x.



INSTALLING
------------------------------------------------------------------------------
1. Backup your database.

2. Copy the complete 'pinned_site/' directory into the 'sites/all/modules/',
   'sites/default/modules' or 'sites/name_of_your_site/modules' folder of 
   your Drupal setup. 
   More information about installing and enabling contributed modules could be 
   found at "Installing contributed modules (Drupal 7)" 
   (http://drupal.org/documentation/install/modules-themes/modules-7)
   
3. Enable the "Pinned Site" module from the module administration page
   (Administration >> Modules).
   
4. Configure the module (see "CONFIGURATION" below).



UPDATING
------------------------------------------------------------------------------
1. Verify that the version you are going to upgrade contains all the features
   you are using in your Drupal setup. Some features could have been removed
   or replaced by others.

2. Read carefully in the project issue tracking about upgrade paths problems
   before you start the upgrade process. 

3. Backup your database.

4. Update current module code with latest recommended version. Previous 
   versions could have bugs already reported and fixed in the last version.

5. Complete the update process, set maintenance mode, call the update.php 
   script and finish the update operation.

6. Verify your module configuration and check that the features you are using
   work as expected. Also verify that all required modules are enabled, and
   permissions are set as desired.

Note: Whenever you have the chance, try an update in a local or development
      copy of your site.



CONFIGURATION
------------------------------------------------------------------------------
1. On the access control administration page ("Administration >> People 
   >> Permissions") you need to assign:

 * "administer pinned site settings" permission to the roles that are allowed 
   to administer the Pinned Site settings.

2. On the settings page ("Administration >> Configuration >> System
   >> Pinned Site") there are several tabs: Basic Settings, Start Tile, 
   Static Tasks and Recent Posts. To access any of these tabs users need the 
   "administer pinned site settings" permission.

3. On the Basic Settings tab you can specify some static application settings, 
   such as: Application Name, Application Tooltip, Application Start URI, etc. 

4. On the Start Tile tab you can specify some settings for the Live Tile on the 
   Windows startscreen, such as: Tile Color and Tile Image.
   
   Note: The client needs Windows 8 to use this feature.

5. On the Static Tasks tab you can define the list of tasks for the most 
   frequently used features of the website.
   Static Tasks are application-specific actions that are tailored to a website. 
   The tasks provide a set of static URIs that users can access at any time, 
   even if the browser instance is not running. 

   Note: Keep in mind you cannot add more than 5 static tasks.

6. On the Recent Post tab you can enable displaying recent posts in separate 
   category of the Jump List.

   Note: Maximum quantity of recent posts category items is 10.



BUGS AND SHORTCOMINGS
------------------------------------------------------------------------------
* See the list of project issues [3].



AUTHOR
------------------------------------------------------------------------------
Original author of this module is Konstantin Komelin [4].



MAINTAINERS
------------------------------------------------------------------------------
Current maintainers of this module are:
* Konstantin Komelin [4]
* Bastian Konetzny [5]



[1]    http://msdn.microsoft.com/en-us/library/gg491738%28v=VS.85%29.aspx
[2]    http://msdn.microsoft.com/en-us/library/gg491731%28v=VS.85%29.aspx
[3]    http://drupal.org/project/issues/pinned_site
[4]    http://drupal.org/user/1195752
[5]    http://drupal.org/user/367646
