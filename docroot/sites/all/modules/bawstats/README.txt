CONTENTS OF THIS FILE
---------------------
 
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Customising which statistics to display


INTRODUCTION
------------

The BAWStats module displays web statistics data extracted from web
server logs using AWStats (http://awstats.sourceforge.net/). It is
based on the third party betterAWStats PHP library
(http://betterawstats.com/), which now appears to be unmaintained.
The library had to be modified to work with Drupal and the original
author did not accept these modifications so that the package could
act as a library in Drupal.

Statistics data is collected by AWStats, stored in the normal AWStats
files and presented using the betterAWStats engine within
Drupal. BAWStats has the ability to display statistics information for
multiple websites. Data is displayed in a variety of graphs, tables,
and schematics.


REQUIREMENTS
------------

BAWStats (betterAWStats as a Drupal module) requires: 
* Drupal 6 
* the Libraries API module (http://www.drupal.org/project/libraries)
* a working AWStats system, with access to the AWStats data

However, AWStats does not have to be operating as a CGI.

Administration privileges within Drupal are required to install the
module. For viewing statistics from multiple sites (in addition to the
default installed site) the ability to edit the Drupal site's
settings.php file is required.


INSTALLATION
------------

0. Install AWStats. The packages is in the Debian and Ubuntu
   repositories and can be installed from the command line:
   
   apt-get install awstats

   Follow the instructions in /usr/share/doc/awstats/README.Debian.gz
   to configure AWStats.

   If not in Debian or Ubuntu download the package from
   http://awstats.sourceforge.net/ and follow the installation
   instructions there.

   If you are on a shared host, AWStats may already be
   installed. However, PHP might not be configured to read the AWStats
   library, language and data directories (see below). In this case,
   you may need to download the same version of AWStats used by your
   host, and put the "lib" and "lang" directories in a place that can be
   accessed by PHP.

1. Obtain and install the Libraries API module at
   http://drupal.org/project/libraries . For general installation
   instructions, see http://drupal.org/node/70151 for further information.

2. Install the BAWStats module as usual.

3. Install the AWStats icon directory

   In Debian and Ubuntu, the AWStats icon directory is
 
   /usr/share/awstats/icon/        
           
   In the top level of a default AWStats install, this is the
   directory wwwroot/icon, which contains operating system and country
   flag icons.  

   In either case, this directory must be copied to
   sites/all/libraries/bawstats in the Drupal tree, so that there is a
   directory sites/all/libraries/bawstats/icon. (This could also be a
   symlink to the AWStats tree.) NOTE: the "icon" directory is not the
   same as the BAWStats "icons" directory. The "icon" directory is
   needed as the icons are referenced as URLs within the Drupal tree.

CONFIGURATION
-------------

Before the BAWStats Drupal module can access the AWStats data, the
locations of the three key AWStats directories must be set on the
administration pages at admin/settings/bawstats.

* AWStats data directory: The directory containing the AWStats data. 

  In Debian/Ubuntu it is /var/lib/awstats

  Within the AWStats configuration it is the directory specified by
  the "DirData" parameter. The data must be accessible by PHP.

* AWStats library directory: The directory containing the AWStats
  library files.

  In Debian/Ubuntu it is /usr/share/awstats/lib

  In the top level of a default AWStats install, this is the directory
  wwwroot/cgi-bin/lib. Its contents must be accessible by PHP.

* AWStats language directory: The directory containg the AWStats
  language files.

  In Debian/Ubuntu it is /usr/share/awstats/lang

  In the top level of a default AWStats install, this is the directory
  wwwroot/cgi-bin/lang. Again it must be accessible by PHP.

Once configured correctly, betterAWStats statistics can be viewed in the
Drupal admin location admin/bawstats.


CUSTOMISING WHICH STATISTICS TO DISPLAY
---------------------------------------

AWStats can be configured to collect and store statistics for multiple
sites in its data directory.

For privacy reasons, the BAWStats module will present and display
statistics only for the domain name of the site it is running in Drupal
under. For example, if the BAWStats module is running on the Drupal
website http://www.example.com the module will look for AWStats data
files for "example.com" (AWStats drops the "www" by default).

If this is incorrect, for example, you wish the BAWStats module
running on site http://www.example.com to display some other AWStats
statistics (say, stats for another site, http://www.myexample.com),
the default can be overridden by editing the Drupal settings.php file
belonging to the www.example.com site. In the settings.php file, the
$conf['bawstats_defsite'] variable can be set to the site from which
to collect and display AWStats statistics. For example:

$conf['bawstats_defsite'] = 'myexample.com';

would configure the bawstats module to look for and display AWStats
data for the domain myexample.com.

The reason this is not made configurable with the other configuration
settings above (i.e. via the Drupal admin pages) is that it may be
deemed inappropriate that site administrators for one site can configure
BAWStats to look at statistics for other sites.

One of the nice features of betterAWStats is its ability to show
statistics from multiple sites. This is available in the Drupal
module, again, by editing the settings.php of the Drupal site for
which you want bawstats to have this ability. Adding the following to
a settings.php file will allow bawstats to view ALL statistics
available in the AWStats data directory:

$conf['bawstats_admin_access'] = 1;


OTHER CUSTOMISATION
-------------------

Other configurations to the display of data can be made by directly
editing the config.php file in the module directory.


NOTICE
-------

This module uses one non-Drupal CGI to render the world map of domain
locations. No data is revealed from this CGI (as it simply builds the
map image from the data it is given), but if this causes concern, it
can be disabled by removing access to the file
modules/render_map.inc.php
