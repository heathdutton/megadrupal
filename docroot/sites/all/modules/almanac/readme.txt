; $Id: readme.txt,v 1.4 2011/02/18 09:58:03 nhwebworker Exp $

Readme
------
This modukle calculates sunrise/sunset/twilight times and moonrise/moonset time. It provides both page and block outputs. 

The individual user location is set via a settings page. The user's time zone, longitude, and latitude must be known in advance. There are many excellent mapping resources available on the web to assist with obtaining this information.

Send comments to Ed McCarren (nhwebworker) at http://drupal.org/user/23026

Requirements
------------
Drupal 7.x and compatible Date API module.

Update from 6.1
---------------
This version of the module doesn't use any module-specific database tables, and variable names for the settings have not 
changed from 6.x-1.x. No module-specific updates are required.

Installation
------------
1. Copy the Almanac directory and its contents to the appropriate directory, most likely /sites/all/modules/.

2. Enable the Almanac module in the modules admin page /admin/build/modules.

3. Set the location name, timezone, and latitude/longitude at admin/settings/almanac.

No database tables are created as part of the install. To uninstall simply disable the module.