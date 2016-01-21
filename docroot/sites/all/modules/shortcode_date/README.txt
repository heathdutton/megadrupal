
-- SUMMARY --

Date shortcode using Drupal Shortcode API.

Embeds date with given time and format. 

For example: [date format="Y-m-d" /] 
or [date time="now" format="Y-m-d" /]. 

Default is time="now" format="Y-m-d". 
"time" can be any string which supports by strtotime()
  http://php.net/manual/en/function.strtotime.php
"format" can be any string which supports by date()
  http://php.net/manual/en/function.date.php


This module have dependency on Core's Filter and Drupal Shortcode API.
  https://drupal.org/project/shortcode



-- REQUIREMENTS --

You must have to enable core's Filter module and install the Drupal Shortcode 
API module to use this module.
Drupal Shortcode API
  https://drupal.org/project/shortcode


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --
  You must have enable Filter Module from core.
  Then install Drupal Shortcode API module and this module.
  Go Administration » Configuration » Content authoring » Text formats.
  Click configure link on which text format you want to enable.
  Check mark Shortcodes.
  At below of this page, under Shortcode tab check mark Enable Date shortcode 
  and you are done.
  Now write the short code where you need, but remember you are using the short 
  code with correct text format.



-- CONTACT --
behestee@gmail.com

Current maintainers:
* Hussain KMR Behestee - http://drupal.org/user/345066/

For more information see: http://behestee.wordpress.com/
or http://behestee.com/
