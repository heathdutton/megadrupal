CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Installation
 * Configuration
 * Notes


INTRODUCTION
------------
Adds the Weather Check block to your website.

Weather Check lets the user to create a block to display the weather 
condition at a particular location. By default, it displays the weather
at location, set in the administrator configuration page. Administrator
decide the parameters to be displayed in the block. 
Anonymous user can move the location pin across the map to view the 
weather condition at that particular location.

Administrator can choose from any of these available parameters:
1. City Name
2. Country Code
3. Temperature
4. Minimum temperature
5. Maximum temperature
6. Cloud Description
7. Pressure
8. Humidity
9. Visibility level
10.Wind speed
11.Wind degree


INSTALLATION
------------
Install as you would normally install a contributed Drupal module.
See:https://drupal.org/documentation/install/modules-themes/modules-7
for further information.


CONFIGURATION
-------------

Initial Setup:

1. Navigate to 
   Administration » Configuration » Weather Check » Weather check configuration
2. Enter the latitude and longitude of the desired location or 
   pick it directly from the map alongside
3. Select the required parameters for weather forecast
4. Save the configuration

Adding block:

Navigate to Administration » Structure » Blocks
Find "Weather Check" block and select the appropriate region


NOTES
-----

This module uses

1. AngularJS
2. Google Maps
3. OpenWeatherMap API
