Name: Uptime (uptime)
Author: Martin Postma ('lolandese', http://drupal.org/user/210402)
Drupal: 7.x


-- SUMMARY --

A block showing the uptime ratio of the site (e.g. 99,85%).

It uses the service from UptimeRobot.com:
"Monitors your websites every 5 minutes, totally free.
Get alerts by e-mail, SMS, Twitter, RSS or push notifications for iPhone/iPad."

Two versions are available:
- A visualy appealing widget, by default enabled in the footer.
- A simple text version, by default disabled.


-- INSTALL --

Extract the package in your modules directory, '/sites/all/modules'.

Enable the module at '/admin/modules'.


-- CONFIGURE --
Configuration at 
'/admin/structure/block/manage/uptime/uptime_widget/configure' 
(widget) or at 
'/admin/structure/block/manage/uptime/uptime_text/configure' 
(text).

To use only a simple text, disable the 'Uptime ratio widget' block and enable 
'Uptime ratio text only'.


-- CUSTOMIZE --

To change the content in the widget (e.g. to put the ratio first):
1. Copy the uptime.tpl.php file to your theme's template folder.
2. Make your changes.
3. Clear the site cache at '/admin/config/development/performance'.

To change the style of the widget (e.g. colors):
1. Copy & paste the code in the uptime.css into your theme's custom CSS file.
2. Make your changes.
3. Clear both your browser and site cache.

Alternatively you can install and enable the helper module
https://www.drupal.org/project/style_settings. This will make many CSS
attributes configurable through the settings page (UI).

To override the string "Uptime" use 
http://drupal.org/project/stringoverrides which provides an easy way to replace 
any text on your site that's wrapped in the t() function.


-- TROUBLESHOOTING --

Many issues can be resolved by clearing the site's cache at
'admin/config/development/performance'.
