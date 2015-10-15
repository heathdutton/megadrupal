********************************************************************
                      D R U P A L    M O D U L E
********************************************************************
Name: Airtime Widgets for Drupal
Author: Andre Langner (medienverbinder)
Contact: http://drupal.org/user/1293712
Drupal: 7
RELEASE STATUS: This module is at an initial release state.
********************************************************************

INTRODUCTION
------------

The first version of this functionality should receive 'Now Playing'
information from Sourceforge Airtime Radio Station, a free open source
radio automation software.

The following description I have taken from the airtime manual written
by the company Sourcefabric. (airtime manual, page 142 and following)
(see ressources)

There are two kinds of information that can be retrieved remotely from
Airtime; the metadata for the current show plus the following show
(live-info), or the schedule for the current week (week-info). This
metadata includes show names, times, descriptions and individual show
URLs on your public website. That way, the audience for your station
can click through from the schedule information to find out more about
a particular show, or download a previous show recording that you might
have made available.

If your Airtime server was accessible at:

=> http://air1.example.com


the live show information could be retrieved by your web server using
this URL:

=> http://air1.example.com/api/live-info/?callback


The information for the current week's schedule could be retrieved using
the URL:

=> http://air1.example.com/api/week-info/?callback


If you see the message You are not allowed to access this resource when
attempting to display schedule information in your web browser, log in
to the Airtime administration interface, click System in the main menu,
then Preferences. Set Allow Remote Websites To Access "Schedule" Info?
to Enabled, click the Submit button, then refresh the browser window
opened on the schedule export URL.


INSTALLATION
------------

Libraries API 2

Download and install the Libraries API 2 module.
Move the downloaded module package to sites/all/modules/.
 => sites/all/modules/libraries

In administration back-end on the module configuration (admin/modules)
page you have now to install/activate the module and save the module
configuration.


jQuery Update

Download and install the jQuery Update module.
Move the downloaded module package to sites/all/modules/.
  => sites/all/modules/jquery_update

In administration back-end on the module configuration (admin/modules)
page you have now to install/activate the module and save the module
configuration.

Notice:
The Sourcefabric Airtime Widgets using jQuery UI Tabs version 1.8.10.
For this reason the jQuery version 1.8 must be enabled after installation
of jQuery update. Otherwise, the Weekly Programm can not be displayed.
  => /admin/config/development/jquery_update


Sourcefabric Airtime Widgets

Download the original Airtime Widgets from Sourcefabric [3].
Make sure to use Airtime Widgets for AIRTIME_API_VERSION 1.1.

Move the plugin, css files and image folder to the directory
"airtimewidgets" and place it inside 
 => "sites/all/libraries/airtimewidgets"

Make sure the path to the plugin file becomes
 => "sites/all/libraries/airtimewidgets/js/jquery.showinfo.js"

and

 => "sites/all/libraries/airtimewidgets/css/airtime-widgets.css"

and

 => "sites/all/libraries/airtimewidgets/css/widget-img/"

Notice:
The Drupal status report provides information on the correct installation
of the Airtime Widgets plugin. (/admin/reports/status)


Drupal Airtime Widgets Module

Download and install the Airtime Widgets module.
Move the downloaded module package to:
 => sites/all/modules/airtimewidgets

or

 => sites/all/modules/custom/airtimewidgets
(yeah(!) its custom ;-) )

In administration back-end on the module configuration (admin/modules)
page you have now to install/activate the module and save the module
configuration.


Configuration

After installation you should check the configuration page for the
SimpleTagCanvas module and customize the settings.
(/admin/config/content/airtimewidgets/)


Activating and placing the block

After activation of the module four new blocks added to the block configuration
page under structure / blocks. Now move the new block to the desired region and
make some block specific configuration.


RESOURCES
---------

[1] Ideas on Airtime-Drupal Interoperability
http://wiki.sourcefabric.org/display/CC/Ideas+on+Airtime-Drupal+Interoperability

[2] Sourcefabric Airtime on GitHub
https://github.com/sourcefabric/Airtime

[3] Sourcefabric Airtime Widgets on GitHub
https://github.com/sourcefabric/Airtime/tree/master/widgets

[4] Manual Airtime 2 for Broadcasters
http://files.sourcefabric.org/manuals/Airtime/en/Airtime_2.1.3-A4-Download.pdf

[5] Sourcfabric WIKI
https://wiki.sourcefabric.org/display/CC/Frontend+Widgets

[6] German short description
http://andrelangner.de/drupal/project/airtimewidgets


CONTACT
-------

Current maintainer:
Andre Langner (medienverbinder) - http://drupal.org/user/1293712
