README.txt
==========

The Simple Weather project provides a module that when installed creates a block
that displays current weather conditions for a location. The location for the
current weather report is defined by the user who has permission to administer
blocks by submitting the block configuration form for the Simple Weather block.

There are two fields that can determine the location of the weather report:
US ZIP code field and the Where On Earth Identification (WOEID) field. The data
in these fields are passed to the simpleWeather jQuery plugin that queries the
data from Yahoo's Weather service and returns it for display in the Simple
Weather block. There is also a field that sets the temperature scale as Celsius
or Fahrenheit and returns the data accordingly.

The simpleWeather jQuery Plugin is made available as free software using an MIT
License. It is not distributed with this module and is authored and maintained
by James Fleeting. Visit http://simpleweatherjs.com for more information.

INSTALLATION INSTRUCTIONS
=========================

Install this module using standard Drupal contributed module installation
procedure. Then complete the following steps:

1. Download the required simpleWeather jQuery Plugin using the URL:
https://github.com/monkeecreate/jquery.simpleWeather/zipball/master

2. Unzip the downloaded compressed zip file.

3. Rename the jquery.simpleWeather-x.x.min.js to jquery.simpleWeather.min.js.

4. Place the file jquery.simpleWeather.min.js
inside the sites/all/libraries/simpleWeather directory.

5. Navigate to structure => blocks administration page and configure the Simple
Weather block by entering either a US ZIP code or WOEID and selecting a
temperature scale. Finish by placing the Simple Weather block for displaying a
block region.

Rejoice at your beautiful current weather widget with data provided by Yahoo
Weather in real-time.

DEPENDENCIES
============
Libraries API module: https://drupal.org/project/libraries

AUTHOR/MAINTAINER
======================
Justin Christoffersen (larsdesigns)
http://www.larsdesigns.net
