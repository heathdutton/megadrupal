$Id

-- SUMMARY --

The Visitor Info module captures the IP address of your visitors, then uses the
IPInfoDB.com XML API to retrieve the geographic location and provides you with
the originating city, state, and country. Latitude and longitude are also gathered
and plotted on a Google map.

-- REQUIREMENTS --

0) UPDATE: November 15, 2010 - IPInfoDB.com has changed its APIs. Now an API key is
   required in order to do lookups. The API key is free and only requires you to
   register by going to http://www.ipinfodb.com/register.php. Once you register,
   simply activate your account by clicking the link you receive via email. You
   will find your API key on your account page.
   
   Version 6.x-1.6 of Visitor Info now includes a text box on the configuration
   page (/admin/settings/visitorinfo) to paste in your API key.
   
   Please note that a new version of Visitor Info is being written (6.x-2.0) that
   will no longer rely on IPInfoDB.com but will utilize the APIs provided by the
   GeoIP API (drupal.org/project/geoip) module.

1) In order to show locations on a Google Map, you must have a Google API key.
   If you have the GMap module installed and configured, then this requirement
   has already been met.

-- INSTALLATION --

Simply install Visitor Info like you would any other module.

1) Copy the visitorinfo folder to the modules folder in your installation.

2) Enable the module using Administer -> Modules (/admin/build/modules).

-- CONFIGURATION --

* If you have the GMap module installed and the Google API key configured,
  then Visitor Info will use that key. No need to do anything else.

  Otherwise, you can get a key by visiting the Google Maps API sign up page
  at http://code.google.com/apis/maps/signup.html.
   
* Once you have a key, go to the Visitor Info module settings page at 
  Administer -> Settings -> Visitor Info (/admin/settings/visitorinfo) and
  enter the key in the box provided.

-- CUSTOMIZATION --

* You can customize a few defaults by visiting the Visitor Info configuration
  page, Administer -> Settings -> Visitor Info (/admin/settings/visitorinfo).
  Configuration options include;

  - map width
  - map height
  - number of visitor locations to show on map
  - default beginning latitude
  - default beginning longitude
  - default beginning zoom level

* You can also configure the number of visitors to show in the Visitor Info
  list block as well as choose which data to display.

-- USAGE --

* You can view the map at http://yoursite.com/visitorinfo-map.

* There are two blocks at the Blocks management page (admin/build/block).

* The Visitor Info block will list all available information about your
  visitors in a table format. This is best displayed on a full width page
  instead of a sidebar block due to its width. If your active theme
  has a content bottom region, you might put the list block in that region
  on the visitorinfo-map page.

* The Visitor Map block will provide you with a Google map. Note that the map
  will break if displayed on the visitorinfo-map page. More work needs to be 
  done here.

* As of version 6.x-1.5, Visitor Info has been integrated with Views so all
  visitor data can be accessed and displayed via the Views module.

-- NOTE --

* The Visitor Info module starts gathering IPs once it is installed so you
  will have to allow some time before points start showing on the map.

* There are some IP addresses that simply do not have any geolocation data
  available. Apparently some ISP's do not give away geolocation information.
  See this post, http://forum.ipinfodb.com/viewtopic.php?f=7&t=1014.

* The Visitors map can be printed directly from a template by calling the theme
  function "theme('visitorinfo_map');"
