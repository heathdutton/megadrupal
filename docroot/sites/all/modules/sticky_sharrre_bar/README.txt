Sticky Sharrre Bar - module for Drupal based on
http://sharrre.com, http://imakewebthings.com/jquery-waypoints.
Provides sticky block for social network sharing.

Also this module uses icon pack http://sensationalfix.com/flat-social-icons-eps/.
The file icons.png is derived from work that is Copyright Jorge Calvo
(http://sensationalfix.com/).
Source: http://sensationalfix.com/flat-social-icons-eps/.
Used here under the Creative Commons Attribution-ShareAlike (CC BY-SA) license.

SUPPORTED PROVIDERS
-------------------
- Google Plus
- Facebook
- Twitter
- Digg
- Delicious
- StumbleUpon
- Linkedin
- Pinterest

Author: Ruslan Piskarev <http://drupal.org/user/424444>.

REQUIREMENTS
----------
- Libraries API module.
  Download link: https://drupal.org/project/libraries.
- jQuery Update module.
  Download link: https://drupal.org/project/jquery_update.
- jQuery Waypoints - library v2.0.5 or higher.
  Download link: http://imakewebthings.com/jquery-waypoints.
- jQuery Sharrre  - library v1.3.5 or higher.
  Download link http://sharrre.com.

INSTALLING
----------
- Download and unzip the "Waypoints" library
  to "/sites/all/libraries/jquery-waypoints" directory.
  Files structure:
    /sites/all/libraries/jquery-waypoints/waypoints.min.js
    ...
- Download and unzip the "Sharrre" library
  to "/sites/all/libraries/sharrre" directory.
  Files structure:
    /sites/all/libraries/sharrre/jquery.sharrre.min.js
    ...
  Warning! For security reasons you should delete the sharrre.php file
  from the "Sharrre" library folder.
- Go to "Home > Administration > Modules"
  and enable "Sticky Sharrre Bar" module.
- Go to "Home > Administration > Structure > Blocks > Sticky Sharrre Bar"
  and set the necessary providers, regions, etc.
  By default, the block is installed in the "header" region.
  If your theme does not have this region,
    you can choose the necessary region manually.
  You can unset "Use the css of module" and make your own style in your theme.

POSSIBLE TROUBLES
-----------------
- To get the counts from "Google Plus" and "StumbleUpon" module uses cURL.
  Therefore, these providers will not work
    if the "libcurl" is not enabled on your server.
  You can disable these providers on the block configuration page.

MORE INFORMATION
----------------
- To issue any bug reports, feature or support requests, see the module issue
  queue at https://drupal.org/project/issues/2268241.
