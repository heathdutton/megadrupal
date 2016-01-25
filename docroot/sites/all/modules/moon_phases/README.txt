
Drupal Moon Phases module displays the phases of the moon in a block and a calendar and provides a page that
describes the phases of the moon. It is configured to show the moon image with a blue background during the
day and a black background during the night.

This module was created by Tom Camp <tom@campsitemedia.com>


Installation:
-------------
Extract the tarball to your modules directory, usually /sites/all/modules/
Enable the module at the modules page Administer -> Site Buildings -> Modules - /admin/build/modules/


Configuration:
--------------
In order to localize the sunrise/sunset aspect of the module go to Adminster -> Site Configuration -> Moon Phases
/admin/settings/moon - and enter a zip code. A longitude and latitude will be generated from the zip code using the 
National Oceanic and Atmospheric Administration WSDL - http://www.weather.gov/forecasts/xml/DWMLgen/wsdl/ndfdXML.wsdl.

Usage:
------
The module provides two pages and one block to display the data. The block shows the current phase of the moon,
the name of the phase and links to the moon phase page - /moon/phases.

The Moon Phase page shows the phase of the moon, describes what the phase is, shows the number of days until the
next full moon, the next new moon and the percentage of the moon which is illuminated. It also links to the previous
and next moon.

There is a tabbed navigation to this page and the moon phase caledar. The calendar shows a months worth of moon phases.
You can go forward or backwards by month.